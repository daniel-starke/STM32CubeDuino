/**
 * @file CDC.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2022-03-21
 * 
 * @see https://www.silabs.com/documents/public/application-notes/AN758.pdf
 */
#include "Arduino.h"
#include "util/atomic.h"


#if !defined(STM32CUBEDUINO_DISABLE_SERIAL) && !defined(PLUGGABLE_USB_ENABLED) && defined(USBCON)
#define CDC_ACM_INTERFACE  uint8_t(this->pluggedInterface)     /* CDC ACM (needs to be first) */
#define CDC_DATA_INTERFACE uint8_t(this->pluggedInterface + 1) /* CDC data */
#define CDC_ENDPOINT_ACM   uint8_t(this->pluggedEndpoint)      /* needs to be first */
#define CDC_ENDPOINT_OUT   uint8_t(this->pluggedEndpoint + 1)
#define CDC_ENDPOINT_IN    uint8_t(this->pluggedEndpoint + 2)

#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN

#define CDC_LINESTATE_DTR    0x01 /* Data Terminal Ready */
#define CDC_LINESTATE_RTS    0x02 /* Ready To Send */

#define CDC_LINESTATE_READY  (CDC_LINESTATE_RTS | CDC_LINESTATE_DTR)

/**
 * Line Coding Structure
 * 
 * Offset| Field       | Size | Description
 * :----:|-------------|:----:|----------------------------------------------
 *    0  | dwDTERate   |  4   | Data terminal rate, in bits per second
 *    4  | bCharFormat |  1   | Stop bits
 *       |             |      | - 0 - 1 Stop bit
 *       |             |      | - 1 - 1.5 Stop bits
 *       |             |      | - 2 - 2 Stop bits
 *    5  | bParityType |  1   | Parity
 *       |             |      | - 0 - None
 *       |             |      | - 1 - Odd
 *       |             |      | - 2 - Even
 *       |             |      | - 3 - Mark
 *       |             |      | - 4 - Space
 *    6  | bDataBits   |  1   | Data bits (5, 6, 7, 8 or 16).
 */
struct LineInfo {
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
} __attribute__((packed));


static volatile LineInfo deviceLineInfo = {115200, 0x00, 0x00, 0x08};
///**
// * Control Signal Bitmap
// * - 0x01 - DCD (V.24 signal 109)
// * - 0x02 - DSR (V.24 signal 106)
// * - 0x04 - break state
// * - 0x08 - RING (V.24 signal 125)
// * - 0x10 - framing error
// * - 0x20 - parity error
// * - 0x40 - overrun
// */
//static volatile uint8_t deviceLineState = 0; // currently unsupported
static volatile uint8_t hostLineState = 0; /* 0x01 RTS (V.24 signal 105) | 0x02 DTR (V.24 signal 108) */
static volatile int32_t breakValue = -1;
int _serialPeek = -1;


/**
 * Constructor.
 */
Serial_::Serial_():
	PluggableUSBModule(3, 2, epType)
{
	/* same order as CDC_ENDPOINT_XXX */
	this->epType[0] = USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0);
	this->epType[1] = USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_OUT(0);
	this->epType[2] = USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_IN(0);
	PluggableUSB().plug(this);
}


/**
 * Starts the serial interface with the given settings.
 * 
 * @param[in] baudrate - physical baud rate (e.g. 9600)
 * @param[in] mode - data framing mode (e.g. SERIAL_8N1)
 * @remarks Baud rate and mode will be reported to the host.
 * This has no influence on the actual transmission behavior.
 */
void Serial_::begin(const unsigned long baudrate, const uint8_t mode) {
	deviceLineInfo.dwDTERate = uint32_t(baudrate);
	deviceLineInfo.bCharFormat = uint8_t(((mode & 0x08) == 0x08) ? 2 : 0); /* 2 or 1 stop bit */
	switch ((mode & 0x07)) {
	case 0x02: deviceLineInfo.bDataBits = 6; break;
	case 0x04: deviceLineInfo.bDataBits = 7; break;
	case 0x06: deviceLineInfo.bDataBits = 8; break;
	default:   deviceLineInfo.bDataBits = 8; break; /* fallback default */
	}
	if ((mode & 0x30) == 0x30) {
		/* odd */
		deviceLineInfo.bParityType = 1;
	} else if ((mode & 0x20) == 0x20) {
		/* even */
		deviceLineInfo.bParityType = 2;
	} else {
		/* none */
		deviceLineInfo.bParityType = 0;
	}
	//deviceLineState = 0x03; /* online */ // currently unsupported
}


/**
 * Stops the serial interface.
 */
void Serial_::end(void) {
}


/**
 * Returns the number of available bytes in the receive buffer.
 * 
 * @return number of bytes available for read
 */
int Serial_::available(void) {
	return USBDevice.available(CDC_RX) + ((_serialPeek >= 0) ? 1 : 0);
}


/**
 * Returns the number of bytes which can be written without buffer overflow.
 * 
 * @return number of bytes available for write
 */
int Serial_::availableForWrite(void) {
	return USBDevice.available(CDC_TX);
}


/**
 * Returns the first byte in the receive buffer without removing it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int Serial_::peek(void) {
	if (_serialPeek < 0) _serialPeek = USBDevice.recv(CDC_RX);
	return _serialPeek;
}


/**
 * Returns a byte from the receive buffer and removes it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int Serial_::read(void) {
	if (_serialPeek >= 0) {
		const int c = _serialPeek;
		_serialPeek = -1;
		return c;
	}
	return USBDevice.recv(CDC_RX);
}


/**
 * Waits until all data in transmission queue is sent.
 */
void Serial_::flush(void) {
	USBDevice.flush(CDC_TX);
}


/**
 * Sends the given byte.
 * 
 * @param[in] val - byte value to send
 * @return number of bytes written to the transmission queue
 */
size_t Serial_::write(const uint8_t val) {
	return this->write(&val, 1);
}

/**
 * Sends the requested number of bytes from the given buffer.
 * 
 * @param[in] buffer - data to send
 * @param[in] length - size of the data in the buffer
 * @return number of bytes written to the transmission queue
 */
size_t Serial_::write(const uint8_t * buffer, size_t size) {
	/* send only if the OS signaled that the connection is open by setting DTR on */
#ifdef STM32CUBEDUINO_LEGACY_API
	if (hostLineState > 0)	{
#else /* not STM32CUBEDUINO_LEGACY_API */
	/* see https://github.com/arduino/ArduinoCore-avr/issues/63 */
	if ( this->dtr() ) {
#endif /* not STM32CUBEDUINO_LEGACY_API */
		const int res = USBDevice.send(CDC_TX, buffer, size);
		if (res > 0) {
			return size_t(res);
		} else {
			this->setWriteError();
			return 0;
		}
	}
	this->setWriteError();
	return 0;
}


/**
 * Returns the connection status.
 * 
 * @return true if CDC is up and running, else false
 */
Serial_::operator bool() {
	if ((hostLineState & CDC_LINESTATE_READY) != 0) return true;
	delay(10);
	return false;
}


/**
 * Returns the most recent break value.
 * 
 * @return break value
 */
int32_t Serial_::readBreak() {
	/* needed to avoid read/write concurrency in interrupt handler */
	int32_t res;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		res = breakValue;
		breakValue = -1;
	}
	return res;
}


/**
 * Returns the current baud rate configuraion.
 * 
 * @return baud rate
 */
uint32_t Serial_::baud() {
	return deviceLineInfo.dwDTERate;
}


/**
 * Returns the current stop bit configuration.
 * 
 * @return number of stop bits
 */
uint8_t Serial_::stopbits() {
	return deviceLineInfo.bCharFormat;
}


/**
 * Returns the current parity configuration.
 * 
 * @return parity type
 */
uint8_t Serial_::paritytype() {
	return deviceLineInfo.bParityType;
}


/**
 * Returns the current data bits configuration.
 * 
 * @return number of data bits
 */
uint8_t Serial_::numbits() {
	return deviceLineInfo.bDataBits;
}


/**
 * Returns the current line state for DTR (data terminal ready).
 * 
 * @return DTR state
 */
bool Serial_::dtr() {
	return (hostLineState & CDC_LINESTATE_DTR) != 0;
}


/**
 * Returns the current line state for RTS (ready to send).
 * 
 * @return RTS state
 */
bool Serial_::rts() {
	return (hostLineState & CDC_LINESTATE_RTS) != 0;
}


/**
 * Sends the USB device descriptor to the host.
 * 
 * @param[in] setup - USB setup message
 * @return bytes sent
 */
int Serial_::getDescriptor(USBSetup & /* setup */) {
	return 0;
}


/**
 * Sends the USB interface description to the host.
 * 
 * @param[in,out] interfaceCount - index of this interface
 * @return bytes sent
 */
int Serial_::getInterface(uint8_t * interfaceCount) {
	static const CDCDescriptor cdcInterface = {
		D_IAD(0, 2, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),
		/*	CDC communication interface */
		D_INTERFACE(CDC_ACM_INTERFACE, 1, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),
		D_CDCCS(CDC_HEADER, 0x10, 0x01),                           /* header (1.10 BCD) */
		D_CDCCS(CDC_CALL_MANAGEMENT, 1, 1),                        /* device handles call management (not) */
		D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT, 6),              /* SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported */
		D_CDCCS(CDC_UNION, CDC_ACM_INTERFACE, CDC_DATA_INTERFACE), /* communication interface is master, data interface is slave 0 */
		D_ENDPOINT(USB_ENDPOINT_IN(CDC_ENDPOINT_ACM), USB_ENDPOINT_TYPE_INTERRUPT, 0x10, 0x40),
		/*	CDC data interface */
		D_INTERFACE(CDC_DATA_INTERFACE, 2, CDC_DATA_INTERFACE_CLASS, 0, 0),
		D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0),
		D_ENDPOINT(USB_ENDPOINT_IN(CDC_ENDPOINT_IN), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0)
	};
	(*interfaceCount) = uint8_t((*interfaceCount) + 2); /* uses 2 */
	return USBDevice.sendControl(&cdcInterface, sizeof(cdcInterface));
}


/**
 * USB setup handler.
 * 
 * @param[in] setup - USB setup message
 * @return true on success, else false
 */
bool Serial_::setup(USBSetup & setup) {
	if (setup.wIndex != CDC_ACM_INTERFACE) return false;
	switch (setup.bmRequestType) {
	case REQUEST_DEVICETOHOST_CLASS_INTERFACE:
		if (setup.bRequest == CDC_GET_LINE_CODING) {
			USBDevice.sendControl(const_cast<const LineInfo *>(&deviceLineInfo), sizeof(deviceLineInfo));
			return true;
		}
		break;
	case REQUEST_HOSTTODEVICE_CLASS_INTERFACE:
		switch (setup.bRequest) {
		case CDC_SEND_BREAK:
			breakValue = (uint16_t(setup.wValueH) << 8) | setup.wValueL;
			break;
		case CDC_SET_LINE_CODING:
			USBDevice.recvControl(const_cast<LineInfo *>(&deviceLineInfo), sizeof(deviceLineInfo));
			break;
		case CDC_SET_CONTROL_LINE_STATE:
			hostLineState = setup.wValueL;
			break;
		default:
			return false;
			break;
		}
		return true;
		break;
	default:
		break;
	}
	return false;
}


/**
 * Returns the USB device serial number.
 * 
 * @param[out] name - copy serial number to this buffer
 * @return number of bytes copied
 */
uint8_t Serial_::getShortName(char * name) {
	name[ 0] = ' ';
	name[ 1] = 'C';
	name[ 2] = 'D';
	name[ 3] = 'C';
	name[ 4] = ' ';
	name[ 5] = 'I';
	name[ 6] = 'n';
	name[ 7] = 't';
	name[ 8] = 'e';
	name[ 9] = 'r';
	name[10] = 'f';
	name[11] = 'a';
	name[12] = 'c';
	name[13] = 'e';
	return 14;
}


Serial_ Serial;


#endif /* USBCON */
