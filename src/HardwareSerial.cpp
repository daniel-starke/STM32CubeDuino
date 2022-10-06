/**
 * @file HardwareSerial.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-12
 * @version 2022-10-06
 */
#include "Arduino.h"
#include "wiring_irq.h"


#if !defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(IS_UART_MODE) /* STM32 HAL UART header was included */


#ifndef UART_IRQ_PRIO
#error Please define UART_IRQ_PRIO in board.hpp.
#endif
#ifndef UART_IRQ_SUBPRIO
#error Please define UART_IRQ_SUBPRIO in board.hpp.
#endif


/** Local definition of the RX circular buffer queue within HardwareSerial. */
#define RX_QUEUE this->rxBuffer, SERIAL_RX_BUFFER_SIZE, this->rxHead, this->rxTail
/** Local definition of the TX circular buffer queue within HardwareSerial. */
#define TX_QUEUE this->txBuffer, SERIAL_TX_BUFFER_SIZE, this->txHead, this->txTail


namespace {
/*
 * These global variables are introduced to map the IRQ event to the specific HardwareSerial
 * instance. It is used to allow flexible variable naming of the HardwareSerial instances
 * while preserving low latency response delays in the interrupt handler.
 * This approach also tries to minimize any memory overhead.
 * 
 * @see `getHandlePtrFromId()`
 */
#ifdef USART1
UART_HandleTypeDef * uart1Handle = NULL;
#endif /* USART1 */
#ifdef USART2
UART_HandleTypeDef * uart2Handle = NULL;
#endif /* USART2 */
#ifdef USART3
UART_HandleTypeDef * uart3Handle = NULL;
#endif /* USART3 */
#if defined(UART4) || defined(USART4)
UART_HandleTypeDef * uart4Handle = NULL;
#endif /* UART4 || USART4 */
#if defined(UART5) || defined(USART5)
UART_HandleTypeDef * uart5Handle = NULL;
#endif /* UART5 || USART5 */
#ifdef USART6
UART_HandleTypeDef * uart6Handle = NULL;
#endif /* USART6 */
#if defined(UART7) || defined(USART7)
UART_HandleTypeDef * uart7Handle = NULL;
#endif /* UART7 || USART7 */
#if defined(UART8) || defined(USART8)
UART_HandleTypeDef * uart8Handle = NULL;
#endif /* UART8 || USART8 */
#ifdef USART9
UART_HandleTypeDef * uart9Handle = NULL;
#endif /* USART9 */
#ifdef USART10
UART_HandleTypeDef * uart10Handle = NULL;
#endif /* USART10 */
#ifdef LPUART1
UART_HandleTypeDef * lpuart1Handle = NULL;
#endif /* LPUART1 */


/**
 * Finds the base object pointer from a class member variable pointer. This acts like the
 * Linux kernel container_of() for a C++ class/struct.
 * 
 * @param[in,out] ptr - pointer to the class member variable
 * @param[in] member - class member variable pointer
 * @return base object pointer
 */
template <typename T, typename U>
inline T * getObjFromMemberPtr(void * ptr, U T::* const member) {
#define _RC(x) reinterpret_cast<x *>
	return _RC(T)(_RC(uint8_t)(ptr) - ptrdiff_t(_RC(uint8_t)(&(_RC(T)(NULL)->*member)) - _RC(uint8_t)(NULL)));
#undef _RC
}


/**
 * Returns the pointer to the UART handle which corresponds to the passed UART instance.
 * 
 * @param[in,out] hUart - UART instance as defined by STM32 HAL API
 * @return pointer to UART handler pointer
 * @remarks USART1 has the same value as USART1_BASE, but a different type. The same applies for the other values.
 * @remarks There is either the UART or the USART variant of one number. In fact, e.g. UART4 and USART4 are both
 * always defined as APB1PERIPH_BASE + 0x4C00.
 */
UART_HandleTypeDef ** getHandlePtrFromId(USART_TypeDef * hUart) {
	UART_HandleTypeDef ** res = NULL;
	switch (reinterpret_cast<uintptr_t>(hUart)) {
#ifdef USART1
	case USART1_BASE: res = &uart1Handle; break;
#endif /* USART1 */
#ifdef USART2
	case USART2_BASE: res = &uart2Handle; break;
#endif /* USART2 */
#ifdef USART3
	case USART3_BASE: res = &uart3Handle; break;
#endif /* USART3 */
#ifdef UART4
	case UART4_BASE: res = &uart4Handle; break;
#endif /* UART4 */
#ifdef USART4
	case USART4_BASE: res = &uart4Handle; break;
#endif /* USART4 */
#ifdef UART5
	case UART5_BASE: res = &uart5Handle; break;
#endif /* UART5 */
#ifdef USART5
	case USART5_BASE: res = &uart5Handle; break;
#endif /* USART5 */
#ifdef USART6
	case USART6_BASE: res = &uart6Handle; break;
#endif /* USART6 */
#ifdef UART7
	case UART7_BASE: res = &uart7Handle; break;
#endif /* UART7 */
#ifdef USART7
	case USART7_BASE: res = &uart7Handle; break;
#endif /* USART7 */
#ifdef UART8
	case UART8_BASE: res = &uart8Handle; break;
#endif /* UART8 */
#ifdef USART8
	case USART8_BASE: res = &uart8Handle; break;
#endif /* USART8 */
#ifdef UART9
	case UART9_BASE: res = &uart9Handle; break;
#endif /* UART9 */
#ifdef UART10
	case UART10_BASE: res = &uart10Handle; break;
#endif /* UART10 */
#ifdef LPUART1
	case LPUART1_BASE: res = &lpuart1Handle; break;
#endif /* LPUART1 */
	default: break;
	}
	return res;
}
} /* namespace anonymous */


/* UART/USART IRQ handlers */
extern "C" {
/* USARTs */
#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for USARTx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForUSART##x(void) { \
		if (uart##x##Handle != NULL) HAL_UART_IRQHandler(uart##x##Handle); \
	}

#ifdef USART1
DEF_IRQ_HANDLER(1)
#endif /* USART1 */

#ifdef USART2
DEF_IRQ_HANDLER(2)
#endif /* USART2 */

#ifdef USART3
DEF_IRQ_HANDLER(3)
#endif /* USART3 */

#ifdef USART4
DEF_IRQ_HANDLER(4)
#endif /* USART4 */

#ifdef USART5
DEF_IRQ_HANDLER(5)
#endif /* USART5 */

#ifdef USART6
DEF_IRQ_HANDLER(6)
#endif /* USART6 */

#ifdef USART7
DEF_IRQ_HANDLER(7)
#endif /* USART7 */

#ifdef USART8
DEF_IRQ_HANDLER(8)
#endif /* USART8 */

#ifdef USART10
DEF_IRQ_HANDLER(10)
#endif /* USART10 */


/* UARTs */
#undef DEF_IRQ_HANDLER
#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for UARTx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForUART##x(void) { \
		if (uart##x##Handle != NULL) HAL_UART_IRQHandler(uart##x##Handle); \
	}

#ifdef UART4
DEF_IRQ_HANDLER(4)
#endif /* UART4 */

#ifdef UART5
DEF_IRQ_HANDLER(5)
#endif /* UART5 */

#ifdef UART7
DEF_IRQ_HANDLER(7)
#endif /* UART7 */

#ifdef UART8
DEF_IRQ_HANDLER(8)
#endif /* UART8 */

#ifdef UART9
DEF_IRQ_HANDLER(9)
#endif /* UART9 */

#ifdef UART10
DEF_IRQ_HANDLER(10)
#endif /* UART10 */


/* LPUARTs */
#undef DEF_IRQ_HANDLER
#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for LPUARTx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForLPUART##x(void) { \
		if (lpuart##x##Handle != NULL) HAL_UART_IRQHandler(lpuart##x##Handle); \
	}

#ifdef LPUART1
DEF_IRQ_HANDLER(1)
#endif /* LPUART1 */


/**
 * Overwrites the STM32 HAL API handler for UART reception complete events.
 * This maps the UART instance to a call within the associated HardwareSerial instance.
 * 
 * @param[in,out] hUart - pointer to UART handle
 * @see HardwareSerial::rxCompleteHandler()
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * hUart) {
	HardwareSerial * obj = getObjFromMemberPtr(hUart, &HardwareSerial::handle);
	if (obj == NULL) return;
	obj->rxCompleteHandler();
}


/**
 * Overwrites the STM32 HAL API handler for UART transmission complete events.
 * This maps the UART instance to a call within the associated HardwareSerial instance and
 * continues transmission upon positive result from the HardwareSerial handler.
 * 
 * @param[in,out] hUart - pointer to UART handle
 * @see HardwareSerial::txCompleteHandler()
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * hUart) {
	HardwareSerial * obj = getObjFromMemberPtr(hUart, &HardwareSerial::handle);
	if (obj == NULL) return;
	obj->txCompleteHandler();
}


/**
 * Overwrites the STM32 HAL API handler for UART error events.
 * Error events are silently ignored and the UART is reset to continue data reception.
 * 
 * @param[in,out] hUart - pointer to UART handle
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef * hUart) {
	/* clear PE (parity error) flag */
	__HAL_UART_CLEAR_PEFLAG(hUart);
	/* clear FE (framing error) flag */
	__HAL_UART_CLEAR_FEFLAG(hUart);
	/* clear NE (noise error) flag */
	__HAL_UART_CLEAR_NEFLAG(hUart);
	/* clear ORE (overrun error) flag */
	__HAL_UART_CLEAR_OREFLAG(hUart);
	/* resume data reception */
	HardwareSerial * obj = getObjFromMemberPtr(hUart, &HardwareSerial::handle);
	if (obj == NULL) return;
	HAL_UART_Receive_IT(hUart, obj->recv, 1);
}


#ifdef USART_ISR_WUF
/**
 * Overwrites the STM32 HAL API handler for UART wakeup events.
 * Data reception is resumed.
 * 
 * @param[in,out] hUart - pointer to UART handle
 */
void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef * hUart) {
	HAL_UART_RxCpltCallback(hUart);
}
#endif /* USART_ISR_WUF */
} /* extern "C" */


/**
 * Constructor.
 * 
 * @param[in,out] instance - UART instance as defined by STM32 HAL API
 * @param[in] irqNum - associated IRQ for the UART as defined by STM32 HAL API
 * @param[in] rxPin - PinName of the RX pin
 * @param[in] txPin - PinName of the TX pin
 * @param[in] rxAltFn - alternate function number of the RX pin (see `pinMode()`)
 * @param[in] txAltFn - alternate function number of the TX pin (see `pinMode()`)
 */
HardwareSerial::HardwareSerial(USART_TypeDef * instance, const IRQn_Type irqNum, const PinName rxPin, const PinName txPin, const uint8_t rxAltFn, const uint8_t txAltFn):
	irq(irqNum),
	pins{uint8_t(rxPin), uint8_t(txPin)},
	afns(uint8_t((rxAltFn << 4) | txAltFn)),
	txNextTail(0)
{
	memset(this->handle, 0, sizeof(*(this->handle)));
	this->handle->Instance = instance;
	UART_HandleTypeDef ** handlePtr = getHandlePtrFromId(instance);
	if (handlePtr == NULL || (*handlePtr != NULL && (*handlePtr)->Instance != NULL)) {
		systemErrorHandler();
		return;
	}
	*handlePtr = this->handle;
	_FIFOX_INIT(RX_QUEUE);
	_FIFOX_INIT(TX_QUEUE);
}


/**
 * Destructor.
 */
HardwareSerial::~HardwareSerial() {
	UART_HandleTypeDef ** handlePtr = getHandlePtrFromId(this->handle->Instance);
	if (handlePtr == NULL) return;
	*handlePtr = NULL;
}


/**
 * Starts the serial interface with the given settings.
 * 
 * @param[in] baudrate - physical baud rate (e.g. 9600)
 * @param[in] mode - data framing mode (e.g. SERIAL_8N1)
 * @param[in] initFn - initialization function for the UART (defaults to that of STM32 HAL)
 * @remarks Baud rates up to half of the supported maximum are possible.
 * Higher rates require UART_OVERSAMPLING_8 and special care in the hardware design.
 * @remarks The function signature differs to that of legacy Arduino due to the additional initFn parameter.
 * @remarks The initFn parameter can be used to provide a wrapper around the STM32 HAL function for customized
 * driver initialization. This is needed, for example, to switch RX and TX.
 */
void HardwareSerial::begin(const unsigned long baudrate, const uint8_t mode, HAL_StatusTypeDef (& initFn)(UART_HandleTypeDef * hUart)) {
	uint32_t databits = 0;
	uint32_t stopbits = 0;
	uint32_t parity = 0;
	/* data bits */
	switch (mode & 0x07) {
	case 0x02: databits = 6; break;
	case 0x04: databits = 7; break;
	case 0x06: databits = 8; break;
	default:   databits = 0; break;
	}
	/* parity */
	if ((mode & 0x30) == 0x30) {
		parity = UART_PARITY_ODD;
		databits++;
	} else if ((mode & 0x20) == 0x20) {
		parity = UART_PARITY_EVEN;
		databits++;
	} else {
		parity = UART_PARITY_NONE;
	}
	/* stop bits */
	if ((mode & 0x08) == 0x08) {
		stopbits = UART_STOPBITS_2;
	} else {
		stopbits = UART_STOPBITS_1;
	}
	switch (databits) {
#ifdef UART_WORDLENGTH_7B
	case 7:
		databits = UART_WORDLENGTH_7B;
		break;
#endif /* UART_WORDLENGTH_7B */
	case 8:
		databits = UART_WORDLENGTH_8B;
		break;
	case 9:
		databits = UART_WORDLENGTH_9B;
		break;
	case 0:
	default:
		systemErrorHandler();
		break;
	}
	/* enable UART clock */
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef USART1
	case USART1_BASE:
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
		__HAL_RCC_USART1_CLK_ENABLE();
		break;
#endif /* USART1 */
#ifdef USART2
	case USART2_BASE:
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();
		__HAL_RCC_USART2_CLK_ENABLE();
		break;
#endif /* USART2 */
#ifdef USART3
	case USART3_BASE:
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();
		__HAL_RCC_USART3_CLK_ENABLE();
		break;
#endif /* USART3 */
#ifdef UART4
	case UART4_BASE:
		__HAL_RCC_UART4_FORCE_RESET();
		__HAL_RCC_UART4_RELEASE_RESET();
		__HAL_RCC_UART4_CLK_ENABLE();
		break;
#endif /* UART4 */
#ifdef USART4
	case USART4_BASE:
		__HAL_RCC_USART4_FORCE_RESET();
		__HAL_RCC_USART4_RELEASE_RESET();
		__HAL_RCC_USART4_CLK_ENABLE();
		break;
#endif /* USART4 */
#ifdef UART5
	case UART5_BASE:
		__HAL_RCC_UART5_FORCE_RESET();
		__HAL_RCC_UART5_RELEASE_RESET();
		__HAL_RCC_UART5_CLK_ENABLE();
		break;
#endif /* UART5 */
#ifdef USART5
	case USART5_BASE:
		__HAL_RCC_USART5_FORCE_RESET();
		__HAL_RCC_USART5_RELEASE_RESET();
		__HAL_RCC_USART5_CLK_ENABLE();
		break;
#endif /* USART5 */
#ifdef USART6
	case USART6_BASE:
		__HAL_RCC_USART6_FORCE_RESET();
		__HAL_RCC_USART6_RELEASE_RESET();
		__HAL_RCC_USART6_CLK_ENABLE();
		break;
#endif /* USART6 */
#ifdef UART7
	case UART7_BASE:
		__HAL_RCC_UART7_FORCE_RESET();
		__HAL_RCC_UART7_RELEASE_RESET();
		__HAL_RCC_UART7_CLK_ENABLE();
		break;
#endif /* UART7 */
#ifdef USART7
	case USART7_BASE:
		__HAL_RCC_USART7_FORCE_RESET();
		__HAL_RCC_USART7_RELEASE_RESET();
		__HAL_RCC_USART7_CLK_ENABLE();
		break;
#endif /* USART7 */
#ifdef UART8
	case UART8_BASE:
		__HAL_RCC_UART8_FORCE_RESET();
		__HAL_RCC_UART8_RELEASE_RESET();
		__HAL_RCC_UART8_CLK_ENABLE();
		break;
#endif /* UART8 */
#ifdef USART8
	case USART8_BASE:
		__HAL_RCC_USART8_FORCE_RESET();
		__HAL_RCC_USART8_RELEASE_RESET();
		__HAL_RCC_USART8_CLK_ENABLE();
		break;
#endif /* USART8 */
#ifdef UART9
	case UART9_BASE:
		__HAL_RCC_UART9_FORCE_RESET();
		__HAL_RCC_UART9_RELEASE_RESET();
		__HAL_RCC_UART9_CLK_ENABLE();
		break;
#endif /* UART9 */
#ifdef UART10
	case UART10_BASE:
		__HAL_RCC_UART10_FORCE_RESET();
		__HAL_RCC_UART10_RELEASE_RESET();
		__HAL_RCC_UART10_CLK_ENABLE();
		break;
#endif /* UART10 */
#ifdef LPUART1
	case LPUART1_BASE:
		__HAL_RCC_LPUART1_FORCE_RESET();
		__HAL_RCC_LPUART1_RELEASE_RESET();
		__HAL_RCC_LPUART1_CLK_ENABLE();
		break;
#endif /* LPUART1 */
	default:
		systemErrorHandler();
		break;
	}
	/* set pin mode and alternate function */
#ifdef STM32F1
	/* STM32F1 needs RX to be configured as INPUT */
	pinModeEx(this->pins[0], INPUT, this->afns >> 4);
#else /* not STM32F1 */
	pinModeEx(this->pins[0], ALTERNATE_FUNCTION, this->afns >> 4);
#endif /* not STM32F1 */
	pinModeEx(this->pins[1], ALTERNATE_FUNCTION, this->afns & 0xF);
	/* initialize */
	this->handle->Init.BaudRate = uint32_t(baudrate);
	this->handle->Init.WordLength = databits;
	this->handle->Init.StopBits = stopbits;
	this->handle->Init.Parity = parity;
	this->handle->Init.Mode = UART_MODE_TX_RX;
	this->handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	this->handle->Init.OverSampling = UART_OVERSAMPLING_16;
#ifdef UART_ONE_BIT_SAMPLE_DISABLE
	this->handle->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
#endif /* UART_ONE_BIT_SAMPLE_DISABLE */
#ifdef UART_ADVFEATURE_NO_INIT
	this->handle->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
#endif /* UART_ADVFEATURE_NO_INIT */
	if (initFn(this->handle) != HAL_OK) {
		systemErrorHandler();
	}
	/* must disable interrupt to prevent handle lock contention */
	HAL_NVIC_DisableIRQ(this->irq);
	__DMB(); __DSB(); __ISB(); /* data and instruction barrier */
	HAL_UART_Receive_IT(this->handle, this->recv, 1); /* receive single bytes for minimal latency */
	/* enable interrupt */
	HAL_NVIC_SetPriority(this->irq, UART_IRQ_PRIO, UART_IRQ_SUBPRIO);
	HAL_NVIC_EnableIRQ(this->irq);
}


/**
 * Stops the serial interface.
 */
void HardwareSerial::end() {
	this->flush();
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef USART1
	case USART1_BASE:
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
		__HAL_RCC_USART1_CLK_DISABLE();
		break;
#endif /* USART1 */
#ifdef USART2
	case USART2_BASE:
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();
		__HAL_RCC_USART2_CLK_DISABLE();
		break;
#endif /* USART2 */
#ifdef USART3
	case USART3_BASE:
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();
		__HAL_RCC_USART3_CLK_DISABLE();
		break;
#endif /* USART3 */
#ifdef UART4
	case UART4_BASE:
		__HAL_RCC_UART4_FORCE_RESET();
		__HAL_RCC_UART4_RELEASE_RESET();
		__HAL_RCC_UART4_CLK_DISABLE();
		break;
#endif /* UART4 */
#ifdef USART4
	case USART4_BASE:
		__HAL_RCC_USART4_FORCE_RESET();
		__HAL_RCC_USART4_RELEASE_RESET();
		__HAL_RCC_USART4_CLK_DISABLE();
		break;
#endif /* USART4 */
#ifdef UART5
	case UART5_BASE:
		__HAL_RCC_UART5_FORCE_RESET();
		__HAL_RCC_UART5_RELEASE_RESET();
		__HAL_RCC_UART5_CLK_DISABLE();
		break;
#endif /* UART5 */
#ifdef USART5
	case USART5_BASE:
		__HAL_RCC_USART5_FORCE_RESET();
		__HAL_RCC_USART5_RELEASE_RESET();
		__HAL_RCC_USART5_CLK_DISABLE();
		break;
#endif /* USART5 */
#ifdef USART6
	case USART6_BASE:
		__HAL_RCC_USART6_FORCE_RESET();
		__HAL_RCC_USART6_RELEASE_RESET();
		__HAL_RCC_USART6_CLK_DISABLE();
		break;
#endif /* USART6 */
#ifdef UART7
	case UART7_BASE:
		__HAL_RCC_UART7_FORCE_RESET();
		__HAL_RCC_UART7_RELEASE_RESET();
		__HAL_RCC_UART7_CLK_DISABLE();
		break;
#endif /* UART7 */
#ifdef USART7
	case USART7_BASE:
		__HAL_RCC_USART7_FORCE_RESET();
		__HAL_RCC_USART7_RELEASE_RESET();
		__HAL_RCC_USART7_CLK_DISABLE();
		break;
#endif /* USART7 */
#ifdef UART8
	case UART8_BASE:
		__HAL_RCC_UART8_FORCE_RESET();
		__HAL_RCC_UART8_RELEASE_RESET();
		__HAL_RCC_UART8_CLK_DISABLE();
		break;
#endif /* UART8 */
#ifdef USART8
	case USART8_BASE:
		__HAL_RCC_USART8_FORCE_RESET();
		__HAL_RCC_USART8_RELEASE_RESET();
		__HAL_RCC_USART8_CLK_DISABLE();
		break;
#endif /* USART8 */
#ifdef UART9
	case UART9_BASE:
		__HAL_RCC_UART9_FORCE_RESET();
		__HAL_RCC_UART9_RELEASE_RESET();
		__HAL_RCC_UART9_CLK_DISABLE();
		break;
#endif /* UART9 */
#ifdef UART10
	case UART10_BASE:
		__HAL_RCC_UART10_FORCE_RESET();
		__HAL_RCC_UART10_RELEASE_RESET();
		__HAL_RCC_UART10_CLK_DISABLE();
		break;
#endif /* UART10 */
#ifdef LPUART1
	case LPUART1_BASE:
		__HAL_RCC_LPUART1_FORCE_RESET();
		__HAL_RCC_LPUART1_RELEASE_RESET();
		__HAL_RCC_LPUART1_CLK_DISABLE();
		break;
#endif /* LPUART1 */
	default:
		systemErrorHandler();
		break;
	}
	HAL_UART_DeInit(this->handle);
	_FIFOX_CLEAR(RX_QUEUE);
}


/**
 * Returns the number of available bytes in the receive buffer.
 * 
 * @return number of bytes available for read
 */
int HardwareSerial::available(void) {
	return int(_FIFOX_SIZE(RX_QUEUE));
}


/**
 * Returns the number of bytes which can be written without buffer overflow.
 * 
 * @return number of bytes available for write
 */
int HardwareSerial::availableForWrite(void) {
	/* we are being pessimistic here in case of concurrent interrupts */
	const tx_buffer_index_t tail = this->txTail;
	return int(_FIFO_AVAILABLE(this->txBuffer, SERIAL_TX_BUFFER_SIZE, this->txHead, tail));
}


/**
 * Returns the first byte in the receive buffer without removing it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int HardwareSerial::peek(void) {
	return _FIFOX_PEEK(RX_QUEUE);
}


/**
 * Returns a byte from the receive buffer and removes it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int HardwareSerial::read(void) {
	return _FIFOX_POP(RX_QUEUE);
}


/**
 * Waits until all data in transmission queue is sent.
 */
void HardwareSerial::flush(void) {
	/* wait until the interrupt handler wrote out all data */
	while ( ! _FIFOX_EMPTY(TX_QUEUE) );
}


/**
 * Sends the given byte.
 * 
 * @param[in] val - byte value to send
 * @return number of bytes written to the transmission queue
 */
size_t HardwareSerial::write(const uint8_t val) {
	bool canWait = false;
	const bool interruptsEnabled = ((__get_PRIMASK() & 0x1) == 0);
	if ( interruptsEnabled ) {
		const uint32_t irqExecutionNumber = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
		if (irqExecutionNumber == 0 || NVIC_GetPriority(IRQn_Type(irqExecutionNumber - 16)) > UART_IRQ_PRIO) {
			/* UART interrupt is enabled and we were not called from an interrupt with higher priority */
			canWait = true;
		}
	}
	if ( canWait ) {
		/* wait until space is available and add to queue */
		_FIFOX_WPUSH(TX_QUEUE, val);
	} else {
		/* add to queue or fail if no space is available, because there is currently no chance to get data out */
		if ( ! _FIFOX_PUSH(TX_QUEUE, val) ) return 0;
	}
	if ( this->txBusy() ) return 1;
	/* must disable interrupt to prevent handle lock contention */
	HAL_NVIC_DisableIRQ(this->irq);
	__DMB(); __DSB(); __ISB(); /* data and instruction barrier */
	const size_t blockSize = _FIFOX_BSIZE(TX_QUEUE);
	/* ensure that we have enough space for user requests before the IRQ returns by using double buffering */
	const tx_buffer_index_t trimmedBlockSize = tx_buffer_index_t(max(1, min(blockSize, (SERIAL_TX_BUFFER_SIZE / 2))));
	this->txNextTail = _FIFOX_INDEX(TX_QUEUE, trimmedBlockSize);
	HAL_UART_Transmit_IT(this->handle, this->txBuffer + this->txTail, trimmedBlockSize);
	/* enable interrupt */
	HAL_NVIC_SetPriority(this->irq, UART_IRQ_PRIO, UART_IRQ_SUBPRIO);
	HAL_NVIC_EnableIRQ(this->irq);
	return 1;
}


/**
 * Returns whether the UART is in BUSY reception state or not.
 * 
 * @return true if busy, else false
 */
bool HardwareSerial::rxBusy() {
	return ((HAL_UART_GetState(this->handle) & HAL_UART_STATE_BUSY_RX) == HAL_UART_STATE_BUSY_RX);
}


/**
 * Returns whether the UART is in BUSY transmission state or not.
 * 
 * @return true if busy, else false
 */
bool HardwareSerial::txBusy() {
	return ((HAL_UART_GetState(this->handle) & HAL_UART_STATE_BUSY_TX) == HAL_UART_STATE_BUSY_TX);
}


/**
 * Reception complete interrupt handler.
 */
void HardwareSerial::rxCompleteHandler() {
	if ( this->rxBusy() ) return; /* transaction ongoing */
	/* no parity error */
	_FIFOX_PUSH(RX_QUEUE, *(this->recv));
	/* receive next byte */
	HAL_UART_Receive_IT(this->handle, this->recv, 1);
}


/**
 * Transmission complete interrupt handler.
 */
void HardwareSerial::txCompleteHandler() {
	this->txTail = this->txNextTail;
	const size_t blockSize = _FIFOX_BSIZE(TX_QUEUE);
	if (blockSize <= 0) return;
	/* ensure that we have enough space for user requests before the IRQ returns by using double buffering */
	const tx_buffer_index_t trimmedBlockSize = tx_buffer_index_t(max(1, min(blockSize, (SERIAL_TX_BUFFER_SIZE / 2))));
	this->txNextTail = _FIFOX_INDEX(TX_QUEUE, trimmedBlockSize);
	HAL_UART_Transmit_IT(this->handle, this->txBuffer + this->txTail, trimmedBlockSize);
}


#endif /* IS_UART_MODE */
