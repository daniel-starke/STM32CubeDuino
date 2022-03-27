/**
 * @file SPI.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-02-13
 * @version 2022-03-27
 */
#include "Arduino.h"
#include "SPI.h"
#include "wiring_private.h"


#if !defined(STM32CUBEDUINO_DISABLE_SPI) && defined(IS_SPI_MODE) /* STM32 HAL SPI header was included */


/**
 * Helper function to check whether the given parameters have valid
 * values.
 * 
 * @param[in] bitOrder - SPI bit order
 * @param[in] dataMode - SPI data mode
 * @return true if valid, else false
 */
static bool isValidSpiConfiguration(const uint32_t bitOrder, const uint32_t dataMode) {
	switch (bitOrder) {
	case LSBFIRST:
	case MSBFIRST:
		break;
	default:
		return false;
	}
	switch (dataMode) {
	case SPI_MODE0:
	case SPI_MODE1:
	case SPI_MODE2:
	case SPI_MODE3:
		break;
	default:
		return false;
	}
	return true;
}


/**
 * Constructor.
 * 
 * @param[in,out] instance - SPI instance as defined by STM32 HAL API
 * @param[in] sclkPin - PinName of the SCLK pin
 * @param[in] mosiPin - PinName of the MOSI pin
 * @param[in] misoPin - PinName of the MISO pin
 * @param[in] sclkAltFn - alternate function number of the SCLK pin (see `pinMode()`)
 * @param[in] mosiAltFn - alternate function number of the MOSI pin (see `pinMode()`)
 * @param[in] misoAltFn - alternate function number of the MISO pin (see `pinMode()`)
 */
SPIClass::SPIClass(SPI_TypeDef * instance, const PinName sclkPin, const PinName mosiPin, const PinName misoPin, const uint8_t sclkAltFn, const uint8_t mosiAltFn, const uint8_t misoAltFn):
	pinSclk(sclkPin),
	pinMosi(mosiPin),
	pinMiso(misoPin),
	afnSclk(sclkAltFn),
	afnMosi(mosiAltFn),
	afnMiso(misoAltFn),
	initialized(false),
	lastInitFn(NULL)
{
	memset(this->handle, 0, sizeof(*(this->handle)));
	this->handle->Instance = instance;
	this->handle->Init.Mode = SPI_MODE_MASTER;
	this->handle->Init.Direction = SPI_DIRECTION_2LINES;
	this->handle->Init.DataSize = SPI_DATASIZE_8BIT;
	this->handle->Init.NSS = SPI_NSS_SOFT;
	this->handle->Init.TIMode = SPI_TIMODE_DISABLE;
#ifdef SPI_CRCCALCULATION_DISABLE
	this->handle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	this->handle->Init.CRCPolynomial = 7;
#ifdef SPI_CRC_LENGTH_DATASIZE
	this->handle->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
#endif /* SPI_CRC_LENGTH_DATASIZE */
#endif /* SPI_CRCCALCULATION_DISABLE */
#ifdef SPI_NSS_PULSE_DISABLE
	this->handle->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
#endif /* SPI_NSS_PULSE_DISABLE */
#ifdef SPI_MASTER_KEEP_IO_STATE_ENABLE
	/* Set to avoid glitches. */
	this->handle->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
#endif
}


/**
 * Destructor.
 */
SPIClass::~SPIClass() {}


/**
 * Starts the SPI interface with the given settings.
 * 
 * @param[in] initFn - initialization function for the SPI (defaults to that of STM32 HAL)
 */
void SPIClass::begin(HAL_StatusTypeDef (& initFn)(SPI_HandleTypeDef * hSpi)) {
	/* enable SPI clock */
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef SPI1
	case SPI1_BASE:
		__HAL_RCC_SPI1_CLK_ENABLE();
		__HAL_RCC_SPI1_FORCE_RESET();
		__HAL_RCC_SPI1_RELEASE_RESET();
		break;
#endif /* SPI1 */
#ifdef SPI2
	case SPI2_BASE:
		__HAL_RCC_SPI2_CLK_ENABLE();
		__HAL_RCC_SPI2_FORCE_RESET();
		__HAL_RCC_SPI2_RELEASE_RESET();
		break;
#endif /* SPI2 */
#ifdef SPI3
	case SPI3_BASE:
		__HAL_RCC_SPI3_CLK_ENABLE();
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		break;
#endif /* SPI3 */
#ifdef SPI4
	case SPI4_BASE:
		__HAL_RCC_SPI4_CLK_ENABLE();
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();
		break;
#endif /* SPI4 */
#ifdef SPI5
	case SPI5_BASE:
		__HAL_RCC_SPI5_CLK_ENABLE();
		__HAL_RCC_SPI5_FORCE_RESET();
		__HAL_RCC_SPI5_RELEASE_RESET();
		break;
#endif /* SPI5 */
#ifdef SPI6
	case SPI6_BASE:
		__HAL_RCC_SPI6_CLK_ENABLE();
		__HAL_RCC_SPI6_FORCE_RESET();
		__HAL_RCC_SPI6_RELEASE_RESET();
		break;
#endif /* SPI6 */
	default:
		systemErrorHandler();
		break;
	}
	/* set pin mode and alternate function */
	pinModeEx(this->pinSclk, ALTERNATE_FUNCTION, this->afnSclk);
	pinModeEx(this->pinMosi, ALTERNATE_FUNCTION, this->afnMosi);
	pinModeEx(this->pinMiso, ALTERNATE_FUNCTION, this->afnMiso);
	/* initialize */
	this->lastInitFn = initFn;
	this->initialized = false;
}


/**
 * Stops the SPI interface.
 */
void SPIClass::end() {
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef SPI1
	case SPI1_BASE:
		__HAL_RCC_SPI1_FORCE_RESET();
		__HAL_RCC_SPI1_RELEASE_RESET();
		__HAL_RCC_SPI1_CLK_DISABLE();
		break;
#endif /* SPI1 */
#ifdef SPI2
	case SPI2_BASE:
		__HAL_RCC_SPI2_FORCE_RESET();
		__HAL_RCC_SPI2_RELEASE_RESET();
		__HAL_RCC_SPI2_CLK_DISABLE();
		break;
#endif /* SPI2 */
#ifdef SPI3
	case SPI3_BASE:
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		__HAL_RCC_SPI3_CLK_DISABLE();
		break;
#endif /* SPI3 */
#ifdef SPI4
	case SPI4_BASE:
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();
		__HAL_RCC_SPI4_CLK_DISABLE();
		break;
#endif /* SPI4 */
#ifdef SPI5
	case SPI5_BASE:
		__HAL_RCC_SPI5_FORCE_RESET();
		__HAL_RCC_SPI5_RELEASE_RESET();
		__HAL_RCC_SPI5_CLK_DISABLE();
		break;
#endif /* SPI5 */
#ifdef SPI6
	case SPI6_BASE:
		__HAL_RCC_SPI6_FORCE_RESET();
		__HAL_RCC_SPI6_RELEASE_RESET();
		__HAL_RCC_SPI6_CLK_DISABLE();
		break;
#endif /* SPI6 */
	default:
		systemErrorHandler();
		break;
	}
	HAL_SPI_DeInit(this->handle);
	this->initialized = false;
}


/**
 * Configures the SPI interface to begin data transfers. This should
 * be called before calling any transfer function and after `begin()`
 * has been called.
 * 
 * @param[in] settings - SPI settings
 */
void SPIClass::beginTransaction(const SPISettings settings) {
	uint32_t prescaler = SPI_BAUDRATEPRESCALER_256; /* Default to lowest speed possible. */
	
	if ( ! isValidSpiConfiguration(settings.mBitOrder, settings.mDataMode) ) return;
	
	const uint32_t clock = getSpiClockFrequency(this->handle->Instance);
	this->config = settings;
	
	if (this->config.mClock >= (clock / SPI_CLOCK_DIV2)) {
		prescaler = SPI_BAUDRATEPRESCALER_2;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV4)) {
		prescaler = SPI_BAUDRATEPRESCALER_4;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV8)) {
		prescaler = SPI_BAUDRATEPRESCALER_8;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV16)) {
		prescaler = SPI_BAUDRATEPRESCALER_16;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV32)) {
		prescaler = SPI_BAUDRATEPRESCALER_32;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV64)) {
		prescaler = SPI_BAUDRATEPRESCALER_64;
	} else if (this->config.mClock >= (clock / SPI_CLOCK_DIV128)) {
		prescaler = SPI_BAUDRATEPRESCALER_128;
	}
	
	this->initialize(prescaler);
}


/**
 * Leaves the data transmission mode. Call `beginTransaction()` again
 * before re-entering data transmissions.
 */
void SPIClass::endTransaction() {
}


/**
 * Performs a full-duplex data transfer of 1 byte.
 * 
 * @param[in] data - data to send
 * @return data received
 */
uint8_t SPIClass::transfer(uint8_t data) {
	this->transfer(&data, 1);
	if (HAL_SPI_GetError(this->handle) != HAL_SPI_ERROR_NONE) return 0;
	return data;
}


/**
 * Performs a full-duplex data transfer of 2 bytes.
 * 
 * @param[in] data - data to send
 * @return data received
 */
uint16_t SPIClass::transfer16(uint16_t data) {
	this->transfer(&data, 2);
	if (HAL_SPI_GetError(this->handle) != HAL_SPI_ERROR_NONE) return 0;
	return data;
}


/**
 * Performs a full-duplex data transfer with the given buffer.
 * All data received is stored in the same buffer.
 * 
 * @param[in,out] buffer - data to send; filled with the data received
 * @param[in] length - size of the data in the buffer
 */
void SPIClass::transfer(void * buffer, const size_t length) {
	if ( ! this->initialized ) {
		this->initialize(this->handle->Init.BaudRatePrescaler);
	}
	HAL_SPI_TransmitReceive(this->handle, reinterpret_cast<uint8_t *>(buffer), reinterpret_cast<uint8_t *>(buffer), uint16_t(length), SPI_TRANSFER_TIMEOUT);
}


/**
 * Changes the used bit order.
 * 
 * @param[in] bitOrder - new bit order (e.g. `MSBFIRST`)
 */
void SPIClass::setBitOrder(const uint8_t bitOrder) {
	if ( ! isValidSpiConfiguration(bitOrder, this->config.mDataMode) ) return;
	this->config.mBitOrder = bitOrder;
	if ( this->initialized ) {
		this->initialize(this->handle->Init.BaudRatePrescaler);
	}
}


/**
 * Changes the used data mode.
 * 
 * @param[in] dataMode - new data mode (e.g. `SPI_MODE0`)
 */
void SPIClass::setDataMode(const uint8_t dataMode) {
	if ( ! isValidSpiConfiguration(this->config.mBitOrder, dataMode) ) return;
	this->config.mDataMode = dataMode;
	if ( this->initialized ) {
		this->initialize(this->handle->Init.BaudRatePrescaler);
	}
}


/**
 * Changes the used clock divider.
 * 
 * @param[in] clockDivider - new clock divider (e.g. `SPI_CLOCK_DIV2`)
 */
void SPIClass::setClockDivider(const uint8_t clockDivider) {
	uint32_t prescaler = 0;
	switch (clockDivider) {
	case SPI_CLOCK_DIV2: prescaler = SPI_BAUDRATEPRESCALER_2; break;
	case SPI_CLOCK_DIV4: prescaler = SPI_BAUDRATEPRESCALER_4; break;
	case SPI_CLOCK_DIV8: prescaler = SPI_BAUDRATEPRESCALER_8; break;
	case SPI_CLOCK_DIV16: prescaler = SPI_BAUDRATEPRESCALER_16; break;
	case SPI_CLOCK_DIV32: prescaler = SPI_BAUDRATEPRESCALER_32; break;
	case SPI_CLOCK_DIV64: prescaler = SPI_BAUDRATEPRESCALER_64; break;
	case SPI_CLOCK_DIV128: prescaler = SPI_BAUDRATEPRESCALER_128; break;
	default: return; /* invalid value */
	}
	const uint32_t clock = getSpiClockFrequency(this->handle->Instance);
	this->config.mClock = clock / clockDivider;
	if ( this->initialized ) {
		if ( ! isValidSpiConfiguration(this->config.mBitOrder, this->config.mDataMode) ) return;
		this->initialize(prescaler);
	}
}


/**
 * Internal function to initialize the SPI interface with the
 * current settings and passed prescaler.
 * 
 * @param[in] prescaler - prescaler to use (e.g. `SPI_BAUDRATEPRESCALER_2`)
 */
void SPIClass::initialize(const uint32_t prescaler) {
	this->initialized = false;
	
	/* According to the STM32 datasheet for SPI peripheral SCLK needs to be
	 * pulled up or down depending no the polarity used.
	 */
	if (this->config.mDataMode == SPI_MODE2 || this->config.mDataMode == SPI_MODE3) {
		this->handle->Init.CLKPolarity = SPI_POLARITY_HIGH;
		LL_GPIO_SetPinPull(getInternalPort(digitalPinToPort(this->pinSclk)), getInternalPin(this->pinSclk), GPIO_PULLUP);
	} else {
		this->handle->Init.CLKPolarity = SPI_POLARITY_LOW;
		LL_GPIO_SetPinPull(getInternalPort(digitalPinToPort(this->pinSclk)), getInternalPin(this->pinSclk), GPIO_PULLDOWN);
	}
	
	if (this->config.mDataMode == SPI_MODE1 || this->config.mDataMode == SPI_MODE3) {
		this->handle->Init.CLKPhase = SPI_PHASE_2EDGE;
	} else {
		this->handle->Init.CLKPhase = SPI_PHASE_1EDGE;
	}
	
	this->handle->Init.BaudRatePrescaler = prescaler;
	this->handle->Init.FirstBit = (this->config.mBitOrder == LSBFIRST) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
	
	if (this->lastInitFn != NULL) {
		if (this->lastInitFn(this->handle) != HAL_OK) {
			systemErrorHandler();
		} else {
			__HAL_SPI_ENABLE(this->handle);
			this->initialized = true;
		}
	}
}


#endif /* IS_SPI_MODE */
