/**
 * @file SPI.h
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-02-13
 * @version 2022-03-11
 */
#ifndef __SPI_H__
#define __SPI_H__

#include "Stream.h"


#if !defined(STM32CUBEDUINO_DISABLE_SPI) && defined(IS_SPI_MODE) /* STM32 HAL SPI header was included */


/**
 * `SPI_HAS_TRANSACTION` means SPI has `beginTransaction()`, `endTransaction()`,
 * `usingInterrupt()` and `SPISetting(clock, bitOrder, dataMode)`.
 */
#define SPI_HAS_TRANSACTION 1


/** `SPI_HAS_NOTUSINGINTERRUPT` means that SPI `notUsingInterrupt()`. */
#define SPI_HAS_NOTUSINGINTERRUPT 1


/** May be defined with `HAL_MAX_DELAY` to wait indefinitely. This macro is STM32 specific. */
#ifndef SPI_TRANSFER_TIMEOUT
#define SPI_TRANSFER_TIMEOUT 1000
#endif


#define SPI_CLOCK_DIV2 2
#define SPI_CLOCK_DIV4 4
#define SPI_CLOCK_DIV8 8
#define SPI_CLOCK_DIV16 16
#define SPI_CLOCK_DIV32 32
#define SPI_CLOCK_DIV64 64
#define SPI_CLOCK_DIV128 128


#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3


class SPISettings {
private:
	friend class SPIClass;
	uint32_t mClock;
	uint32_t mBitOrder;
	uint32_t mDataMode;
public:
	/**
	 * Constructor.
	 */
	SPISettings():
		mClock(4000000U),
		mBitOrder(MSBFIRST),
		mDataMode(SPI_MODE0)
	{}
	
	/**
	 * Constructor.
	 * 
	 * @param[in] clock - desired clock speed (e.g. 4000000)
	 * @param[in] bitOrder - bit order (e.g. `MSBFIRST`)
	 * @param[in] dataMode - data transmission mode (e.g. `SPI_MODE0`)
	 */
	SPISettings(const uint32_t clock, const uint8_t bitOrder, const uint8_t dataMode):
		mClock(clock),
		mBitOrder(bitOrder),
		mDataMode(dataMode)
	{}
};


class SPIClass {
protected:
	SPI_HandleTypeDef handle[1];
	uint8_t pinSclk;
	uint8_t pinMosi;
	uint8_t pinMiso;
	uint8_t afnSclk;
	uint8_t afnMosi;
	uint8_t afnMiso;
	SPISettings config;
	bool initialized;
	HAL_StatusTypeDef (* lastInitFn)(SPI_HandleTypeDef * hSpi);
public:
	SPIClass(SPI_TypeDef * instance, const PinName sclkPin, const PinName mosiPin, const PinName misoPin, const uint8_t sclkAltFn, const uint8_t mosiAltFn, const uint8_t misoAltFn);
	virtual ~SPIClass();
	
	inline void begin() { this->begin(HAL_SPI_Init); }
	void begin(HAL_StatusTypeDef (& initFn)(SPI_HandleTypeDef * hSpi)); /* STM32 specific */
	void end();
	
	void beginTransaction(const SPISettings settings);
	void endTransaction();
	
	uint8_t transfer(uint8_t data);
	uint16_t transfer16(uint16_t data);
	void transfer(void * buffer, const size_t length);
	
	void setBitOrder(const uint8_t bitOrder);
	void setDataMode(const uint8_t dataMode);
	void setClockDivider(const uint8_t clockDivider);
	
	/* The following methods are only included for compatibility reasons. */
	inline void usingInterrupt(const uint8_t /* interruptNumber */) const {}
	inline void notUsingInterrupt(const uint8_t /* interruptNumber */) const {}
	/* These undocumented functions should not be used. */
	inline void attachInterrupt() const {}
	inline void detachInterrupt() const {}
protected:
	void initialize(const uint32_t prescaler);
};


/* available if defined in board.cpp or by the user */
extern SPIClass SPI;


#endif /* IS_SPI_MODE */
#endif /* __SPI_H__ */
