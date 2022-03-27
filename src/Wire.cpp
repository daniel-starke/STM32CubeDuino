/**
 * @file Wire.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-01-18
 * @version 2022-03-22
 * 
 * @remarks I2C specification: https://www.nxp.com/docs/en/user-guide/UM10204.pdf
 * @see https://www.ti.com/lit/an/slva704/slva704.pdf
 */
#include "Arduino.h"
#include "Wire.h"
#include "scdinternal/macro.h"
#include "wiring_irq.h"
#include "wiring_private.h"


#if !defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE) /* STM32 HAL I2C header was included */


#ifndef I2C_IRQ_PRIO
#error Please define I2C_IRQ_PRIO in board.hpp.
#endif
#ifndef I2C_IRQ_SUBPRIO
#error Please define I2C_IRQ_SUBPRIO in board.hpp.
#endif


/** Local definition of the RX circular buffer queue within a given object. */
#define RX_QUEUE(o) (o)->rxBuffer, TWOWIRE_RX_BUFFER_SIZE, (o)->rxHead, (o)->rxTail


#define I2C_PRESC_MAX     16U
#define I2C_SCLDELAY_MAX  16U
#define I2C_SDADELAY_MAX  16U
#define I2C_SCLHIGH_MAX  256U
#define I2C_SCLLOW_MAX   256U


#ifndef I2C_USE_ANALOG_FILTER
#define I2C_USE_ANALOG_FILTER 1
#endif

/* Available in the datasheet of the MCU. */
#ifndef I2C_ANALOG_FILTER_DELAY_MIN
#define I2C_ANALOG_FILTER_DELAY_MIN 50 /* ns */
#endif

/* Available in the datasheet of the MCU. */
#ifndef I2C_ANALOG_FILTER_DELAY_MAX
#define I2C_ANALOG_FILTER_DELAY_MAX 260 /* ns */
#endif

/* Value in the range 0 <= x <= 15. */
#ifndef I2C_DIGITAL_FILTER_COEF
#define I2C_DIGITAL_FILTER_COEF 0
#endif


/**
 * This is for internal use. I2C bus compatible devices are not allowed to response to this
 * address. Here it is used to distinguish internally between target and controller mode.
 */
#define I2C_CONTROLLER_ADDRESS 0x01


/**
 * I2C timing configuration parameters sets for SM, FM and FMP.
 * 
 * @see https://www.nxp.com/docs/en/user-guide/UM10204.pdf#page=44
 */
static const _I2cTimingConfig i2cTimingConfig[3] = {
	/* 100 kHz (SM) */
	{ 100000, 0, 3450, 250, 4700, 4000, 1000, 300, I2C_DIGITAL_FILTER_COEF},
	/* 400 kHz (FM) */
	{ 400000, 0,  900, 100, 1300,  600,  300, 300, I2C_DIGITAL_FILTER_COEF},
	/* 1 MHz (FMP) */
	{1000000, 0,  450,  50,  500,  260,  120, 120, I2C_DIGITAL_FILTER_COEF}
};


/** Cached I2C timing values. */
_I2cTimingCache i2cTimingCache = {0, 0, 0, 0};


namespace {
/*
 * These global variables are introduced to map the IRQ event to the specific TwoWire
 * instance. It is used to allow flexible variable naming of the TwoWire instances
 * while preserving low latency response delays in the interrupt handler.
 * This approach also tries to minimize any memory overhead.
 * 
 * @see `getHandlePtrFromId()`
 */
#ifdef I2C1
I2C_HandleTypeDef * i2c1Handle = NULL;
#endif /* I2C1 */
#ifdef I2C2
I2C_HandleTypeDef * i2c2Handle = NULL;
#endif /* I2C2 */
#ifdef I2C3
I2C_HandleTypeDef * i2c3Handle = NULL;
#endif /* I2C3 */
#ifdef I2C4
I2C_HandleTypeDef * i2c4Handle = NULL;
#endif /* I2C4 */
#ifdef I2C5
I2C_HandleTypeDef * i2c5Handle = NULL;
#endif /* I2C5 */
#ifdef I2C6
I2C_HandleTypeDef * i2c6Handle = NULL;
#endif /* I2C6 */


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
 * Returns the pointer to the I2C handle which corresponds to the passed I2C instance.
 * 
 * @param[in,out] hI2c - I2C instance as defined by STM32 HAL API
 * @return pointer to I2C handler pointer
 * @remarks I2C1 has the same value as I2C1_BASE, but a different type. The same applies for the other values.
 */
I2C_HandleTypeDef ** getHandlePtrFromId(I2C_TypeDef * hI2c) {
	I2C_HandleTypeDef ** res = NULL;
	switch (reinterpret_cast<uintptr_t>(hI2c)) {
#ifdef I2C1
	case I2C1_BASE: res = &i2c1Handle; break;
#endif /* I2C1 */
#ifdef I2C2
	case I2C2_BASE: res = &i2c2Handle; break;
#endif /* I2C2 */
#ifdef I2C3
	case I2C3_BASE: res = &i2c3Handle; break;
#endif /* I2C3 */
#ifdef I2C4
	case I2C4_BASE: res = &i2c4Handle; break;
#endif /* I2C4 */
#ifdef I2C5
	case I2C5_BASE: res = &i2c5Handle; break;
#endif /* I2C5 */
#ifdef I2C6
	case I2C6_BASE: res = &i2c6Handle; break;
#endif /* I2C6 */
	default: break;
	}
	return res;
}


/* C++ SFINAE for handling the different names for HAL_I2C_Master_Seq_Transmit_IT(), HAL_I2C_Master_Seq_Receive_IT(), HAL_I2C_Slave_Seq_Transmit_IT() and HAL_I2C_Slave_Seq_Receive_IT(). */
DEF_ALT_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_I2C_Master_Seq_Transmit_IT_Wrapper, HAL_I2C_Master_Seq_Transmit_IT, HAL_I2C_Master_Sequential_Transmit_IT, I2C_HandleTypeDef *, uint16_t, uint8_t *, uint16_t, uint32_t)
DEF_ALT_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_I2C_Master_Seq_Receive_IT_Wrapper,  HAL_I2C_Master_Seq_Receive_IT,  HAL_I2C_Master_Sequential_Receive_IT,  I2C_HandleTypeDef *, uint16_t, uint8_t *, uint16_t, uint32_t)
DEF_ALT_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_I2C_Slave_Seq_Transmit_IT_Wrapper,  HAL_I2C_Slave_Seq_Transmit_IT,  HAL_I2C_Slave_Sequential_Transmit_IT,  I2C_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
DEF_ALT_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_I2C_Slave_Seq_Receive_IT_Wrapper,   HAL_I2C_Slave_Seq_Receive_IT,   HAL_I2C_Slave_Sequential_Receive_IT,   I2C_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
} /* namespace anonymous */



/* I2C IRQ handlers */
extern "C" {
/* IC2s */
/**
 * Helper function to handle both I2C event and error IRQs at the
 * same time.
 * 
 * @param[in,out] hI2c - I2C handler pointer
 */
static void HAL_I2C_EV_ER_IRQHandler(I2C_HandleTypeDef * hI2c) {
	HAL_I2C_EV_IRQHandler(hI2c);
	HAL_I2C_ER_IRQHandler(hI2c);
}

#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for I2Cx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForI2C##x(void) { \
		if (i2c##x##Handle != NULL) HAL_I2C_EV_ER_IRQHandler(i2c##x##Handle); \
	}

#ifdef I2C1
DEF_IRQ_HANDLER(1)
#endif /* I2C1 */

#ifdef I2C2
DEF_IRQ_HANDLER(2)
#endif /* I2C2 */

#ifdef I2C3
DEF_IRQ_HANDLER(3)
#endif /* I2C3 */

#ifdef I2C4
DEF_IRQ_HANDLER(4)
#endif /* I2C4 */

#ifdef I2C5
DEF_IRQ_HANDLER(5)
#endif /* I2C5 */

#ifdef I2C6
DEF_IRQ_HANDLER(6)
#endif /* I2C6 */


/**
 * Overwrites the STM32 HAL API handler for I2C target address received events.
 * 
 * @param[in,out] hI2c - pointer to I2C handle
 * @param[in] transferDirection - transfer direction request from controller's point of view (I2C_DIRECTION_TRANSMIT or I2C_DIRECTION_RECEIVE)
 * @param[in] addrMatchCode - address match code
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef * hI2c, uint8_t transferDirection, uint16_t addrMatchCode) {
	TwoWire * obj = getObjFromMemberPtr(hI2c, &TwoWire::handle);
	if (obj == NULL) return;
	/* Previous controller transaction has ended. Notify the upper layer. */
	if (obj->targetMode == TwoWire::TARGET_MODE_RECEIVE && obj->rxHead != obj->rxTail) {
		obj->onReceiveService();
		obj->targetMode = TwoWire::TARGET_MODE_LISTEN;
	}
	if (addrMatchCode != hI2c->Init.OwnAddress1) return;
	if (transferDirection == I2C_DIRECTION_RECEIVE) {
		obj->targetMode = TwoWire::TARGET_MODE_TRANSMIT;
		obj->onRequestService();
		HAL_I2C_Slave_Seq_Transmit_IT_Wrapper(hI2c, obj->txBuffer, obj->txBufferSize, I2C_LAST_FRAME);
	} else {
		obj->targetMode = TwoWire::TARGET_MODE_RECEIVE;
		/* Receive one byte at a time until the controller is done. */
		HAL_I2C_Slave_Seq_Receive_IT_Wrapper(hI2c, obj->recv, 1, I2C_NEXT_FRAME);
	}
}


/**
 * Overwrites the STM32 HAL API handler for I2C listen complete events.
 * 
 * @param[in,out] hI2c - pointer to I2C handle
 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef * hI2c) {
	TwoWire * obj = getObjFromMemberPtr(hI2c, &TwoWire::handle);
	if (obj == NULL) return;
	/* Previous controller transaction has ended. Notify the upper layer. */
	if (obj->targetMode == TwoWire::TARGET_MODE_RECEIVE && obj->rxHead != obj->rxTail) {
		obj->onReceiveService();
	}
	obj->targetMode = TwoWire::TARGET_MODE_LISTEN;
	HAL_I2C_EnableListen_IT(hI2c);
}


/**
 * Overwrites the STM32 HAL API handler for I2C target reception complete events.
 * 
 * @param[in,out] hI2c - pointer to I2C handle
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef * hI2c) {
	TwoWire * obj = getObjFromMemberPtr(hI2c, &TwoWire::handle);
	if (obj == NULL) return;
	_FIFOX_PUSH(RX_QUEUE(obj), *(obj->recv));
	/* receive next byte */
	if (obj->targetMode == TwoWire::TARGET_MODE_RECEIVE) {
		HAL_I2C_Slave_Seq_Receive_IT_Wrapper(hI2c, obj->recv, 1, I2C_NEXT_FRAME);
	}
}


/**
 * Overwrites the STM32 HAL API handler for I2C target transmission complete events.
 * 
 * @param[in,out] hI2c - pointer to I2C handle
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef * hI2c) {
	TwoWire * obj = getObjFromMemberPtr(hI2c, &TwoWire::handle);
	if (obj == NULL) return;
	obj->txBufferSize = 0;
}


/**
 * Overwrites the STM32 HAL API handler for I2C error events.
 * Error events are silently ignored and the I2C is reset to continue data transmission.
 * 
 * @param[in,out] hI2c - pointer to I2C handle
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef * hI2c) {
	TwoWire * obj = getObjFromMemberPtr(hI2c, &TwoWire::handle);
	if (obj == NULL || obj->isController) return;
	HAL_I2C_EnableListen_IT(hI2c);
}


/**
 * Calculates the I2C timing for the give peripheral clock speed
 * and target configuration.
 * 
 * @param[in] clockSpeed - peripheral clock speed
 * @param[in] itc - I2C timing configuration
 * @return timings encoded for use in STM32 HAL API
 * @see https://www.st.com/content/ccc/resource/technical/document/reference_manual/21/bd/0f/bd/1c/88/40/f0/DM00108282.pdf/files/DM00108282.pdf/jcr:content/translations/en.DM00108282.pdf
 * @note tAnalogFilterDelayMax and tSdaDelayMax are unused and only included in the code for information.
 * Those could be used to check the parameter constraints to fail if not met.
 */
uint32_t calculateI2cTiming(const uint32_t clockSpeed, const _I2cTimingConfig & itc) {
	const auto minZero = [](const int32_t val) -> uint32_t { return (val < 0) ? 0 : uint32_t(val); };
	uint32_t presc = 0;

	/* Convert frequency [Hz] to duration [ns]. */
	const uint32_t tPhyClk = (1000000000UL + (clockSpeed / 2)) / clockSpeed; /* round nearest */
	/* tScl == 2 * tSync + tSclLow + tSclHigh + <undefined rising period> + <undefined falling period>
	 * whereas the rising and falling slope is partly within tSync */
	const uint32_t tScl = (1000000000UL + (itc.freq / 2)) / itc.freq; /* round nearest */

	const uint32_t tAnalogFilterDelayMin = (I2C_USE_ANALOG_FILTER != 0) ? I2C_ANALOG_FILTER_DELAY_MIN : 0;
	//const uint32_t tAnalogFilterDelayMax = (I2C_USE_ANALOG_FILTER != 0) ? I2C_ANALOG_FILTER_DELAY_MAX : 0;

	const uint32_t tSdaDelayMin = minZero(int32_t(itc.fallTimeMax + itc.dataHoldMin) - tAnalogFilterDelayMin - ((itc.digitalNoiseFilterCoeff + 3) * tPhyClk));
	//const uint32_t tSdaDelayMax = minZero(int32_t(itc.dataValidMax) - itc.riseTimeMax - tAnalogFilterDelayMax - ((itc.digitalNoiseFilterCoeff + 4) * tPhyClk));
	const uint32_t tSclDelayMin = itc.riseTimeMax + itc.dataSetupMin;

	/* Use at most ~100 periphery ticks per single I2C interface tick. */
	const uint32_t tPhyClkMax = tScl / 100;
	for (uint32_t tPhyClkPresc = tPhyClk; presc < I2C_PRESC_MAX && tPhyClkPresc < tPhyClkMax; presc++, tPhyClkPresc += tPhyClk);
	if (presc >= I2C_PRESC_MAX) presc = I2C_PRESC_MAX - 1;
	const uint32_t tPresc = (presc + 1) * tPhyClk;
	
	const uint32_t sclDelay = minZero(int32_t((tSclDelayMin + tPresc - 1) / tPresc) - 1); /* round up */
	uint32_t sdaDelay = (tSdaDelayMin + tPresc - 1) / tPresc; /* round up */
	if (sdaDelay >= I2C_SDADELAY_MAX) sdaDelay = I2C_SDADELAY_MAX - 1;
	
	uint32_t tSclLowMin = tAnalogFilterDelayMin + ((itc.digitalNoiseFilterCoeff + 4) * tPhyClk);
	if (tSclLowMin < itc.lowSclMin) tSclLowMin = itc.lowSclMin;
	
	const uint32_t tSync = tAnalogFilterDelayMin + ((itc.digitalNoiseFilterCoeff + 2) * tPhyClk); /* synchronization time (lower bound) */
	/* Remaining time for SCL is distributed to low and high duration in the ratio of 2:1.
	 * The undefined periods of the rising and falling clock edges are assumed to be 50% within tSync. */
	const uint32_t tSclRem = minZero(int32_t(tScl) - ((itc.riseTimeMax + itc.fallTimeMax) / 2) - tSclLowMin - itc.highSclMin - (2 * tSync));
	const uint32_t sclHigh = minZero(((int32_t(itc.highSclMin + ((tSclRem + 1) / 3)) + tPresc - 1) / tPresc) - 1); /* round up */
	const uint32_t sclLow = minZero(((int32_t(tSclLowMin + (((2 * tSclRem) + 1) / 3)) + tPresc - 1) / tPresc) - 1); /* round up */
	
	return ((presc & 0x0F) << 28) | ((sclDelay & 0x0F) << 20) | ((sdaDelay & 0x0F) << 16) | ((sclHigh & 0xFF) << 8) | (sclLow & 0xFF);
}
} /* extern "C" */


/**
 * Helper function to calculate the I2C timing for the give peripheral clock speed
 * and target configuration.
 * 
 * @param[in] clockSpeed - peripheral clock speed
 * @param[in] itc - I2C timing configuration
 * @param[in,out] cachedValue - cached timing configuration to use/update
 * @return timings encoded for use in STM32 HAL API
 */
static uint32_t __attribute__((unused)) calculateCachedI2cTiming(const uint32_t clockSpeed, const _I2cTimingConfig & itc, uint32_t & cachedValue) {
	/* Return cached value if available. */
	if (i2cTimingCache.clockSpeed == clockSpeed) {
		/* Early out here if a cached timing configuration is available. */
		if (cachedValue != 0) return cachedValue;
	} else {
		/* Clear cache for new peripheral clock speed. */
		i2cTimingCache.clockSpeed = clockSpeed;
		i2cTimingCache.timing100kHz = 0;
		i2cTimingCache.timing400kHz = 0;
		i2cTimingCache.timing1MHz = 0;
	}
	cachedValue = calculateI2cTiming(clockSpeed, itc);
	return cachedValue;
}


/**
 * Helper function to perform a blocking I2C transfer via
 * the passed transfer function.
 * 
 * @param[in,out] hI2c - I2C instance as defined by STM32 HAL API
 * @param[in] timeout - timeout in milliseconds or HAL_MAX_DELAY to disable timeout
 * @param[out] timeoutFlag - set true if a timeout occurs
 * @param[in] fn - transfer function as `HAL_StatusTypeDef fn(void)`
 * @return 0 on success
 */
template <typename Fn>
static uint8_t i2cBlockingTransfer(I2C_HandleTypeDef * hI2c, const uint32_t timeout, bool & timeoutFlag, const Fn fn) {
	const uint32_t startTime = millis();
	if (fn() != HAL_OK) return TwoWire::I2C_ERROR;
	while (HAL_I2C_GetState(hI2c) != HAL_I2C_STATE_READY) {
		if (timeout != HAL_MAX_DELAY && (millis() - startTime) >= timeout) {
			timeoutFlag = true;
			return TwoWire::I2C_ERROR;
		}
		if (HAL_I2C_GetError(hI2c) != HAL_I2C_ERROR_NONE) {
			break;
		}
	}
	const uint32_t error = HAL_I2C_GetError(hI2c);
	if ((error & HAL_I2C_ERROR_TIMEOUT) == HAL_I2C_ERROR_TIMEOUT) {
		timeoutFlag = true;
	} else if ((error & HAL_I2C_ERROR_AF) == HAL_I2C_ERROR_AF) {
		return TwoWire::I2C_NACK_DATA;
	} else if (error == HAL_I2C_ERROR_NONE) {
		return TwoWire::I2C_OK;
	}
	return TwoWire::I2C_ERROR;
}


/**
 * Constructor.
 * 
 * @param[in,out] instance - I2C instance as defined by STM32 HAL API
 * @param[in] irqEvNum - associated IRQ for the I2C events as defined by STM32 HAL API
 * @param[in] irqErNum - associated IRQ for the I2C errors as defined by STM32 HAL API (may be same as irqEvNum)
 * @param[in] sclPin - PinName of the SCL pin
 * @param[in] sdaPin - PinName of the SDA pin
 * @param[in] sclAltFn - alternate function number of the SCL pin (see `pinMode()`)
 * @param[in] sdaAltFn - alternate function number of the SDA pin (see `pinMode()`)
 */
TwoWire::TwoWire(I2C_TypeDef * instance, const IRQn_Type irqEvNum, const IRQn_Type irqErNum, const PinName sclPin, const PinName sdaPin, const uint8_t sclAltFn, const uint8_t sdaAltFn):
	irqEv(irqEvNum),
	irqEr(irqErNum),
	pins{uint8_t(sclPin), uint8_t(sdaPin)},
	afns(uint8_t((sclAltFn << 4) | sdaAltFn)),
	txAddress(0),
	isController(true),
	transmitting(false),
	initialized(false),
	timeoutFlag(false),
	targetMode(TARGET_MODE_LISTEN),
	timeout((WIRE_DEFAULT_TIMEOUT > 0) ? WIRE_DEFAULT_TIMEOUT : HAL_MAX_DELAY),
	lastInitFn(NULL),
	txBufferSize(0),
	onRequestCallback(NULL),
	onReceiveCallback(NULL)
{
	memset(this->handle, 0, sizeof(*(this->handle)));
	this->handle->Instance = instance;
	I2C_HandleTypeDef ** handlePtr = getHandlePtrFromId(instance);
	if (handlePtr == NULL || (*handlePtr != NULL && (*handlePtr)->Instance != NULL)) {
		systemErrorHandler();
		return;
	}
	*handlePtr = this->handle;
	_FIFOX_INIT(RX_QUEUE(this));
}


/**
 * Destructor.
 */
TwoWire::~TwoWire() {
	I2C_HandleTypeDef ** handlePtr = getHandlePtrFromId(this->handle->Instance);
	if (handlePtr == NULL) return;
	*handlePtr = NULL;
}


/**
 * Starts the I2C interface with the given settings.
 * 
 * @param[in] enableGeneralCall - true to enable I2C generic calls
 * @param[in] initFn - initialization function for the I2C (defaults to that of STM32 HAL)
 * @remarks The function signature differs to that of legacy Arduino due to the additional initFn parameter.
 * @remarks The initFn parameter can be used to provide a wrapper around the STM32 HAL function for customized
 * driver initialization. This is needed, for example, to use non-default timing settings.
 */
void TwoWire::begin(const bool enableGeneralCall, HAL_StatusTypeDef (& initFn)(I2C_HandleTypeDef * hI2c)) {
	this->begin(uint8_t(I2C_CONTROLLER_ADDRESS), enableGeneralCall, initFn);
}


/**
 * Starts the I2C interface with the given settings.
 * 
 * @param[in] ownAddress - any value different from 1 will enable target mode instead of controller mode
 * @param[in] enableGeneralCall - true to enable I2C generic calls
 * @param[in] initFn - initialization function for the I2C (defaults to that of STM32 HAL)
 * @remarks The function signature differs to that of legacy Arduino due to the additional initFn parameter.
 * @remarks The initFn parameter can be used to provide a wrapper around the STM32 HAL function for customized
 * driver initialization. This is needed, for example, to use non-default timing settings.
 */
void TwoWire::begin(const uint8_t ownAddress, const bool enableGeneralCall, HAL_StatusTypeDef (& initFn)(I2C_HandleTypeDef * hI2c)) {
	/* enable I2C clock */
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef I2C1
	case I2C1_BASE:
		__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_I2C1_FORCE_RESET();
		__HAL_RCC_I2C1_RELEASE_RESET();
		break;
#endif /* I2C1 */
#ifdef I2C2
	case I2C2_BASE:
		__HAL_RCC_I2C2_CLK_ENABLE();
		__HAL_RCC_I2C2_FORCE_RESET();
		__HAL_RCC_I2C2_RELEASE_RESET();
		break;
#endif /* I2C2 */
#ifdef I2C3
	case I2C3_BASE:
		__HAL_RCC_I2C3_CLK_ENABLE();
		__HAL_RCC_I2C3_FORCE_RESET();
		__HAL_RCC_I2C3_RELEASE_RESET();
		break;
#endif /* I2C3 */
#ifdef I2C4
	case I2C4_BASE:
		__HAL_RCC_I2C4_CLK_ENABLE();
		__HAL_RCC_I2C4_FORCE_RESET();
		__HAL_RCC_I2C4_RELEASE_RESET();
		break;
#endif /* I2C4 */
#ifdef I2C5
	case I2C5_BASE:
		__HAL_RCC_I2C5_CLK_ENABLE();
		__HAL_RCC_I2C5_FORCE_RESET();
		__HAL_RCC_I2C5_RELEASE_RESET();
		break;
#endif /* I2C5 */
#ifdef I2C6
	case I2C6_BASE:
		__HAL_RCC_I2C6_CLK_ENABLE();
		__HAL_RCC_I2C6_FORCE_RESET();
		__HAL_RCC_I2C6_RELEASE_RESET();
		break;
#endif /* I2C6 */
	default:
		systemErrorHandler();
		break;
	}
	/* set pin mode and alternate function */
	pinModeEx(this->pins[0], ALTERNATE_FUNCTION_OPEN_DRAIN, this->afns >> 4);
	pinModeEx(this->pins[1], ALTERNATE_FUNCTION_OPEN_DRAIN, this->afns & 0xF);
	/* initialize */
	this->isController = (ownAddress == I2C_CONTROLLER_ADDRESS);
	this->handle->State = HAL_I2C_STATE_RESET;
#ifdef I2C_TIMINGR_PRESC
	if (this->handle->Init.Timing == 0) this->setClock(100000); /* Default to standard mode. */
#else /* not I2C_TIMINGR_PRESC */
	if (this->handle->Init.ClockSpeed == 0) this->setClock(100000); /* Default to standard mode. */
#endif /* not I2C_TIMINGR_PRESC */
	this->handle->Init.OwnAddress1 = (ownAddress != I2C_CONTROLLER_ADDRESS) ? (ownAddress << 1) : 0;
	this->handle->Init.OwnAddress2 = 0;
#ifdef I2C_OA2_NOMASK
	this->handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
#endif /* I2C_OA2_NOMASK */
	this->handle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	this->handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	this->handle->Init.GeneralCallMode = enableGeneralCall ? I2C_GENERALCALL_ENABLE : I2C_GENERALCALL_DISABLE;
	this->handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	this->lastInitFn = &initFn;
	if (initFn(this->handle) != HAL_OK) {
		systemErrorHandler();
	}
	this->initialized = true;
	this->timeoutFlag = false;
	this->targetMode = TARGET_MODE_LISTEN;
	_FIFOX_CLEAR(RX_QUEUE(this));
	if ( ! this->isController ) HAL_I2C_EnableListen_IT(this->handle);
	/* enable interrupts */
	HAL_NVIC_SetPriority(this->irqEv, UART_IRQ_PRIO, UART_IRQ_SUBPRIO);
	HAL_NVIC_EnableIRQ(this->irqEv);
	if (this->irqEv != this->irqEr) {
		HAL_NVIC_SetPriority(this->irqEr, UART_IRQ_PRIO, UART_IRQ_SUBPRIO);
		HAL_NVIC_EnableIRQ(this->irqEr);
	}
}


/**
 * Stops the I2C interface.
 */
void TwoWire::end() {
	this->flush();
	switch (reinterpret_cast<uintptr_t>(this->handle->Instance)) {
#ifdef I2C1
	case I2C1_BASE:
		__HAL_RCC_I2C1_FORCE_RESET();
		__HAL_RCC_I2C1_RELEASE_RESET();
		__HAL_RCC_I2C1_CLK_DISABLE();
		break;
#endif /* I2C1 */
#ifdef I2C2
	case I2C2_BASE:
		__HAL_RCC_I2C2_FORCE_RESET();
		__HAL_RCC_I2C2_RELEASE_RESET();
		__HAL_RCC_I2C2_CLK_DISABLE();
		break;
#endif /* I2C2 */
#ifdef I2C3
	case I2C3_BASE:
		__HAL_RCC_I2C3_FORCE_RESET();
		__HAL_RCC_I2C3_RELEASE_RESET();
		__HAL_RCC_I2C3_CLK_DISABLE();
		break;
#endif /* I2C3 */
#ifdef I2C4
	case I2C4_BASE:
		__HAL_RCC_I2C4_FORCE_RESET();
		__HAL_RCC_I2C4_RELEASE_RESET();
		__HAL_RCC_I2C4_CLK_DISABLE();
		break;
#endif /* I2C4 */
#ifdef I2C5
	case I2C5_BASE:
		__HAL_RCC_I2C5_FORCE_RESET();
		__HAL_RCC_I2C5_RELEASE_RESET();
		__HAL_RCC_I2C5_CLK_DISABLE();
		break;
#endif /* I2C5 */
#ifdef I2C6
	case I2C6_BASE:
		__HAL_RCC_I2C6_FORCE_RESET();
		__HAL_RCC_I2C6_RELEASE_RESET();
		__HAL_RCC_I2C6_CLK_DISABLE();
		break;
#endif /* I2C6 */
	default:
		systemErrorHandler();
		break;
	}
	HAL_I2C_DeInit(this->handle);
	this->initialized = false;
	_FIFOX_CLEAR(RX_QUEUE(this));
}


/**
 * Changes the frequency on the I2C interface. This also re-initializes it.
 * 
 * @param[in] clock - new interface speed in Hz
 */
void TwoWire::setClock(const uint32_t clock) {
#ifdef I2C_TIMINGR_PRESC
	uint32_t correctedClock;
	if (clock <= 100000) {
		correctedClock = 100000; /* 100 kHz*/
	} else if (clock <= 400000) {
		correctedClock = 400000; /* 400 kHz */
	} else {
		correctedClock = 1000000; /* 1 MHz */
	}
	/* The exact I2C timing behavior needs to be calculated and set. */
	switch (correctedClock) {
	case 100000: /* 100 kHz*/
#ifdef I2C_TIMINGR_PRESC_SM
		this->handle->Init.Timing = I2C_TIMINGR_PRESC_SM;
#else /* not I2C_TIMINGR_PRESC_SM */
		this->handle->Init.Timing = calculateCachedI2cTiming(getI2cClockFrequency(this->handle->Instance), i2cTimingConfig[0], i2cTimingCache.timing100kHz);
#endif /* not I2C_TIMINGR_PRESC_SM */
		break;
	case 400000: /* 400 kHz */
#ifdef I2C_TIMINGR_PRESC_FM
		this->handle->Init.Timing = I2C_TIMINGR_PRESC_FM;
#else /* not I2C_TIMINGR_PRESC_FM */
		this->handle->Init.Timing = calculateCachedI2cTiming(getI2cClockFrequency(this->handle->Instance), i2cTimingConfig[1], i2cTimingCache.timing400kHz);
#endif /* not I2C_TIMINGR_PRESC_FM */
		break;
	default: /* 1 MHz */
#ifdef I2C_TIMINGR_PRESC_FMP
		this->handle->Init.Timing = I2C_TIMINGR_PRESC_FMP;
#else /* not I2C_TIMINGR_PRESC_FMP */
		this->handle->Init.Timing = calculateCachedI2cTiming(getI2cClockFrequency(this->handle->Instance), i2cTimingConfig[2], i2cTimingCache.timing1MHz);
#endif /* not I2C_TIMINGR_PRESC_FMP */
		break;
	}
#else /* not I2C_TIMINGR_PRESC */
	/* Standard mode (SM) is up to 100kHz, then it is Fast mode (FM). */
	/* In fast mode the duty cycle bit must be set within the CCR register. */
	this->handle->Init.ClockSpeed = clock;
	if (clock > 100000) {
		this->handle->Init.DutyCycle = I2C_DUTYCYCLE_16_9;
	} else {
		this->handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
	}
#endif /* not I2C_TIMINGR_PRESC */
	/* Re-initialize I2C instance if it was already running to set the new speed. */
	if (this->initialized && this->lastInitFn != NULL) {
		if (this->lastInitFn(this->handle) != HAL_OK) {
			systemErrorHandler();
		}
		this->targetMode = TARGET_MODE_LISTEN;
		_FIFOX_CLEAR(RX_QUEUE(this));
		if ( ! this->isController ) HAL_I2C_EnableListen_IT(this->handle);
	}
}


/**
 * Sets a new timeout for I2C transmissions.
 * 
 * @param[in] newTimeout - timeout in milliseconds or 0 for no timeout
 * @param[in] resetWithTimeout - set true to reset the interface if a timeout occurs (unsupported)
 */
void TwoWire::setWireTimeout(const uint32_t newTimeout, const bool /* resetWithTimeout */) {
	this->timeout = (newTimeout > 0) ? newTimeout : HAL_MAX_DELAY;
}


/**
 * Starts a new transmission in controller mode.
 * The user can start sending data and needs to call `endTransmission()` at the end.
 * 
 * @param[in] address - target address
 */
void TwoWire::beginTransmission(const uint8_t address) {
	this->transmitting = true;
	this->txAddress = uint8_t(address << 1); /* Set address of the target. */
	this->txBufferSize = 0;
}


/**
 * Completes the current transmission in controller mode.
 * 
 * @return 0 on success
 */
uint8_t TwoWire::endTransmission() {
	return this->endTransmission(true);
}


/**
 * Completes the current transmission in controller mode.
 * 
 * @param[in] sendStop - 1 to send a stop message at the end to release the I2C bus
 * @return 0 on success
 */
uint8_t TwoWire::endTransmission(const uint8_t sendStop) {
#ifndef I2C_OTHER_FRAME
	UNUSED(sendStop);
#endif /* not I2C_OTHER_FRAME */
	uint8_t res = I2C_ERROR;
	if ( ! this->isController ) return res;
	/* Transmit buffer blocking. */
	if (this->txBufferSize > 0) {
		res = i2cBlockingTransfer(this->handle, this->timeout, this->timeoutFlag, [=]() -> HAL_StatusTypeDef {
#ifdef I2C_OTHER_FRAME
			return HAL_I2C_Master_Seq_Transmit_IT_Wrapper(this->handle, this->txAddress, this->txBuffer, this->txBufferSize, sendStop ? I2C_OTHER_AND_LAST_FRAME : I2C_OTHER_FRAME);
#else /* not I2C_OTHER_FRAME */
			return HAL_I2C_Master_Transmit_IT(this->handle, this->txAddress, this->txBuffer, this->txBufferSize);
#endif /* not I2C_OTHER_FRAME */
		});
	} else {
		/* Nothing to send. Return device status. */
		switch (HAL_I2C_IsDeviceReady(this->handle, this->txAddress, 1, this->timeout)) {
		case HAL_OK:
			res = I2C_OK;
			break;
		case HAL_TIMEOUT:
			if (this->handle->State != HAL_I2C_STATE_READY) {
				this->timeoutFlag = true;
			}
			/* fall-through */
		default:
			if (this->handle->State == HAL_I2C_STATE_READY) {
				res = I2C_NACK_ADDR;
			}
			break;
		}
	}
	this->txBufferSize = 0;
	this->transmitting = false;
	return res;
}


/**
 * Used in controller mode to request data from the target.
 * Afterwards, the received data can be requested with `available()` and `read()`.
 * 
 * @param[in] address - target address
 * @param[in] length - number of bytes to request
 * @return number of byes received
 */
uint8_t TwoWire::requestFrom(const uint8_t address, const uint8_t length) {
	return this->requestFrom(address, length, uint8_t(true));
}


/**
 * Used in controller mode to request data from the target.
 * Afterwards, the received data can be requested with `available()` and `read()`.
 * 
 * @param[in] address - target address
 * @param[in] length - number of bytes to request
 * @param[in] sendStop - 1 to send a stop message at the end to release the I2C bus
 * @return number of byes received
 */
uint8_t TwoWire::requestFrom(const uint8_t address, const uint8_t length, const uint8_t sendStop) {
	return this->requestFrom(address, length, uint32_t(0), uint8_t(0), sendStop);
}


/**
 * Used in controller mode to request data from the target.
 * Afterwards, the received data can be requested with `available()` and `read()`.
 * 
 * @param[in] address - target address
 * @param[in] length - number of bytes to request
 * @param[in] iaddress - remote register address
 * @param[in] isize - number of bytes of the remote register address (max 3)
 * @param[in] sendStop - 1 to send a stop message at the end to release the I2C bus
 * @return number of byes received
 */
uint8_t TwoWire::requestFrom(const uint8_t address, uint8_t length, const uint32_t iaddress, uint8_t isize, const uint8_t sendStop) {
#ifndef I2C_OTHER_FRAME
	UNUSED(sendStop);
#endif /* not I2C_OTHER_FRAME */
	if ( ! this->isController ) return 0;
	/* Sets `rxHead` and `rxTail` to 0, which is needed for the HAL receive function. */
	_FIFOX_INIT(RX_QUEUE(this));
	
	if (isize > 0) {
		/* Send internal address; this mode allows sending a repeated start to access
		 * some devices' internal registers.
		 */
		this->beginTransmission(address);
		/* The maximum size of an internal address is 3 bytes. */
		if (isize > 3) {
			isize = 3;
		}
		/* Write internal register address - most significant byte first. */
		for (; isize > 0; isize--) {
			this->write(uint8_t((iaddress >> (isize * 8)) & 0xFF));
		}
		this->endTransmission(false);
	}
	
	/* Perform blocking read into buffer. */
	const uint16_t size = min(uint16_t(length), uint16_t(TWOWIRE_RX_BUFFER_SIZE - 1));
	const uint8_t res = i2cBlockingTransfer(this->handle, this->timeout, this->timeoutFlag, [=]() -> HAL_StatusTypeDef {
#ifdef I2C_OTHER_FRAME
		return HAL_I2C_Master_Seq_Receive_IT_Wrapper(this->handle, uint8_t(address << 1), this->rxBuffer, size, sendStop ? I2C_OTHER_AND_LAST_FRAME : I2C_OTHER_FRAME);
#else /* not I2C_OTHER_FRAME */
		return HAL_I2C_Master_Receive_IT(this->handle, uint8_t(address << 1), this->rxBuffer, size);
#endif /* not I2C_OTHER_FRAME */
	});
	this->rxHead = (res == I2C_OK) ? rx_buffer_index_t(size) : 0;
	return this->rxHead;
}

/**
 * Returns the number of available bytes in the receive buffer.
 * This is available after `requestFrom()` or `onReceive()`.
 * 
 * @return number of bytes available for read
 */
int TwoWire::available(void) {
	return int(_FIFOX_SIZE(RX_QUEUE(this)));
}


/**
 * Returns the number of bytes which can be written without buffer overflow.
 * 
 * @return number of bytes available for write
 */
int TwoWire::availableForWrite(void) {
	return TWOWIRE_TX_BUFFER_SIZE - this->txBufferSize;
}


/**
 * Returns the first byte in the receive buffer without removing it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int TwoWire::read(void) {
	return _FIFOX_POP(RX_QUEUE(this));
}


/**
 * Returns a byte from the receive buffer and removes it.
 * 
 * @return first received byte in buffer if any, else -1
 */
int TwoWire::peek(void) {
	return _FIFOX_PEEK(RX_QUEUE(this));
}


/**
 * Waits until all data in transmission queue is sent.
 */
void TwoWire::flush(void) {
	this->txBufferSize = 0;
}


/**
 * Sends the given byte.
 * 
 * @param[in] val - byte value to send
 * @return number of bytes written to the transmission queue
 */
size_t TwoWire::write(const uint8_t data) {
	if (this->txBufferSize < TWOWIRE_TX_BUFFER_SIZE) {
		this->txBuffer[this->txBufferSize++] = data;
		return 1;
	}
	return 0;
}


/**
 * Sends the requested number of bytes from the given buffer.
 * 
 * @param[in] buffer - data to send
 * @param[in] length - size of the data in the buffer
 * @return number of bytes written to the transmission queue
 */
size_t TwoWire::write(const uint8_t * data, const size_t length) {
	const size_t remaining = size_t(TWOWIRE_TX_BUFFER_SIZE - this->txBufferSize);
	if (remaining > 0) {
		const size_t size = min(length, remaining);
		memcpy(this->txBuffer + this->txBufferSize, data, size);
		this->txBufferSize = tx_buffer_index_t(this->txBufferSize + size);
		return size;
	}
	return 0;
}


/**
 * Registers a function to be called when this target received a
 * transmission from a controller. 
 * 
 * @param[in] function - new callback function
 * @remarks The first parameter of the callback function will
 * received the number of bytes available in the receive buffer.
 */
void TwoWire::onReceive(void (* function)(int)) {
	this->onReceiveCallback = function;
}


/**
 * Register a function to be called when a controller requests
 * data from this target device. 
 * 
 * @param[in] function - new callback function
 */
void TwoWire::onRequest(void (* function)(void)) {
	this->onRequestCallback = function;
}


/**
 * Default initialization function for I2C via STM32 HAL.
 * 
 * @param[in,out] hI2c - I2C handler pointer
 */
HAL_StatusTypeDef TwoWire::init(I2C_HandleTypeDef * hI2c) {
	HAL_StatusTypeDef res;
	res = HAL_I2C_Init(hI2c);
	if (res != HAL_OK) return res;
#ifdef IS_I2C_ANALOG_FILTER
	res = HAL_I2CEx_ConfigAnalogFilter(hI2c, I2C_USE_ANALOG_FILTER ? I2C_ANALOGFILTER_ENABLE : I2C_ANALOGFILTER_DISABLE);
	if (res != HAL_OK) return res;
#endif /* IS_I2C_ANALOG_FILTER */
#ifdef IS_I2C_DIGITAL_FILTER
	res = HAL_I2CEx_ConfigDigitalFilter(hI2c, I2C_DIGITAL_FILTER_COEF);
	if (res != HAL_OK) return res;
#endif /* IS_I2C_DIGITAL_FILTER */
	return res;
}


/**
 * Informs the user about received data from the controller.
 */
void TwoWire::onReceiveService() {
	/* Alert user program. */
	if (this->onReceiveCallback == NULL) return;
	this->onReceiveCallback(this->available());
}


/**
 * Requests the user to send data to the controller.
 */
void TwoWire::onRequestService() {
	this->txBufferSize = 0;
	if (this->onRequestCallback == NULL) return;
	this->onRequestCallback();
}


#endif /* IS_I2C_ADDRESSING_MODE */
