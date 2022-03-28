/**
 * @file serial.c
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-26
 * @version 2022-03-28
 */
#include <stdio.h>
#include <string.h>
#include "serial.h"
#include "target.h"

#if defined(PCF_IS_WIN)
#undef WIN32_LEAN_AND_MEAN
#include <windows.h>


/** Check of the serial interface device was removed every given milliseconds. */
#define SER_CHECK_INTERVAL 100


/**
 * Internal serial interface handle.
 */
struct tSerial {
	HANDLE port;
	size_t speed;
	tSerFraming framing;
	tSerFlowCtrl flow;
	tSerStatusLine status;
	OVERLAPPED recvStruct[1];
	OVERLAPPED sendStruct[1];
	char * devPath;
	HANDLE termEvent;
	HANDLE checkThread;
	volatile int removed;
};


/**
 * Fills the given DCB structure with the passed configuration settings.
 * 
 * @param[out] config - DCB structure to fill
 * @param[in] speed - serial interface speed in Baud
 * @param[in] framing - serial interface framing (e.g. SFR_8N1)
 * @param[in] flow - serial interface flow control setting (e.g. SFC_NONE)
 * @retrun 1 on success, else 0
 */
static int ser_fillConfig(DCB * config, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	static const char data[]   = {'7', '8', '7', '8', '7', '8', '7', '8', '7', '8', '7', '8'};
	static const char parity[] = {'N', 'N', 'N', 'N', 'E', 'E', 'E', 'E', 'O', 'O', 'O', 'O'};
	static const char stop[]   = {'1', '1', '2', '2', '1', '1', '2', '2', '1', '1', '2', '2'};
	char buffer[128] = { 0 };
	
	snprintf(buffer, sizeof(buffer), "baud=%u parity=%c data=%c stop=%c", (unsigned)speed, parity[framing], data[framing], stop[framing]);
	config->DCBlength = sizeof(DCB);
	if (BuildCommDCBA(buffer, config) == 0) return 0;
	switch (flow) {
	case SFC_NONE:
		config->fOutX = FALSE;
		config->fInX = FALSE;
		config->fOutxCtsFlow = FALSE;
		config->fOutxDsrFlow = FALSE;
		config->fDsrSensitivity = FALSE;
		config->fRtsControl = RTS_CONTROL_ENABLE;
		config->fDtrControl = DTR_CONTROL_ENABLE;
		break;
	case SFC_SW:
		config->fOutX = TRUE;
		config->fInX = TRUE;
		config->fOutxCtsFlow = FALSE;
		config->fOutxDsrFlow = FALSE;
		config->fDsrSensitivity = FALSE;
		config->fRtsControl = RTS_CONTROL_ENABLE;
		config->fDtrControl = DTR_CONTROL_ENABLE;
		break;
	case SFC_HW:
		config->fOutX = FALSE;
		config->fInX = FALSE;
		config->fOutxCtsFlow = TRUE;
		config->fOutxDsrFlow = TRUE;
		config->fDsrSensitivity = TRUE;
		config->fRtsControl = RTS_CONTROL_HANDSHAKE;
		config->fDtrControl = DTR_CONTROL_HANDSHAKE;
		break;
	}
	config->fErrorChar = FALSE;
	config->fNull = FALSE;
	config->fAbortOnError = FALSE;
	return 1;
}


/**
 * Background thread which checks if the serial interface device was removed.
 * 
 * @param[in] lpParam - lpParam - user data
 * @return 1 on success, else 0
 */
static DWORD WINAPI ser_checkThread(LPVOID lpParam) {
	tSerial * ser = (tSerial *)lpParam;
	for (;;) {
		SetLastError(0);
		const HANDLE hnd = CreateFileA(ser->devPath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
		const DWORD err = GetLastError();
		if (hnd != INVALID_HANDLE_VALUE) CloseHandle(hnd);
		if (err == ERROR_FILE_NOT_FOUND) {
			ser->removed = 1;
			return 1;
		}
		switch (WaitForSingleObject(ser->termEvent, (DWORD)(SER_CHECK_INTERVAL))) {
		case WAIT_OBJECT_0: return 1;
		case WAIT_TIMEOUT: break;
		default: return 0;
		}
	}
	return 0;
}


/**
 * Create a new serial interface context based on the given device path and configuration.
 * 
 * @param[in] device - serial interface device
 * @param[in] speed - serial interface speed in Baud
 * @param[in] framing - serial interface framing (e.g. SFR_8N1)
 * @param[in] flow - serial interface flow control setting (e.g. SFC_NONE)
 * @retrun Handle on success, else NULL.
 */
tSerial * ser_create(const char * device, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	size_t devPathLen;
	DCB params = { 0 };
	COMMTIMEOUTS tout = { 0 };
	if (device == NULL) return NULL;
	
	tSerial * res = (tSerial *)calloc(1, sizeof(tSerial));
	if (res == NULL) return NULL;
	res->port = INVALID_HANDLE_VALUE;
	
	devPathLen = strlen(device) + 5;
	res->devPath = (char *)malloc(devPathLen * sizeof(char));
	if (res->devPath == NULL) goto onError;
	snprintf(res->devPath, devPathLen, "\\\\.\\%s", device);
	
	res->port = CreateFileA(res->devPath, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	if (res->port == INVALID_HANDLE_VALUE) goto onError;
	
	if (ser_fillConfig(&params, speed, framing, flow) == 0) goto onError;
	if (SetCommState(res->port, &params) == 0) goto onError;
	
	tout.ReadIntervalTimeout = MAXDWORD;
	tout.ReadTotalTimeoutConstant = 0;
	tout.ReadTotalTimeoutMultiplier = 0;
	tout.WriteTotalTimeoutConstant = 0;
	tout.WriteTotalTimeoutMultiplier = 0;
	if (SetCommTimeouts(res->port, &tout) == 0) goto onError;
	
	res->speed = speed;
	res->framing = framing;
	res->flow = flow;
	res->status = (tSerStatusLine)(SL_RTS | SL_DTR);
	ser_getLines(res);
	res->recvStruct->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (res->recvStruct->hEvent == NULL) goto onError;
	res->sendStruct->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (res->sendStruct->hEvent == NULL) goto onError;
	res->termEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (res->termEvent == NULL) goto onError;
	res->checkThread = CreateThread(NULL, 0, ser_checkThread, (LPVOID)res, 0, NULL);
	if (res->checkThread == NULL) goto onError;
	
	return res;
onError:
	ser_delete(res);
	return NULL;
}


/**
 * Sets the serial interface configuration.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] speed - new speed value
 * @param[in] framing - new framing
 * @param[in] flow - new flow control setting
 * @return 1 on success, else 0
 */
int ser_setConfig(tSerial * ser, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	if (ser == NULL || ser->removed != 0) return 0;
	if (ser->speed == speed && ser->framing == framing && ser->flow == flow) return 1;
	DCB params = { 0 };
	int res = 0;
	
	if (ser_fillConfig(&params, speed, framing, flow) == 0) goto onError;
	if (SetCommState(ser->port, &params) == 0) goto onError;
	
	ser->speed = speed;
	ser->framing = framing;
	ser->flow = flow;
	ser->status = (tSerStatusLine)(ser->status & (SL_RTS | SL_DTR));
	ser_getLines(ser);
	
	res = 1;
onError:
	return res;
}


/**
 * Returns the current serial status lines.
 * 
 * @param[in,out] ser - serial interface context
 * @return serial status lines
 */
tSerStatusLine ser_getLines(tSerial * ser) {
	DWORD status;
	if (ser == NULL || ser->removed != 0) return (tSerStatusLine)0;
	ser->status = (tSerStatusLine)(ser->status & (SL_RTS | SL_DTR));
	if (GetCommModemStatus(ser->port, &status) != 0) {
		if ((status & MS_CTS_ON)  != 0) ser->status |= SL_CTS;
		if ((status & MS_DSR_ON)  != 0) ser->status |= SL_DSR;
		if ((status & MS_RLSD_ON) != 0) ser->status |= SL_DCD;
		if ((status & MS_RING_ON) != 0) ser->status |= SL_RING;
	} else {
		ser->status = (tSerStatusLine)0;
	}
	return ser->status;
}


/**
 * Sets the serial status lines RTS and DTR accordingly.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] status - new status line values (only RTS and DTR are set)
 * @return 1 on success, else 0
 */
int ser_setLines(tSerial * ser, const tSerStatusLine status) {
	if (ser == NULL || ser->removed != 0) return 0;
	
	/* update RTS on change */
	if (((ser->status ^ status) & SL_RTS) != 0) {
		if (EscapeCommFunction(ser->port, ((status & SL_RTS) != 0) ? SETRTS : CLRRTS) == 0) return 0;
		ser->status = ((tSerStatusLine)(~SL_RTS) & ser->status) | (tSerStatusLine)(SL_RTS & status);
	}
	
	/* update DTR on change */
	if (((ser->status ^ status) & SL_DTR) != 0) {
		if (EscapeCommFunction(ser->port, ((status & SL_DTR) != 0) ? SETDTR : CLRDTR) == 0) return 0;
		ser->status = ((tSerStatusLine)(~SL_DTR) & ser->status) | (tSerStatusLine)(SL_DTR & status);
	}
	
	return 1;
}


/**
 * Reads a chunk of data from the given serial interface to the passed buffer.
 * 
 * @param[in,out] ser - serial interface context
 * @param[out] buf - received serial data chunk
 * @param[in] size - received serial data chunk size
 * @param[in] timeout - timeout in milliseconds
 * @return number of bytes written, -1 on error, -2 on timeout
 */
ssize_t ser_read(tSerial * ser, uint8_t * buf, const size_t size, const size_t timeout) {
	if (ser == NULL || ser->removed != 0 || buf == NULL) return -1;
	if (size == 0) return 0;
	COMSTAT comStat = { 0 };
	DWORD dwErrorFlags = 0;
	DWORD dwCommEvent = 0;
	DWORD dwRead = 0;
	const DWORD start = timeGetTime();
	size_t diff;
	size_t tout = timeout;
	
	do {
		ResetEvent(ser->recvStruct->hEvent);
		ClearCommError(ser->port, &dwErrorFlags, &comStat);
		/* we need to set the mask before every wait */
		if (SetCommMask(ser->port, EV_RXCHAR) == 0) return -1;
		if (comStat.cbInQue <= 0) {
			/* wait for serial data */
			if (WaitCommEvent(ser->port, &dwCommEvent, ser->recvStruct) == 0) {
				if (GetLastError() != ERROR_IO_PENDING) return -1; /* abort */
				switch (WaitForSingleObject(ser->recvStruct->hEvent, (DWORD)tout)) {
				case WAIT_OBJECT_0:
					break;
				case WAIT_TIMEOUT:
					goto onTimeout;
				default:
					return -1;
				}
				if (GetOverlappedResult(ser->port, ser->recvStruct, &dwCommEvent, TRUE) == 0) {
					return -1; /* overlapping error */
				}
			}
			if ((dwCommEvent & EV_RXCHAR) == 0) {
				/* an event different from our event mask triggered -> try again */
				goto onNoEvent;
			}
		}
   	 
		/* read serial data */
		ResetEvent(ser->recvStruct->hEvent);
		if (ReadFile(ser->port, buf, (DWORD)size, &dwRead, ser->recvStruct) == 0) {
			if (GetLastError() != ERROR_IO_PENDING) return -1; /* abort */
			switch (WaitForSingleObject(ser->recvStruct->hEvent, (DWORD)tout)) {
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				goto onTimeout;
			default:
				return -1;
			}
			if (GetOverlappedResult(ser->port, ser->recvStruct, &dwRead, TRUE) == 0) {
				return -1; /* overlapping error */
			}
		}
onNoEvent:
		diff = (size_t)(timeGetTime() - start);
		tout = (diff < timeout) ? (size_t)(timeout - diff) : 0;
	} while (dwRead <= 0 && tout > 0);
	
onTimeout:
	if (dwRead > 0) return (ssize_t)dwRead;
	return -2; /* timeout */
}


/**
 * Writes a chunk of data to the given serial interface from the passed buffer.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] buf - output serial data chunk
 * @param[in] size - output serial data chunk size
 * @param[in] timeout - timeout in milliseconds (infinite if set to 0)
 * @return number of bytes written, -1 on error, -2 on timeout
 */
ssize_t ser_write(tSerial * ser, const uint8_t * buf, const size_t size, const size_t timeout) {
	if (ser == NULL || ser->removed != 0 || buf == NULL) return -1;
	if (size == 0) return 0;
	size_t rem = size;
	DWORD dwWritten;
	ssize_t res = -1;
	
	while (rem > 0) {
		ResetEvent(ser->sendStruct->hEvent);
		if (WriteFile(ser->port, buf, (DWORD)rem, &dwWritten, ser->sendStruct) == 0) {
			if (GetLastError() != ERROR_IO_PENDING) {
				goto onError;
			} else {
				switch (WaitForSingleObject(ser->sendStruct->hEvent, (timeout != 0) ? (DWORD)timeout : INFINITE)) {
				case WAIT_OBJECT_0:
					if (GetOverlappedResult(ser->port, ser->sendStruct, &dwWritten, TRUE) == 0) goto onError;
					break;
				case WAIT_TIMEOUT:
					res = -2;
					goto onError;
				default:
					goto onError;
				}
			}
		}
		buf = buf + (size_t)dwWritten;
		rem = (size_t)(rem - (size_t)dwWritten);
		/* wrote data but not all; timeout not reached -> try again */
	}
	
onError:
	if (rem < size) return (ssize_t)(size - rem);
	return res;
}


/**
 * Clears the input and output buffer for the given serial interface.
 * 
 * @param[in,out] ser - serial interface context
 * @return 1 on success, else 0
 */
int ser_clear(tSerial * ser) {
	if (ser == NULL || ser->removed != 0) return 0;
	ClearCommError(ser->port, NULL, NULL);
	return (PurgeComm(ser->port, PURGE_RXCLEAR | PURGE_TXCLEAR) != 0) ? 1 : 0;
}


/**
 * Frees the given serial interface context. The context is invalid after this call.
 * 
 * @param[in,out] ser - context to free
 */
void ser_delete(tSerial * ser) {
	if (ser == NULL) return;
	if (ser->checkThread != NULL) {
		if (ser->termEvent != NULL) SetEvent(ser->termEvent);
		WaitForSingleObject(ser->checkThread, INFINITE);
		CloseHandle(ser->checkThread);
	}
	if (ser->port != INVALID_HANDLE_VALUE) CloseHandle(ser->port);
	if (ser->recvStruct->hEvent != NULL) CloseHandle(ser->recvStruct->hEvent);
	if (ser->sendStruct->hEvent != NULL) CloseHandle(ser->sendStruct->hEvent);
	if (ser->devPath != NULL) free(ser->devPath);
	if (ser->termEvent != NULL) CloseHandle(ser->termEvent);
	free(ser);
}


#elif defined(PCF_IS_LINUX) /**********************************************************************/
#include <fcntl.h> 
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>


/**
 * Internal serial interface handle.
 */
struct tSerial {
	int port;
	size_t speed;
	tSerFraming framing;
	tSerFlowCtrl flow;
	tSerStatusLine status;
	struct termios oldSettings;
};


/**
 * Fills the given termios structure with the passed configuration settings.
 * 
 * @param[out] config - termios structure to fill
 * @param[in] speed - serial interface speed in Baud
 * @param[in] framing - serial interface framing (e.g. SFR_8N1)
 * @param[in] flow - serial interface flow control setting (e.g. SFC_NONE)
 * @retrun 1 on success, else 0
 */
static int ser_fillConfig(struct termios * config, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	static const tcflag_t data[]   = {CS7, CS8, CS7, CS8, CS7, CS8, CS7, CS8, CS7, CS8, CS7, CS8};
	static const tcflag_t parity[] = {0, 0, 0, 0, PARENB, PARENB, PARENB, PARENB, PARODD, PARODD, PARODD, PARODD};
	static const tcflag_t stop[]   = {0, 0, CSTOPB, CSTOPB, 0, 0, CSTOPB, CSTOPB, 0, 0, CSTOPB, CSTOPB};
	speed_t baudrate = 0;
	
	config->c_iflag = IGNBRK | INPCK;
	config->c_lflag = 0;
	config->c_oflag = 0;
	config->c_cc[VMIN] = 0;
	config->c_cc[VTIME] = 0;
	config->c_cflag = HUPCL | CREAD | data[framing] | parity[framing] | stop[framing];
	
	switch (flow) {
	case SFC_NONE:
		break;
	case SFC_SW:
		config->c_iflag |= (IXON | IXOFF | IXANY);
		break;
	case SFC_HW:
		config->c_cflag |= CRTSCTS;
		break;
	}
	
	switch (speed) {
#ifdef B50
	case 50: baudrate = B50; break;
#endif
#ifdef B75
	case 75: baudrate = B75; break;
#endif
#ifdef B110
	case 110: baudrate = B110; break;
#endif
#ifdef B134
	case 134: baudrate = B134; break;
#endif
#ifdef B150
	case 150: baudrate = B150; break;
#endif
#ifdef B200
	case 200: baudrate = B200; break;
#endif
#ifdef B300
	case 300: baudrate = B300; break;
#endif
#ifdef B600
	case 600: baudrate = B600; break;
#endif
#ifdef B1200
	case 1200: baudrate = B1200; break;
#endif
#ifdef B1800
	case 1800: baudrate = B1800; break;
#endif
#ifdef B2400
	case 2400: baudrate = B2400; break;
#endif
#ifdef B4800
	case 4800: baudrate = B4800; break;
#endif
#ifdef B9600
	case 9600: baudrate = B9600; break;
#endif
#ifdef B19200
	case 19200: baudrate = B19200; break;
#endif
#ifdef B38400
	case 38400: baudrate = B38400; break;
#endif
#ifdef B57600
	case 57600: baudrate = B57600; break;
#endif
#ifdef B115200
	case 115200: baudrate = B115200; break;
#endif
#ifdef B230400
	case 230400: baudrate = B230400; break;
#endif
#ifdef B460800
	case 460800: baudrate = B460800; break;
#endif
#ifdef B500000
	case 500000: baudrate = B500000; break;
#endif
#ifdef B576000
	case 576000: baudrate = B576000; break;
#endif
#ifdef B921600
	case 921600: baudrate = B921600; break;
#endif
#ifdef B1000000
	case 1000000: baudrate = B1000000; break;
#endif
#ifdef B1152000
	case 1152000: baudrate = B1152000; break;
#endif
#ifdef B1500000
	case 1500000: baudrate = B1500000; break;
#endif
#ifdef B2000000
	case 2000000: baudrate = B2000000; break;
#endif
#ifdef B2500000
	case 2500000: baudrate = B2500000; break;
#endif
#ifdef B3000000
	case 3000000: baudrate = B3000000; break;
#endif
#ifdef B3500000
	case 3500000: baudrate = B3500000; break;
#endif
#ifdef B4000000
	case 4000000: baudrate = B4000000; break;
#endif
	default: return 0;
	}
	
	cfsetospeed(config, baudrate);
	cfsetispeed(config, baudrate);
	
	return 1;
}


/**
 * Create a new serial interface context based on the given device path and configuration.
 * 
 * @param[in] device - serial interface device
 * @param[in] speed - serial interface speed in Baud
 * @param[in] framing - serial interface framing (e.g. SFR_8N1)
 * @param[in] flow - serial interface flow control setting (e.g. SFC_NONE)
 * @retrun Handle on success, else NULL.
 */
tSerial * ser_create(const char * device, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	struct termios settings;
	if (device == NULL) return NULL;
	
	tSerial * res = (tSerial *)calloc(1, sizeof(tSerial));
	if (res == NULL) return NULL;
	res->port = -1;
	
	res->port = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (res->port < 0) goto onError;
	
	if (tcgetattr(res->port, &settings) != 0) goto onError;
	memcpy(&(res->oldSettings), &settings, sizeof(struct termios));
	
	if (ser_fillConfig(&settings, speed, framing, flow) == 0) goto onError;
	tcflush(res->port, TCIFLUSH);
	/* DrMemory may warn about uninitialized read here; ignore it for now */
	if (tcsetattr(res->port, TCSANOW, &settings) != 0) goto onError;
	
	res->speed = speed;
	res->framing = framing;
	res->flow = flow;
	ser_getLines(res);
	return res;
onError:
	ser_delete(res);
	return NULL;
}


/**
 * Sets the serial interface configuration.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] speed - new speed value
 * @param[in] framing - new framing
 * @param[in] flow - new flow control setting
 * @return 1 on success, else 0
 */
int ser_setConfig(tSerial * ser, const size_t speed, const tSerFraming framing, const tSerFlowCtrl flow) {
	struct termios settings;
	if (ser == NULL) return 0;
	if (ser->speed == speed && ser->framing == framing && ser->flow == flow) return 1;
	
	if (ser_fillConfig(&settings, speed, framing, flow) != 1) return 0;
	tcflush(ser->port, TCIFLUSH);
	if (tcsetattr(ser->port, TCSANOW, &settings) != 0) return 0;
	
	return 1;
}


/**
 * Returns the current serial status lines.
 * 
 * @param[in,out] ser - serial interface context
 * @return serial status lines
 */
tSerStatusLine ser_getLines(tSerial * ser) {
	int status;
	if (ser == NULL) return (tSerStatusLine)0;
	ser->status = (tSerStatusLine)0;
	if (ioctl(ser->port, TIOCMGET, &status) == 0) {
		if ((status & TIOCM_RTS) != 0)               ser->status |= SL_RTS;
		if ((status & TIOCM_CTS) != 0)               ser->status |= SL_CTS;
		if ((status & (TIOCM_LE | TIOCM_DSR)) != 0)  ser->status |= SL_DSR;
		if ((status & (TIOCM_CAR | TIOCM_CD)) != 0)  ser->status |= SL_DCD;
		if ((status & TIOCM_DTR) != 0)               ser->status |= SL_DTR;
		if ((status & (TIOCM_RNG | TIOCM_RI)) != 0)  ser->status |= SL_RING;
	} else {
		ser->status = (tSerStatusLine)0;
	}
	return ser->status;
}


/**
 * Sets the serial status lines RTS and DTR accordingly.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] status - new status line values (only RTS and DTR are set)
 * @return 1 on success, else 0
 */
int ser_setLines(tSerial * ser, const tSerStatusLine status) {
	if (ser == NULL) return 0;
	int newStatus;
	
	/* update DTR/RTS on change */
	if (((ser->status ^ status) & (SL_RTS | SL_DTR)) != 0) {
		if (ioctl(ser->port, TIOCMGET, &newStatus) != 0) return 0;
		newStatus &= ~(TIOCM_RTS | TIOCM_DTR);
		if ((status & SL_RTS) != 0) newStatus |= TIOCM_RTS;
		if ((status & SL_DTR) != 0) newStatus |= TIOCM_DTR;
		if (ioctl(ser->port, TIOCMSET, &newStatus) != 0) return 0;
	}
	
	return 1;
}


/**
 * Reads a chunk of data from the given serial interface to the passed buffer.
 * 
 * @param[in,out] ser - serial interface context
 * @param[out] buf - received serial data chunk
 * @param[in] size - received serial data chunk size
 * @param[in] timeout - timeout in milliseconds
 * @return number of bytes written, -1 on error, -2 on timeout
 */
ssize_t ser_read(tSerial * ser, uint8_t * buf, const size_t size, const size_t timeout) {
	if (ser == NULL || buf == NULL) return -1;
	if (size == 0) return 0;
	ssize_t received = 0;
	struct timeval tout;
	fd_set fds;
	int sRes;
	
	FD_ZERO(&fds);
	FD_SET(ser->port, &fds);
	tout.tv_sec = (time_t)(timeout / 1000);
	tout.tv_usec = (suseconds_t)((timeout % 1000) * 1000);
	
	/* wait for serial data */
	sRes = select(ser->port + 1, &fds, NULL, NULL, &tout);
	if (sRes < 0) {
		return -1;
	} else if (sRes == 0 || FD_ISSET(ser->port, &fds) == 0) {
		return -2;
	}
   	
	/* read serial data */
	received = read(ser->port, buf, size);
	if (received <= 0) return -1;
	return received;
}


/**
 * Writes a chunk of data to the given serial interface from the passed buffer.
 * 
 * @param[in,out] ser - serial interface context
 * @param[in] buf - output serial data chunk
 * @param[in] size - output serial data chunk size
 * @param[in] timeout - timeout in milliseconds (infinite if set to 0)
 * @return number of bytes written, -1 on error, -2 on timeout
 */
ssize_t ser_write(tSerial * ser, const uint8_t * buf, const size_t size, const size_t timeout) {
	if (ser == NULL || buf == NULL) return -1;
	if (size == 0) return 0;
	size_t rem = size;
	ssize_t written;
	struct timeval tout;
	fd_set fds;
	int sRes;
	ssize_t res = -1;
	
	while (rem > 0) {
		FD_ZERO(&fds);
		FD_SET(ser->port, &fds);
		tout.tv_sec = (time_t)(timeout / 1000);
		tout.tv_usec = (suseconds_t)((timeout % 1000) * 1000);
		sRes = select(ser->port + 1, NULL, &fds, NULL, &tout);
		if (sRes < 0) {
			goto onError;
		} else if (sRes == 0 || FD_ISSET(ser->port, &fds) == 0) {
			res = -2;
			goto onError;
		}
		/* write serial data */
		written = write(ser->port, buf, rem);
		if (written <= 0) goto onError;
		buf = buf + (size_t)written;
		rem = (size_t)(rem - (size_t)written);
		/* wrote data but not all; timeout not reached -> try again */
	}
	
onError:
	if (rem < size) return (ssize_t)(size - rem);
	return res;
}


/**
 * Clears the input and output buffer for the given serial interface.
 * 
 * @param[in,out] ser - serial interface context
 * @return 1 on success, else 0
 */
int ser_clear(tSerial * ser) {
	if (ser == NULL) return 0;
	return (tcflush(ser->port, TCIFLUSH) == 0) ? 1 : 0;
}


/**
 * Frees the given serial interface context. The context is invalid after this call.
 * 
 * @param[in,out] ser - context to free
 */
void ser_delete(tSerial * ser) {
	if (ser == NULL) return;
	if (ser->port >= 0) {
		tcsetattr(ser->port, TCSANOW, &(ser->oldSettings));
		close(ser->port);
	}
	free(ser);
}
#else /* not PCF_IS_WIN and not PCF_IS_LINUX */
#error Unsupported target OS.
#endif
