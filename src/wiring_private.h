/**
 * @file wiring_private.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-10-14
 * @version 2022-03-21
 * 
 * @internal For internal use only.
 */
#ifndef __WIRING_PRIVATE_H__
#define __WIRING_PRIVATE_H__

#include <stdlib.h>
#include <stdint.h>


class HardwareTimer;


/**
 * Timer pin mapping structure.
 * 
 * @remarks Implemented in wiring_timer.cpp.
 */
struct _TimerPinMap {
	enum {
		InstanceBits = 5,
		ChannelBits = 2,
		InvertedBits = 1,
		AlternateFunctionBits = 8,
		InstanceMask = 0xF800,
		ChannelMask = 0x0600,
		InvertedMask = 0x0100,
		AlternateFunctionMask = 0x00FF,
		InstancePos = ChannelBits + InvertedBits + AlternateFunctionBits,
		ChannelPos = InvertedBits + AlternateFunctionBits,
		InvertedPos = AlternateFunctionBits,
		AlternateFunctionPos = 0
	};
	
	enum ClockSource {
		ClockSource_SYSCLK,
		ClockSource_CLKP,
		ClockSource_CSI,
		ClockSource_MSI,
		ClockSource_HSI,
		ClockSource_LSE,
		ClockSource_LSI,
		ClockSource_PCLK1,
		ClockSource_PCLK2,
		ClockSource_PCLK3,
		ClockSource_PCLK4,
		ClockSource_PLL2,
		ClockSource_PLL3,
		ClockSource_Unknown
	};
	
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	struct TimHandle {
		TIM_HandleTypeDef tim;
		HardwareTimer * obj;
	};
#endif /* TIM_CLOCKPRESCALER_DIV1 */
	
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	struct LpTimHandle {
		LPTIM_HandleTypeDef lptim;
		HardwareTimer * obj;
	};
#endif /* LPTIM_PRESCALER_DIV1 */
	
	uint16_t value; /**< Compact representation of instance, channel, inverted flag and alternate function number. */
	
	/**
	 * Constructor.
	 * 
	 * @param[in] inst - TIMx (1..24 as 1..24) and LPTIMx (1..5 as 25..29)
	 * @param[in] ch - channel (1..4 as 0..3)
	 * @param[in] inv - inverted flag (e.g. 1 for TIM1_CH1N and 0 for TIM1_CH1)
	 * @param[in] af - alternate function number for the given pin
	 */
	inline explicit _TimerPinMap(const uint16_t inst, const uint16_t ch, const uint16_t inv, const uint16_t af):
		value(uint16_t(
			((inst << InstancePos) & InstanceMask) |
			((ch << ChannelPos) & ChannelMask) |
			((inv << InvertedPos) & InvertedMask) |
			((af << AlternateFunctionPos) & AlternateFunctionMask)
		))
	{
		static_assert((InstanceBits + ChannelBits + InvertedBits + AlternateFunctionBits) <= (sizeof(this->value) * 8), "Bit mapping exceeds value range.");
	}
	
	/**
	 * Constructor.
	 * 
	 * @param[in] pin - named pin
	 * @remarks Instance is set 0 in case of no match.
	 * @remarks Implemented in wiring_timer.cpp
	 */
	_TimerPinMap(const uint32_t pin);
	
	/**
	 * Returns the instance number.
	 * 
	 * @return instance number
	 */
	inline uint16_t getInstanceNum() const {
		return uint16_t((this->value & InstanceMask) >> InstancePos);
	}
	
	/**
	 * Returns the base address of the given instance or 0 if invalid.
	 * 
	 * @return instance base address or 0
	 * @remarks Implemented in wiring_timer.cpp
	 */
	uint32_t getInstanceBaseAddr() const;
	
	/**
	 * Returns the timer clock source for the given instance.
	 * 
	 * @return clock source
	 * @remarks Implemented in wiring_timer.cpp
	 */
	inline ClockSource getClockSource() const {
		return _TimerPinMap::getClockSource(this->getInstanceNum());
	}
	
	/**
	 * Returns the timer clock source for the given instance.
	 * 
	 * @param[in] instNum - instance number
	 * @return clock source
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static ClockSource getClockSource(const uint16_t instNum);
	
	/**
	 * Returns the frequency in Hertz at which the clock for the given
	 * instance is working.
	 * 
	 * @return instance clock frequency or 0 on error
	 */
	inline uint32_t getClockFrequency() const {
		return _TimerPinMap::getClockFrequency(this->getClockSource());
	}
	
	/**
	 * Returns the frequency in Hertz at which the clock for the given
	 * instance is working.
	 * 
	 * @param[in] clkSrc - clock source to return the frequency of
	 * @return instance clock frequency or 0 on error
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static uint32_t getClockFrequency(const ClockSource clkSrc);
	
	/**
	 * Returns whether the stored instance is a low power instance (LPTIMx) or not.
	 * 
	 * @return true if low power instance, else false
	 */
	inline bool isLowPowerInstance() const {
		return _TimerPinMap::isLowPowerInstance(this->getInstanceNum());
	}
	
	/**
	 * Returns whether the given instance is a low power instance (LPTIMx) or not.
	 * 
	 * @param[in] instNum - instance number
	 * @return true if low power instance, else false
	 */
	inline static bool isLowPowerInstance(const uint16_t instNum) {
		return instNum >= 25;
	}
	
	/**
	 * Returns whether the stored instance has a 32-bit counter or not.
	 * 
	 * @return true if 32-bit counter instance, else false
	 */
	inline bool has32BitCounter() const {
		return _TimerPinMap::has32BitCounter(this->getInstanceNum());
	}
	
	/**
	 * Returns whether the stored instance has a 32-bit counter or not.
	 * 
	 * @param[in] instNum - instance number
	 * @return true if 32-bit counter instance, else false
	 */
	constexpr static inline bool has32BitCounter(const uint16_t instNum) {
#ifdef IS_TIM_32B_COUNTER_INSTANCE
#define __MAP_INST(instNum, is32Bit) | (uint32_t((is32Bit) ? 1 : 0) << ((instNum) - 1))
		const uint32_t is32BitMap = uint32_t(0
#ifdef TIM1_BASE
			__MAP_INST(1, IS_TIM_32B_COUNTER_INSTANCE(TIM1))
#endif /* TIM1_BASE */
#ifdef TIM2_BASE
			__MAP_INST(2, IS_TIM_32B_COUNTER_INSTANCE(TIM2))
#endif /* TIM2_BASE */
#ifdef TIM3_BASE
			__MAP_INST(3, IS_TIM_32B_COUNTER_INSTANCE(TIM3))
#endif /* TIM3_BASE */
#ifdef TIM4_BASE
			__MAP_INST(4, IS_TIM_32B_COUNTER_INSTANCE(TIM4))
#endif /* TIM4_BASE */
#ifdef TIM5_BASE
			__MAP_INST(5, IS_TIM_32B_COUNTER_INSTANCE(TIM5))
#endif /* TIM5_BASE */
#ifdef TIM6_BASE
			__MAP_INST(6, IS_TIM_32B_COUNTER_INSTANCE(TIM6))
#endif /* TIM6_BASE */
#ifdef TIM7_BASE
			__MAP_INST(7, IS_TIM_32B_COUNTER_INSTANCE(TIM7))
#endif /* TIM7_BASE */
#ifdef TIM8_BASE
			__MAP_INST(8, IS_TIM_32B_COUNTER_INSTANCE(TIM8))
#endif /* TIM8_BASE */
#ifdef TIM9_BASE
			__MAP_INST(9, IS_TIM_32B_COUNTER_INSTANCE(TIM9))
#endif /* TIM9_BASE */
#ifdef TIM10_BASE
			__MAP_INST(10, IS_TIM_32B_COUNTER_INSTANCE(TIM10))
#endif /* TIM10_BASE */
#ifdef TIM11_BASE
			__MAP_INST(11, IS_TIM_32B_COUNTER_INSTANCE(TIM11))
#endif /* TIM11_BASE */
#ifdef TIM12_BASE
			__MAP_INST(12, IS_TIM_32B_COUNTER_INSTANCE(TIM12))
#endif /* TIM12_BASE */
#ifdef TIM13_BASE
			__MAP_INST(13, IS_TIM_32B_COUNTER_INSTANCE(TIM13))
#endif /* TIM13_BASE */
#ifdef TIM14_BASE
			__MAP_INST(14, IS_TIM_32B_COUNTER_INSTANCE(TIM14))
#endif /* TIM14_BASE */
#ifdef TIM15_BASE
			__MAP_INST(15, IS_TIM_32B_COUNTER_INSTANCE(TIM15))
#endif /* TIM15_BASE */
#ifdef TIM16_BASE
			__MAP_INST(16, IS_TIM_32B_COUNTER_INSTANCE(TIM16))
#endif /* TIM16_BASE */
#ifdef TIM17_BASE
			__MAP_INST(17, IS_TIM_32B_COUNTER_INSTANCE(TIM17))
#endif /* TIM17_BASE */
#ifdef TIM18_BASE
			__MAP_INST(18, IS_TIM_32B_COUNTER_INSTANCE(TIM18))
#endif /* TIM18_BASE */
#ifdef TIM19_BASE
			__MAP_INST(19, IS_TIM_32B_COUNTER_INSTANCE(TIM19))
#endif /* TIM19_BASE */
#ifdef TIM20_BASE
			__MAP_INST(20, IS_TIM_32B_COUNTER_INSTANCE(TIM20))
#endif /* TIM20_BASE */
#ifdef TIM21_BASE
			__MAP_INST(21, IS_TIM_32B_COUNTER_INSTANCE(TIM21))
#endif /* TIM21_BASE */
#ifdef TIM22_BASE
			__MAP_INST(22, IS_TIM_32B_COUNTER_INSTANCE(TIM22))
#endif /* TIM22_BASE */
#ifdef TIM23_BASE
			__MAP_INST(23, IS_TIM_32B_COUNTER_INSTANCE(TIM23))
#endif /* TIM23_BASE */
#ifdef TIM24_BASE
			__MAP_INST(24, IS_TIM_32B_COUNTER_INSTANCE(TIM24))
#endif /* TIM24_BASE */
		);
		return ((is32BitMap >> (instNum - 1)) & 1) != 0;
#undef __MAP_INST
#else /* IS_TIM_32B_COUNTER_INSTANCE */
		(void)instNum;
		return false;
#endif /* IS_TIM_32B_COUNTER_INSTANCE */
	}
	
	/**
	 * Returns the channel number.
	 * 
	 * @return channel number
	 */
	inline uint16_t getChannelNum() const {
		return uint16_t((this->value & ChannelMask) >> ChannelPos);
	}
	
	/**
	 * Returns the STM32 HAL channel.
	 * 
	 * @return STM32 HAL channel
	 */
	uint32_t getChannel() const;
	
#if defined(LL_TIM_CHANNEL_CH1) || defined(LL_TIM_CHANNEL_CH1N) /* STM32 HAL LL TIM header was included */
	/**
	 * Returns the STM32 LL channel.
	 * 
	 * @return STM32 LL channel
	 */
	uint32_t getChannelLL() const;
#endif /* STM32 HAL LL TIM header was included */
	
	/**
	 * Returns the inverted flag.
	 * 
	 * @return inverted flag
	 */
	inline uint16_t getInvertedFlag() const {
		return uint16_t((this->value & InvertedMask) >> InvertedPos);
	}
	
	/**
	 * Returns the alternate function number.
	 * 
	 * @return alternate function number
	 */
	inline uint16_t getAfNum() const {
		return uint16_t((this->value & AlternateFunctionMask) >> AlternateFunctionPos);
	}
	
	/**
	 * Returns true if this is a valid timer.
	 * 
	 * @return true if valid, else false
	 */
	inline operator bool() const {
		return (this->value & InstanceMask) != 0;
	}
	
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	/**
	 * Returns the TIM handle which corresponds to the passed TIM instance.
	 * This also initializes the handle if retrieved for the first time.
	 * 
	 * @param[in,out] hTim - TIM instance as defined by STM32 HAL API
	 * @return TIM handler pointer
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static inline TIM_HandleTypeDef * getTimHandleFromId(TIM_TypeDef * hTim) {
		return reinterpret_cast<TIM_HandleTypeDef *>(_TimerPinMap::getInternalTimHandleFromId(hTim));
	}
	
	/**
	 * Returns the internal TIM handle which corresponds to the passed TIM instance.
	 * This also initializes the handle if retrieved for the first time.
	 * 
	 * @param[in,out] hTim - TIM instance as defined by STM32 HAL API
	 * @return internal TIM handler pointer
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static TimHandle * getInternalTimHandleFromId(TIM_TypeDef * hTim);
	
	/**
	 * Returns the instance number from the given TIM instance.
	 * 
	 * @param[in] hTim - TIM instance as defined by STM32 HAL API
	 * @return instance number or 0 on error
	 */
	static uint32_t getInstanceNumFromId(TIM_TypeDef * hTim);
#endif /* TIM_CLOCKPRESCALER_DIV1 */
	
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	/**
	 * Returns the LPTIM handle which corresponds to the passed LPTIM instance.
	 * This also initializes the handle if retrieved for the first time.
	 * 
	 * @param[in,out] hLpTim - LPTIM instance as defined by STM32 HAL API
	 * @return LPTIM handler pointer
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static inline LPTIM_HandleTypeDef * getLpTimHandleFromId(LPTIM_TypeDef * hLpTim) {
		return reinterpret_cast<LPTIM_HandleTypeDef *>(_TimerPinMap::getInternalLpTimHandleFromId(hLpTim));
	}
	
	/**
	 * Returns the internal LPTIM handle which corresponds to the passed LPTIM instance.
	 * This also initializes the handle if retrieved for the first time.
	 * 
	 * @param[in,out] hLpTim - LPTIM instance as defined by STM32 HAL API
	 * @return internal LPTIM handler pointer
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static LpTimHandle * getInternalLpTimHandleFromId(LPTIM_TypeDef * hLpTim);
	
	/**
	 * Returns the instance number from the given LPTIM instance.
	 * 
	 * @param[in] hLpTim - LPTIM instance as defined by STM32 HAL API
	 * @return instance number 0 on error
	 */
	static uint32_t getInstanceNumFromId(LPTIM_TypeDef * hLpTim);
#endif /* LPTIM_PRESCALER_DIV1 */

#ifdef STM32F1
	/**
	 * Activate and remap the given pin to the requested timer.
	 * 
	 * @param[in] pin - named pin
	 * @param[in] timer - internal timer number
	 * @remarks Implemented in wiring_timer.cpp
	 */
	static void f1PinModeTimer(const uint32_t pin, const uint16_t timer);
#endif /* STM32F1 */
};


/**
 * Structure to calculate the timing configuration from the given tick count.
 * Ideally, one would iterate through all possible combinations until a perfect
 * match or the closest one is found. This takes too long on a MCU which is why
 * just two cases are tried: 0x10000 and 10000, which assumes a clock frequency
 * with a multiple of 10kHz. 0x10000 maximizes the possible timer resolution.
 */
struct _TimerConfig {
	uint32_t prescaler; /**< HAL specific prescaler value */
	uint32_t period; /**< HAL specific period value */
	uint32_t error; /**< absolute factorization error */
	
	/**
	 * Constructor.
	 * 
	 * @param[in] ticks - desired tick count
	 * @param[in] factor1 - first factor in the equation prescaler * period = ticks
	 * @param[in] lowPowerPrescaler - calculate the LPTIM prescaler instead of the TIM prescaler
	 * @remarks All periods are assumed to be 16-bit.
	 * @remarks Implemented in wiring_timer.cpp
	 */
	explicit _TimerConfig(const uint32_t ticks, const uint32_t factor1, const bool lowPowerPrescaler);
};


#if !defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE) /* STM32 HAL I2C header was included */
/**
 * Structure with cached I2C timing configurations based on the used peripheral
 * clock speed and the requested I2C clock speed.
 */
struct _I2cTimingCache {
	uint32_t clockSpeed;   /**< Peripheral clock speed. */
	uint32_t timing100kHz; /**< Corresponding timing for 100 kHz I2C clock speed (or 0 if unset). */
	uint32_t timing400kHz; /**< Corresponding timing for 400 kHz I2C clock speed (or 0 if unset). */
	uint32_t timing1MHz;   /**< Corresponding timing for 1 MHz I2C clock speed (or 0 if unset). */
};


/**
 * I2C timing configuration parameter set.
 */
struct _I2cTimingConfig {
	uint32_t freq;                   /**< Interface frequency in Hz. */
	uint16_t dataHoldMin;            /**< Minimum data hold time in ns. */
	uint16_t dataValidMax;           /**< Maximum data valid time in ns. */
	uint16_t dataSetupMin;           /**< Minimum data setup time in ns. */
	uint16_t lowSclMin;              /**< Minimum low period of the SCL clock in ns. */
	uint16_t highSclMin;             /**< Minimum high period of SCL clock in ns. */
	uint16_t riseTimeMax;            /**< Maximum rise time in ns. */
	uint16_t fallTimeMax;            /**< Maximum fall time in ns. */
	uint8_t digitalNoiseFilterCoeff; /**< Digital noise filter coefficient (0..15). */
};
#endif /* STM32 HAL I2C header was included */


extern "C" {


/* Implemented in wiring_analog.cpp */
bool setAdcFromPin(const uint32_t pin, ADC_HandleTypeDef & hAdc, ADC_ChannelConfTypeDef & sConfig);

#if !defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE) /* STM32 HAL I2C header was included */
/* Implemented in wiring_timer.cpp */
uint32_t getI2cClockFrequency(const I2C_TypeDef * i2c);
uint32_t calculateI2cTiming(const uint32_t clockSpeed, const _I2cTimingConfig & itc);
#endif /* STM32 HAL I2C header was included */

#if !defined(STM32CUBEDUINO_DISABLE_SPI) && defined(IS_SPI_MODE) /* STM32 HAL SPI header was included */
/* Implemented in wiring_timer.cpp */
uint32_t getSpiClockFrequency(const SPI_TypeDef * spi);
#endif /* STM32 HAL SPI header was included */


} /* extern "C" */


#endif /* __WIRING_PRIVATE_H__ */
