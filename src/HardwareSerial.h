/**
 * @file HardwareSerial.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-12
 * @version 2022-03-11
 */
#ifndef __HARDWARESERIAL_H__
#define __HARDWARESERIAL_H__

#include "scdinternal/fifo.h"


#if !defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE) /* STM32 HAL UART header was included */


#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 64
#endif
#ifndef SERIAL_TX_BUFFER_SIZE
#define SERIAL_TX_BUFFER_SIZE 64
#endif
typedef typename _FifoIndexTypeFor<SERIAL_RX_BUFFER_SIZE>::type rx_buffer_index_t;
typedef typename _FifoIndexTypeFor<SERIAL_TX_BUFFER_SIZE>::type tx_buffer_index_t;


/* the following defines also apply for the USB CDC driver */
#ifdef UART_WORDLENGTH_7B
#define SERIAL_6E1 0x22
#define SERIAL_6O1 0x32
#define SERIAL_6E2 0x2A
#define SERIAL_6O2 0x3A
#define SERIAL_7N1 0x04
#define SERIAL_7N2 0x0C
#endif /* UART_WORDLENGTH_7B */
#define SERIAL_7E1 0x24
#define SERIAL_7O1 0x34
#define SERIAL_7E2 0x2C
#define SERIAL_7O2 0x3C
#define SERIAL_8N1 0x06
#define SERIAL_8E1 0x26
#define SERIAL_8O1 0x36
#define SERIAL_8N2 0x0E
#define SERIAL_8E2 0x2E
#define SERIAL_8O2 0x3E


#define HAVE_HWSERIAL0


class HardwareSerial : public Stream {
private:
	friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef *);
	friend void HAL_UART_TxCpltCallback(UART_HandleTypeDef *);
	friend void HAL_UART_ErrorCallback(UART_HandleTypeDef *);
protected:
	/** @remarks UART is a subset of USART. Only UART functionality is needed here and only used as such to simplify design. */
	UART_HandleTypeDef handle[1];
	IRQn_Type irq;
	uint8_t pins[2];
	uint8_t afns;
	uint8_t recv[1];
	rx_buffer_index_t rxTail;
	volatile rx_buffer_index_t rxHead;
	volatile tx_buffer_index_t txNextTail;
	volatile tx_buffer_index_t txTail;
	tx_buffer_index_t txHead;
	uint8_t rxBuffer[SERIAL_RX_BUFFER_SIZE];
	uint8_t txBuffer[SERIAL_TX_BUFFER_SIZE];
public:
	HardwareSerial(USART_TypeDef * instance, const IRQn_Type irqNum, const PinName rxPin, const PinName txPin, const uint8_t rxAltFn = 0, const uint8_t txAltFn = 0);
	virtual ~HardwareSerial();
	
	void begin(const unsigned long baudrate) { this->begin(baudrate, SERIAL_8N1); }
	void begin(const unsigned long baudrate, const uint8_t mode) { this->begin(baudrate, mode, HAL_UART_Init); }
	void begin(const unsigned long baudrate, const uint8_t mode, HAL_StatusTypeDef (& initFn)(UART_HandleTypeDef * hUart)); /* STM32 specific */
	void end();
	
	virtual int available(void);
	virtual int availableForWrite(void);
	virtual int peek(void);
	virtual int read(void);
	virtual void flush(void);
	virtual size_t write(const uint8_t val);
	
	operator bool() { return true; }
	
	using Print::write;
private:
	bool rxBusy();
	bool txBusy();
	/* interrupt handlers */
	void rxCompleteHandler();
	void txCompleteHandler();
};


/* available if defined in board.cpp or by the user */
extern HardwareSerial Serial1;
extern HardwareSerial Serial2;
extern HardwareSerial Serial3;
extern HardwareSerial Serial4;
extern HardwareSerial Serial5;
extern HardwareSerial Serial6;
extern HardwareSerial Serial7;
extern HardwareSerial Serial8;
extern HardwareSerial Serial9;


#endif /* IS_UART_MODE */
#endif /* __HARDWARESERIAL_H__ */
