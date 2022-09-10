/**
 * @file wiring_analog.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-10-02
 * @version 2022-09-10
 * 
 * @todo check why PWM output of 0 generates a short spike for the first interval at setup
 * @todo add support for SDADC (sigma-delta) (STM32F3 -> https://github.com/eleciawhite/STM32Cube/tree/master/STM32Cube_FW_F3_V1.3.0/Projects/STM32373C_EVAL/Examples/SDADC/SDADC_PressureMeasurement)
 */
#include "Arduino.h"
#include "wiring_private.h"
#include "scdinternal/macro.h"


#ifdef STM32CUBEDUINO_INTERNALS
#define _INTERNAL_ACCESS
#else /* not STM32CUBEDUINO_INTERNALS */
#define _INTERNAL_ACCESS static
#endif /* not STM32CUBEDUINO_INTERNALS */


#ifdef ADC_RESOLUTION_6B
#define MIN_ADC_RESOLUTION 6
#else /* not ADC_RESOLUTION_6B */
#ifdef ADC_RESOLUTION_8B
#define MIN_ADC_RESOLUTION 8
#else /* not ADC_RESOLUTION_8B */
#define MIN_ADC_RESOLUTION 12 /* this also applies for STM32F1 which has no ADC_RESOLUTION_xB */
#endif /* not ADC_RESOLUTION_8B */
#endif /* not ADC_RESOLUTION_6B */


#ifdef ADC_RESOLUTION_16B
#define MAX_ADC_RESOLUTION 16
#else /* not ADC_RESOLUTION_16B */
#define MAX_ADC_RESOLUTION 12
#endif /* not ADC_RESOLUTION_16B */


#define MAX_PWM_RESOLUTION 16


#if defined(STM32MP1)
#define INTERNAL_ADC_INSTANCE ADC2
#elif defined(STM32H7)
#if defined(STM32H7A3xx) || defined(STM32H7A3xxQ) || defined(STM32H7B3xx) || defined(STM32H7B3xxQ) || defined(STM32H7B0xx) || defined(STM32H7B0xxQ)
#define INTERNAL_ADC_INSTANCE ADC2
#else /* not STM32H7A or STM32H7B */
#define INTERNAL_ADC_INSTANCE ADC3
#endif /* not STM32H7A or STM32H7B */
#else /* not STM32MP1 or STM32H7 */
#if defined(ADC1)
#define INTERNAL_ADC_INSTANCE ADC1
#elif defined(ADC)
#define INTERNAL_ADC_INSTANCE ADC
#endif
#endif /* not STM32MP1 or STM32H7 */


#ifndef ADC_SAMPLINGTIME
#if defined(ADC_SAMPLETIME_8CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_8CYCLES_5;
#elif defined(ADC_SAMPLETIME_12CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_12CYCLES_5;
#elif defined(ADC_SAMPLETIME_13CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_13CYCLES_5;
#elif defined(ADC_SAMPLETIME_15CYCLES)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_15CYCLES;
#elif defined(ADC_SAMPLETIME_16CYCLES)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_16CYCLES;
#elif defined(ADC_SAMPLETIME_19CYCLES_5)
#define ADC_SAMPLINGTIME ADC_SAMPLETIME_19CYCLES_5;
#endif
#endif /* !ADC_SAMPLINGTIME */


/* Minimum ADC sampling time is required when reading
 * internal channels so set it to max possible value.
 * It can be defined more precisely by defining:
 * ADC_SAMPLINGTIME_INTERNAL
 * to the desired ADC sample time.
 */
#ifndef ADC_SAMPLINGTIME_INTERNAL
#if defined(ADC_SAMPLETIME_480CYCLES)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_480CYCLES
#elif defined(ADC_SAMPLETIME_384CYCLES)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_384CYCLES
#elif defined(ADC_SAMPLETIME_810CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_810CYCLES_5
#elif defined(ADC_SAMPLETIME_640CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_640CYCLES_5
#elif defined(ADC_SAMPLETIME_601CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_601CYCLES_5
#elif defined(ADC_SAMPLETIME_247CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_247CYCLES_5
#elif defined(ADC_SAMPLETIME_239CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_239CYCLES_5
#elif defined(ADC_SAMPLETIME_160CYCLES_5)
#define ADC_SAMPLINGTIME_INTERNAL ADC_SAMPLETIME_160CYCLES_5
#else
#error "ADC sampling time could not be defined for internal channels!"
#endif
#endif /* not ADC_SAMPLINGTIME_INTERNAL */


#ifndef ADC_CLOCK_DIV
#ifdef ADC_CLOCK_SYNC_PCLK_DIV4
#define ADC_CLOCK_DIV ADC_CLOCK_SYNC_PCLK_DIV4
#elif ADC_CLOCK_SYNC_PCLK_DIV2
#define ADC_CLOCK_DIV ADC_CLOCK_SYNC_PCLK_DIV2
#elif defined(ADC_CLOCK_ASYNC_DIV1)
#define ADC_CLOCK_DIV ADC_CLOCK_ASYNC_DIV1
#endif
#endif /* not ADC_CLOCK_DIV */


#ifndef ADC_REGULAR_RANK_1
#define ADC_REGULAR_RANK_1 1
#endif /* ADC_REGULAR_RANK_1 */


#if defined(DAC) && !defined(DAC1)
#define DAC1 DAC
#endif
#if defined(DAC1) && defined(DAC_BASE) && !defined(DAC1_BASE)
#define DAC1_BASE DAC_BASE
#endif /* not DAC1_BASE but DAC_BASE */


/* C++ SFINAE for handling the absence of AL_ADCEx_Calibration_Start() as a NOP and all variances accordingly */
template <typename T>
static inline auto HAL_ADCEx_Calibration_Start_Wrapper(const int, T hAdc, const uint32_t calibrationMode, uint32_t singleDiff) -> decltype(HAL_ADCEx_Calibration_Start(hAdc, calibrationMode, singleDiff)) {
	return HAL_ADCEx_Calibration_Start(hAdc, calibrationMode, singleDiff);
}

template <typename T>
static inline auto HAL_ADCEx_Calibration_Start_Wrapper(const int, T hAdc, const uint32_t, uint32_t singleDiff) -> decltype(HAL_ADCEx_Calibration_Start(hAdc, singleDiff)) {
	return HAL_ADCEx_Calibration_Start(hAdc, singleDiff);
}

template <typename T>
static inline auto HAL_ADCEx_Calibration_Start_Wrapper(const int, T hAdc, const uint32_t, uint32_t) -> decltype(HAL_ADCEx_Calibration_Start(hAdc)) {
	return HAL_ADCEx_Calibration_Start(hAdc);
}

template <typename T>
static inline HAL_StatusTypeDef HAL_ADCEx_Calibration_Start_Wrapper(const float, T, const uint32_t, const uint32_t) {
	return HAL_OK;
}

static inline HAL_StatusTypeDef HAL_ADCEx_Calibration_Start_Wrapper(ADC_HandleTypeDef * hAdc, const uint32_t calibrationMode, uint32_t singleDiff) {
	return HAL_ADCEx_Calibration_Start_Wrapper(0 /* prefer int here */, hAdc, calibrationMode, singleDiff);
}


/* C++ SFINAE for handling the absence of HAL_TIMEx_PWMN_Start() with HAL_ERROR */
DEF_NO_FN_ARGS_WRAPPER(uint32_t, HAL_TIMEx_PWMN_Start_Wrapper, HAL_TIMEx_PWMN_Start, HAL_ERROR, TIM_HandleTypeDef *, uint32_t)


extern "C" {


static int _readResolution = 10;
static int _internalReadResolution = 10;
static int _writeResolution = 8;
static uint32_t _writeFreq = 1000; /* minimum number of periods per second */


/**
 * Maps the given value from the source bit range to the given destination bit range
 * with rounding to the nearest value.
 * 
 * @param[in] value - value to map
 * @param[in] src - source bit range in number of bits
 * @param[in] dst - destination bit range in number of bits
 * @return mapped value in destination bit range
 */
static inline uint32_t mapBitRange(const uint32_t value, const int src, const int dst) {
	if (src == dst || value == 0) return value;
	if (src < dst) return value << (dst - src);
	const uint32_t roundingOffset = ((src + 1) <= dst) ? 1 : 1 << (src - dst - 1);
	return (value + roundingOffset) >> (src - dst);
}


/**
 * Helper function to return the ADC channel number from the given named pin.
 * 
 * @param[in] pin - named pin
 * @param[in,out] hAdc - set the Instance in this structure
 * @param[in,out] sConfig - set the channel number in this structure
 * @return true on success, else false
 * @remarks The function takes priority to the lower ADC instance numbers in case
 * that multiple are possible for a pin.
 * @remarks Support for the STM32 series F0, F1, F2, F3, F4, F7, G0, G4, H7, L0, L1, L4, L5, MP1 and WB.
 */
__attribute__((weak))
bool setAdcFromPin(const uint32_t pin, ADC_HandleTypeDef & hAdc, ADC_ChannelConfTypeDef & sConfig) {
	/* This enables the compiler to create a compact mapping table. */
	ADC_TypeDef * instance = hAdc.Instance;
#ifdef ADC_CHANNELS_BANK_B
	uint32_t bank = hAdc.Init.ChannelsBank;
#endif /* ADC_CHANNELS_BANK_B */
	uint32_t channel = sConfig.Channel;
	switch (pin) {
#if defined(STM32F0) || defined(STM32L0) /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_15; break;
#endif /* GPIOC_BASE */
#endif /* STM32F0 or STM32L0 */
#ifdef STM32G0 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PA_11: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PA_12: instance = ADC1; channel = ADC_CHANNEL_16; break;
	case PA_13: instance = ADC1; channel = ADC_CHANNEL_17; break;
	case PA_14: instance = ADC1; channel = ADC_CHANNEL_18; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PB_2: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PB_7:
	case PB_10: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PB_11: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PB_12: instance = ADC1; channel = ADC_CHANNEL_16; break;
#ifdef GPIOC_BASE
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_17; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_18; break;
#endif /* GPIOC_BASE */
#endif /* STM32G0 */
#if defined(STM32F1) || defined(STM32F2) /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_15; break;
#endif /* GPIOC_BASE */
#ifdef ADC3
	/* instance ADC3 */
#ifdef GPIOF_BASE
	case PF_3: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PF_4: instance = ADC3; channel = ADC_CHANNEL_14; break;
	case PF_5: instance = ADC3; channel = ADC_CHANNEL_15; break;
	case PF_6: instance = ADC3; channel = ADC_CHANNEL_4; break;
	case PF_7: instance = ADC3; channel = ADC_CHANNEL_5; break;
	case PF_8: instance = ADC3; channel = ADC_CHANNEL_6; break;
	case PF_9: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PF_10: instance = ADC3; channel = ADC_CHANNEL_8; break;
#endif /* GPIOF_BASE */
#endif /* ADC3 */
#endif /* STM32F1 or STM32F2 */
#ifdef STM32L1 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
#ifdef ADC_CHANNELS_BANK_B
	case PB_2: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_0; break;
#endif /* ADC_CHANNELS_BANK_B */
	case PB_12: instance = ADC1; channel = ADC_CHANNEL_18; break;
	case PB_13: instance = ADC1; channel = ADC_CHANNEL_19; break;
	case PB_14: instance = ADC1; channel = ADC_CHANNEL_20; break;
	case PB_15: instance = ADC1; channel = ADC_CHANNEL_21; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_15; break;
#endif /* GPIOC_BASE */
#ifdef GPIOE_BASE
	case PE_7: instance = ADC1; channel = ADC_CHANNEL_22; break;
	case PE_8: instance = ADC1; channel = ADC_CHANNEL_23; break;
	case PE_9: instance = ADC1; channel = ADC_CHANNEL_24; break;
	case PE_10: instance = ADC1; channel = ADC_CHANNEL_25; break;
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6: instance = ADC1; channel = ADC_CHANNEL_27; break;
	case PF_7: instance = ADC1; channel = ADC_CHANNEL_28; break;
	case PF_8: instance = ADC1; channel = ADC_CHANNEL_29; break;
	case PF_9: instance = ADC1; channel = ADC_CHANNEL_30; break;
	case PF_10: instance = ADC1; channel = ADC_CHANNEL_31; break;
#ifdef ADC_CHANNELS_BANK_B
	case PF_11: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_1; break;
	case PF_12: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_2; break;
	case PF_13: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_3; break;
	case PF_14: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_6; break;
	case PF_15: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_7; break;
#endif /* ADC_CHANNELS_BANK_B */
#endif /* GPIOF_BASE */
#if defined(GPIOG_BASE) && defined(ADC_CHANNELS_BANK_B)
	case PG_0: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_8; break;
	case PG_1: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_9; break;
	case PG_2: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_10; break;
	case PG_3: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_11; break;
	case PG_4: instance = ADC1; bank = ADC_CHANNELS_BANK_B; channel = ADC_CHANNEL_12; break;
#endif /* GPIOG_BASE and ADC_CHANNELS_BANK_B */
#endif /* STM32L1 */
#ifdef STM32F3 /* ****************************************** */
	/* instance ADC1 */
#if defined(STM32F373xC) || defined(STM32F378xx)
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_15; break;
#endif /* GPIOC_BASE */
#else /* not (STM32F373xC or STM32F378xx) */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PB_13: instance = ADC1; channel = ADC_CHANNEL_13; break;
#ifdef ADC2
	/* instance ADC2 */
	case PA_4: instance = ADC2; channel = ADC_CHANNEL_1; break;
	case PA_5: instance = ADC2; channel = ADC_CHANNEL_2; break;
	case PA_6: instance = ADC2; channel = ADC_CHANNEL_3; break;
	case PA_7: instance = ADC2; channel = ADC_CHANNEL_4; break;
	case PB_2: instance = ADC2; channel = ADC_CHANNEL_12; break;
	case PB_11: instance = ADC2; channel = ADC_CHANNEL_14; break;
	case PB_12: instance = ADC2; channel = ADC_CHANNEL_13; break;
	case PB_14: instance = ADC2; channel = ADC_CHANNEL_14; break;
	case PB_15: instance = ADC2; channel = ADC_CHANNEL_15; break;
#ifdef GPIOF_BASE
	case PF_4: instance = ADC1; channel = ADC_CHANNEL_5; break;
#endif /* GPIOF_BASE */
#ifdef GPIOC_BASE
	case PC_0: instance = ADC2; channel = ADC_CHANNEL_6; break;
	case PC_1: instance = ADC2; channel = ADC_CHANNEL_7; break;
	case PC_2: instance = ADC2; channel = ADC_CHANNEL_8; break;
	case PC_3: instance = ADC2; channel = ADC_CHANNEL_9; break;
	case PC_4: instance = ADC2; channel = ADC_CHANNEL_5; break;
	case PC_5: instance = ADC2; channel = ADC_CHANNEL_11; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_2: instance = ADC2; channel = ADC_CHANNEL_10; break;
#endif /* GPIOF_BASE */
#ifdef ADC3
	/* instance ADC3 */
#ifdef GPIOD_BASE
	case PD_10: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PD_11: instance = ADC3; channel = ADC_CHANNEL_8; break;
	case PD_12: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PD_13: instance = ADC3; channel = ADC_CHANNEL_10; break;
	case PD_14: instance = ADC3; channel = ADC_CHANNEL_11; break;
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_7: instance = ADC3; channel = ADC_CHANNEL_13; break;
	case PE_8: instance = ADC3; channel = ADC_CHANNEL_6; break;
	case PE_9: instance = ADC3; channel = ADC_CHANNEL_2; break;
	case PE_10: instance = ADC3; channel = ADC_CHANNEL_14; break;
	case PE_11: instance = ADC3; channel = ADC_CHANNEL_15; break;
	case PE_12: instance = ADC3; channel = ADC_CHANNEL_16; break;
	case PE_13: instance = ADC3; channel = ADC_CHANNEL_3; break;
#endif /* GPIOE_BASE */
#endif /* ADC3 */
#ifdef ADC4
	/* instance ADC4 */
#ifdef GPIOD_BASE
	case PD_8: instance = ADC4; channel = ADC_CHANNEL_12; break;
	case PD_9: instance = ADC4; channel = ADC_CHANNEL_13; break;
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_14: instance = ADC4; channel = ADC_CHANNEL_1; break;
	case PE_15: instance = ADC4; channel = ADC_CHANNEL_2; break;
#endif /* GPIOE_BASE */
#endif /* ADC4 */
#else /* not ADC2 */
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PB_11: instance = ADC1; channel = ADC_CHANNEL_14; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_9; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_2: instance = ADC1; channel = ADC_CHANNEL_10; break;
#endif /* GPIOF_BASE */
#endif /* not ADC2 */
#endif /* not (STM32F373xC or STM32F378xx) */
#endif /* STM32F3 */
#if defined(STM32F4) || defined(STM32F7) /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_0; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_9; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_15; break;
#endif /* GPIOC_BASE */
#ifdef ADC3
	/* instance ADC3 */
#ifdef GPIOF_BASE
	case PF_3: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PF_4: instance = ADC3; channel = ADC_CHANNEL_14; break;
	case PF_5: instance = ADC3; channel = ADC_CHANNEL_15; break;
	case PF_6: instance = ADC3; channel = ADC_CHANNEL_4; break;
	case PF_7: instance = ADC3; channel = ADC_CHANNEL_5; break;
	case PF_8: instance = ADC3; channel = ADC_CHANNEL_6; break;
	case PF_9: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PF_10: instance = ADC3; channel = ADC_CHANNEL_8; break;
#endif /* GPIOF_BASE */
#endif /* ADC3 */
#endif /* STM32F4 or STM32F7 */
#ifdef STM32L4 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_16; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_14; break;
#endif /* GPIOC_BASE */
#ifdef ADC3
	/* instance ADC3 */
#ifdef GPIOF_BASE
	case PF_3: instance = ADC3; channel = ADC_CHANNEL_6; break;
	case PF_4: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PF_5: instance = ADC3; channel = ADC_CHANNEL_8; break;
	case PF_6: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PF_7: instance = ADC3; channel = ADC_CHANNEL_10; break;
	case PF_8: instance = ADC3; channel = ADC_CHANNEL_11; break;
	case PF_9: instance = ADC3; channel = ADC_CHANNEL_12; break;
	case PF_10: instance = ADC3; channel = ADC_CHANNEL_13; break;
#endif /* GPIOF_BASE */
#endif /* ADC3 */
#endif /* STM32L4 */
#ifdef STM32G4 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PB_11: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PB_12: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PB_14: instance = ADC1; channel = ADC_CHANNEL_5; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_9; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
#endif /* GPIOF_BASE */
#ifdef ADC2
	/* instance ADC2 */
	case PA_4: instance = ADC2; channel = ADC_CHANNEL_17; break;
	case PA_5: instance = ADC2; channel = ADC_CHANNEL_13; break;
	case PA_6: instance = ADC2; channel = ADC_CHANNEL_3; break;
	case PA_7: instance = ADC2; channel = ADC_CHANNEL_4; break;
	case PB_2: instance = ADC2; channel = ADC_CHANNEL_12; break;
	case PB_15: instance = ADC2; channel = ADC_CHANNEL_15; break;
#ifdef GPIOC_BASE
	case PC_4: instance = ADC2; channel = ADC_CHANNEL_5; break;
	case PC_5: instance = ADC2; channel = ADC_CHANNEL_11; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_1: instance = ADC2; channel = ADC_CHANNEL_10; break;
#endif /* GPIOF_BASE */
#endif /* ADC2 */
#ifdef ADC3
	/* instance ADC3 */
	case PB_13: instance = ADC3; channel = ADC_CHANNEL_5; break;
#ifdef GPIOD_BASE
	case PD_10: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PD_11: instance = ADC3; channel = ADC_CHANNEL_8; break;
	case PD_12: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PD_13: instance = ADC3; channel = ADC_CHANNEL_10; break;
	case PD_14: instance = ADC3; channel = ADC_CHANNEL_11; break;
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_7: instance = ADC3; channel = ADC_CHANNEL_4; break;
	case PE_8: instance = ADC3; channel = ADC_CHANNEL_6; break;
	case PE_9: instance = ADC3; channel = ADC_CHANNEL_2; break;
	case PE_10: instance = ADC3; channel = ADC_CHANNEL_14; break;
	case PE_11: instance = ADC3; channel = ADC_CHANNEL_15; break;
	case PE_12: instance = ADC3; channel = ADC_CHANNEL_16; break;
	case PE_13: instance = ADC3; channel = ADC_CHANNEL_3; break;
#endif /* GPIOE_BASE */
#endif /* ADC3 */
#ifdef ADC4
	/* instance ADC4 */
#ifdef GPIOD_BASE
	case PD_8: instance = ADC4; channel = ADC_CHANNEL_12; break;
	case PD_9: instance = ADC4; channel = ADC_CHANNEL_13; break;
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_14: instance = ADC4; channel = ADC_CHANNEL_1; break;
	case PE_15: instance = ADC4; channel = ADC_CHANNEL_2; break;
#endif /* GPIOE_BASE */
#endif /* ADC4 */
#ifdef ADC5
	/* instance ADC5 */
	case PA_8: instance = ADC5; channel = ADC_CHANNEL_1; break;
	case PA_9: instance = ADC5; channel = ADC_CHANNEL_2; break;
#endif /* ADC5 */
#endif /* STM32G4 */
#ifdef STM32L5 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_16; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_14; break;
#endif /* GPIOC_BASE */
#endif /* STM32L5 */
#ifdef STM32H7 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_16; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_17; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_18; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_19; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_5; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_8; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_11: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PF_12: instance = ADC1; channel = ADC_CHANNEL_6; break;
	/* instance ADC2 */
	case PF_13: instance = ADC2; channel = ADC_CHANNEL_2; break;
	case PF_14: instance = ADC2; channel = ADC_CHANNEL_6; break;
	/* instance ADC3 */
	case PF_3: instance = ADC3; channel = ADC_CHANNEL_5; break;
	case PF_4: instance = ADC3; channel = ADC_CHANNEL_9; break;
	case PF_5: instance = ADC3; channel = ADC_CHANNEL_4; break;
	case PF_6: instance = ADC3; channel = ADC_CHANNEL_8; break;
	case PF_7: instance = ADC3; channel = ADC_CHANNEL_3; break;
	case PF_8: instance = ADC3; channel = ADC_CHANNEL_7; break;
	case PF_9: instance = ADC3; channel = ADC_CHANNEL_2; break;
	case PF_10: instance = ADC3; channel = ADC_CHANNEL_6; break;
#endif /* GPIOF_BASE */
#ifdef GPIOH_BASE
	case PH_2: instance = ADC3; channel = ADC_CHANNEL_13; break;
	case PH_3: instance = ADC3; channel = ADC_CHANNEL_14; break;
	case PH_4: instance = ADC3; channel = ADC_CHANNEL_15; break;
	case PH_5: instance = ADC3; channel = ADC_CHANNEL_16; break;
#endif /* GPIOH_BASE */
#endif /* STM32H7 */
#ifdef STM32MP1 /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_16; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_17; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_14; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_18; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_19; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PB_0: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PB_1: instance = ADC1; channel = ADC_CHANNEL_5; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_8; break;
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_11: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PF_12: instance = ADC1; channel = ADC_CHANNEL_6; break;
#ifdef ADC2
	/* instance ADC2 */
	case PF_13: instance = ADC2; channel = ADC_CHANNEL_2; break;
	case PF_14: instance = ADC2; channel = ADC_CHANNEL_6; break;
#endif /* ADC2 */
#endif /* GPIOF_BASE */
#endif /* STM32MP1 */
#ifdef STM32WB /* ****************************************** */
	/* instance ADC1 */
	case PA_0: instance = ADC1; channel = ADC_CHANNEL_5; break;
	case PA_1: instance = ADC1; channel = ADC_CHANNEL_6; break;
	case PA_2: instance = ADC1; channel = ADC_CHANNEL_7; break;
	case PA_3: instance = ADC1; channel = ADC_CHANNEL_8; break;
	case PA_4: instance = ADC1; channel = ADC_CHANNEL_9; break;
	case PA_5: instance = ADC1; channel = ADC_CHANNEL_10; break;
	case PA_6: instance = ADC1; channel = ADC_CHANNEL_11; break;
	case PA_7: instance = ADC1; channel = ADC_CHANNEL_12; break;
	case PA_8: instance = ADC1; channel = ADC_CHANNEL_15; break;
	case PA_9: instance = ADC1; channel = ADC_CHANNEL_16; break;
#ifdef GPIOC_BASE
	case PC_0: instance = ADC1; channel = ADC_CHANNEL_1; break;
	case PC_1: instance = ADC1; channel = ADC_CHANNEL_2; break;
	case PC_2: instance = ADC1; channel = ADC_CHANNEL_3; break;
	case PC_3: instance = ADC1; channel = ADC_CHANNEL_4; break;
	case PC_4: instance = ADC1; channel = ADC_CHANNEL_13; break;
	case PC_5: instance = ADC1; channel = ADC_CHANNEL_14; break;
#endif /* GPIOC_BASE */
#endif /* STM32WB */
#ifdef ADC_CHANNEL_TEMPSENSOR
	case PINTERN_TEMPSENSOR:      instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_TEMPSENSOR; break;
#endif /* ADC_CHANNEL_TEMPSENSOR */
#ifdef ADC_CHANNEL_TEMPSENSOR_ADC1
	case PINTERN_TEMPSENSOR_ADC1: instance = ADC1; channel = ADC_CHANNEL_TEMPSENSOR_ADC1; break;
#endif /* ADC_CHANNEL_TEMPSENSOR_ADC1 */
#ifdef ADC_CHANNEL_TEMPSENSOR_ADC5
	case PINTERN_TEMPSENSOR_ADC5: instance = ADC5; channel = ADC_CHANNEL_TEMPSENSOR_ADC5; break;
#endif /* ADC_CHANNEL_TEMPSENSOR_ADC5 */
#ifdef ADC_CHANNEL_VBAT
	case PINTERN_VBAT:            instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_VBAT; break;
#endif /* ADC_CHANNEL_VBAT */
#ifdef ADC_CHANNEL_VCOMP
	case PINTERN_VCOMP:           instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_VCOMP; break;
#endif /* ADC_CHANNEL_VCOMP */
#ifdef ADC_CHANNEL_VCORE
	case PINTERN_VCORE:           instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_VCORE; break;
#endif /* ADC_CHANNEL_VCORE */
#ifdef ADC_CHANNEL_VLCD
	case PINTERN_VLCD:            instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_VLCD; break;
#endif /* ADC_CHANNEL_VLCD */
#ifdef ADC_CHANNEL_VREFINT
	case PINTERN_VREFINT:         instance = INTERNAL_ADC_INSTANCE; channel = ADC_CHANNEL_VREFINT; break;
#endif /* ADC_CHANNEL_VREFINT */
	default: break;
	}
	hAdc.Instance = instance;
#ifdef ADC_CHANNELS_BANK_B
	hAdc.Init.ChannelsBank = bank;
#endif /* ADC_CHANNELS_BANK_B */
	sConfig.Channel = channel;
	return true;
}


} /* extern "C" */


/**
 * Sets the analog read resolution.
 * The default is 10 bits.
 * 
 * @param[in] res - new read resolution
 * @remarks systemErrorHandler() is called in case of an invalid number of bits.
 */
void analogReadResolution(const int res) {
	if (res < 0 || res > 32) systemErrorHandler();
	_readResolution = res;
	_internalReadResolution = (_readResolution + 1) & 0xFE; /* round up to an even number */
	if (_readResolution > MAX_ADC_RESOLUTION) {
		_internalReadResolution = MAX_ADC_RESOLUTION;
	} else if (_readResolution < MIN_ADC_RESOLUTION) {
		_internalReadResolution = MIN_ADC_RESOLUTION;
	}
}


/**
 * Reads the analog value from the given pin.
 * 
 * @param[in] pin - named pin
 * @return analog value according to the read resolution or 0 in case of an error
 */
uint32_t analogRead(const uint32_t pin) {
	ADC_HandleTypeDef hAdc = {0};
	ADC_ChannelConfTypeDef sConfig = {0};
	uint32_t samplingTime = (digitalPinToPort(pin) == PortIntern) ? ADC_SAMPLINGTIME_INTERNAL : ADC_SAMPLINGTIME;
	uint32_t result = 0;
	
	/* initializes the ADC instance */
#ifdef ADC_CHANNELS_BANK_A
	hAdc.Init.ChannelsBank = ADC_CHANNELS_BANK_A;
#endif /* ADC_CHANNELS_BANK_A */
	if ( ! setAdcFromPin(pin, hAdc, sConfig) ) return 0;
#ifdef ADC_CLOCK_DIV
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ClockPrescaler, ADC_CLOCK_DIV);
#endif /* ADC_CLOCK_DIV */
#ifdef ADC_RESOLUTION_12B
	switch (_internalReadResolution) {
#ifdef ADC_RESOLUTION_6B
	case 6:
		hAdc.Init.Resolution = ADC_RESOLUTION_6B;
		break;
#endif /* ADC_RESOLUTION_6B */
	case 8:
		hAdc.Init.Resolution = ADC_RESOLUTION_8B;
		break;
	case 10:
		hAdc.Init.Resolution = ADC_RESOLUTION_10B;
		break;
	case 12:
	default:
		hAdc.Init.Resolution = ADC_RESOLUTION_12B;
		break;
#ifdef ADC_RESOLUTION_14B
	case 14:
		hAdc.Init.Resolution = ADC_RESOLUTION_14B;
		break;
#endif /* ADC_RESOLUTION_14B */
#ifdef ADC_RESOLUTION_16B
	case 16:
		hAdc.Init.Resolution = ADC_RESOLUTION_16B;
		break;
#endif /* ADC_RESOLUTION_16B */
	}
#endif /* ADC_RESOLUTION_12B */
#ifdef ADC_DATAALIGN_RIGHT
	SET_FOR_EXISTING_MEMBER(hAdc.Init, DataAlign, ADC_DATAALIGN_RIGHT);
#endif /* ADC_DATAALIGN_RIGHT*/
#if defined(ADC_SCAN_SEQ_FIXED)
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ScanConvMode, ADC_SCAN_SEQ_FIXED);
#elif defined(ADC_SCAN_DISABLE)
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ScanConvMode, ADC_SCAN_DISABLE);
#else /* ADC_SCAN_DISABLE */
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ScanConvMode, DISABLE);
#endif /* ADC_SCAN_SEQ_FIXED*/
#ifdef ADC_EOC_SINGLE_CONV
	SET_FOR_EXISTING_MEMBER(hAdc.Init, EOCSelection, ADC_EOC_SINGLE_CONV);
#endif /* ADC_EOC_SINGLE_CONV */
	SET_FOR_EXISTING_MEMBER(hAdc.Init, LowPowerAutoWait, DISABLE);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, LowPowerAutoPowerOff, DISABLE);
	hAdc.Init.ContinuousConvMode = DISABLE;
	SET_FOR_EXISTING_MEMBER(hAdc.Init, NbrOfConversion, 1);
	hAdc.Init.DiscontinuousConvMode = DISABLE;
	SET_FOR_EXISTING_MEMBER(hAdc.Init, NbrOfDiscConversion, 0);
	hAdc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ExternalTrigConvEdge, ADC_EXTERNALTRIGCONVEDGE_NONE);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, DMAContinuousRequests, DISABLE);
#ifdef ADC_CONVERSIONDATA_DR
	SET_FOR_EXISTING_MEMBER(hAdc.Init, ConversionDataManagement, ADC_CONVERSIONDATA_DR);
#endif /* ADC_CONVERSIONDATA_DR */
#ifdef ADC_OVR_DATA_OVERWRITTEN
	SET_FOR_EXISTING_MEMBER(hAdc.Init, Overrun, ADC_OVR_DATA_OVERWRITTEN);
#endif /* ADC_OVR_DATA_OVERWRITTEN */
#ifdef ADC_LEFTBITSHIFT_NONE
	SET_FOR_EXISTING_MEMBER(hAdc.Init, LeftBitShift, ADC_LEFTBITSHIFT_NONE);
#endif /* ADC_LEFTBITSHIFT_NONE */
	SET_FOR_EXISTING_MEMBER(hAdc.Init, SamplingTimeCommon, samplingTime);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, SamplingTimeCommon1, samplingTime);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, SamplingTimeCommon2, samplingTime);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, LowPowerFrequencyMode, DISABLE);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, SamplingTime, samplingTime);
	SET_FOR_EXISTING_MEMBER(hAdc.Init, OversamplingMode, DISABLE);
#ifdef ADC_DFSDM_MODE_DISABLE
	SET_FOR_EXISTING_MEMBER(hAdc.Init, DFSDMConfig, ADC_DFSDM_MODE_DISABLE);
#endif /* ADC_DFSDM_MODE_DISABLE */
#ifdef ADC_TRIGGER_FREQ_HIGH
	SET_FOR_EXISTING_MEMBER(hAdc.Init, TriggerFrequencyMode, ADC_TRIGGER_FREQ_HIGH);
#endif /* ADC_TRIGGER_FREQ_HIGH */
	hAdc.State = HAL_ADC_STATE_RESET;
	hAdc.DMA_Handle = NULL;
	hAdc.Lock = HAL_UNLOCKED;
	pinMode(pin, INPUT_ANALOG);
	if (HAL_ADC_Init(&hAdc) != HAL_OK) {
		return 0;
	}
	/* initialize the ADC channel */
#if defined(STM32L4) || defined(STM32L5) || defined(STM32WB) || defined(STM32G4)
	if ( ! IS_ADC_CHANNEL(&hAdc, sConfig.Channel) ) {
#else /* not (STM32L4 or STM32WB or STM32G4) */
	if ( ! IS_ADC_CHANNEL(sConfig.Channel) ) {
#endif /* not (STM32L4 or STM32WB or STM32G4) */
		return 0;
	}
#ifdef ADC_SCAN_SEQ_FIXED
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
#else /* not ADC_SCAN_SEQ_FIXED */
	sConfig.Rank = ADC_REGULAR_RANK_1;
#endif /* not ADC_SCAN_SEQ_FIXED */
#ifdef STM32G0
	SET_FOR_EXISTING_MEMBER(sConfig, SamplingTime, ADC_SAMPLINGTIME_COMMON_1);
#else /* not STM32G0 */
	SET_FOR_EXISTING_MEMBER(sConfig, SamplingTime, samplingTime);
#endif /* not STM32G0 */
#ifdef ADC_SINGLE_ENDED
	SET_FOR_EXISTING_MEMBER(sConfig, SingleDiff, ADC_SINGLE_ENDED);
#endif /* ADC_SINGLE_ENDED */
#ifdef ADC_OFFSET_NONE
	SET_FOR_EXISTING_MEMBER(sConfig, OffsetNumber, ADC_OFFSET_NONE);
#endif /* ADC_OFFSET_NONE */
	SET_FOR_EXISTING_MEMBER(sConfig, Offset, 0);
	SET_FOR_EXISTING_MEMBER(sConfig, OffsetRightShift, DISABLE);
	SET_FOR_EXISTING_MEMBER(sConfig, OffsetSignedSaturation, DISABLE);
	if (HAL_ADC_ConfigChannel(&hAdc, &sConfig) != HAL_OK) {
		return 0;
	}
	
	/* calibrate the ADC */
#ifdef ADC_SINGLE_ENDED
#ifdef ADC_CALIB_OFFSET
	if (HAL_ADCEx_Calibration_Start_Wrapper(&hAdc, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK) {
#else /* not ADC_CALIB_OFFSET */
	if (HAL_ADCEx_Calibration_Start_Wrapper(&hAdc, 0, ADC_SINGLE_ENDED) != HAL_OK) {
#endif /* not ADC_CALIB_OFFSET */
#else /* not ADC_SINGLE_ENDED */
	if (HAL_ADCEx_Calibration_Start_Wrapper(&hAdc, 0, 0) != HAL_OK) {
#endif /* not ADC_SINGLE_ENDED */
		return 0;
	}
	
	/* start the conversion */
	if (HAL_ADC_Start(&hAdc) != HAL_OK) {
		return 0;
	}
	
	/* wait for the end of conversion process */
	if (HAL_ADC_PollForConversion(&hAdc, 10 /* ms timeout */) != HAL_OK) {
		return 0;
	}
	
	/* get the value */
	if ((HAL_ADC_GetState(&hAdc) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC) {
		result = HAL_ADC_GetValue(&hAdc);
	}
	
	/* stop the conversion process */
	if (HAL_ADC_Stop(&hAdc) != HAL_OK) {
		return 0;
	}
	
	/* stop the ADC */
	if (HAL_ADC_DeInit(&hAdc) != HAL_OK) {
		return 0;
	}
#if defined(__LL_ADC_COMMON_INSTANCE) && defined(LL_ADC_SetCommonPathInternalCh) && defined(LL_ADC_PATH_INTERNAL_NONE)
	if (__LL_ADC_COMMON_INSTANCE(hAdc.Instance) != 0) {
		LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(hAdc.Instance), LL_ADC_PATH_INTERNAL_NONE);
	}
#endif /* __LL_ADC_COMMON_INSTANCE and LL_ADC_SetCommonPathInternalCh and LL_ADC_PATH_INTERNAL_NONE */
	
	/* map internal to requested read resolution */
	return mapBitRange(result, _internalReadResolution, _readResolution);
}


/**
 * Sets the analog write resolution.
 * The default is 8 bits.
 * 
 * @param[in] res - new write resolution
 * @remarks systemErrorHandler() is called in case of an invalid number of bits.
 */
void analogWriteResolution(const int res) {
	if (res < 0 || res > 32) systemErrorHandler();
	_writeResolution = res;
}



#if !defined(STM32CUBEDUINO_DISABLE_DAC) && defined(DAC_TRIGGER_NONE) /* STM32 HAL DAC header was included */
/**
 * Internal helper function to write analog via DAC.
 * 
 * @param[in] pin - named pin
 * @param[in] instance - DAC instance
 * @param[in] channel - DAC channel number
 * @param[in] value - raw analog value according to the write resolution
 * @return true on success, else false
 */
_INTERNAL_ACCESS bool analogWriteViaDac(const uint32_t pin, DAC_TypeDef * instance, const uint32_t channel, const uint32_t value) {
	pinMode(pin, OUTPUT_ANALOG);
	DAC_HandleTypeDef hDac = {0};
	DAC_ChannelConfTypeDef sChannel = {0};
	hDac.Instance = instance;
	if (HAL_DAC_DeInit(&hDac) != HAL_OK) return false;
	if (HAL_DAC_Init(&hDac) != HAL_OK) return false;
	sChannel.DAC_Trigger = DAC_TRIGGER_NONE;
	sChannel.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hDac, &sChannel, channel) != HAL_OK) return false;
#ifdef DAC_ALIGN_12B_R
	if (_writeResolution <= 8) {
		if (HAL_DAC_SetValue(&hDac, channel, DAC_ALIGN_8B_R, mapBitRange(value, _writeResolution, 8)) != HAL_OK) return false;
	} else {
		if (HAL_DAC_SetValue(&hDac, channel, DAC_ALIGN_12B_R, mapBitRange(value, _writeResolution, 12)) != HAL_OK) return false;
	}
#else /* not DAC_ALIGN_12B_R */
	if (HAL_DAC_SetValue(&hDac, channel, DAC_ALIGN_8B_R, mapBitRange(value, _writeResolution, 8)) != HAL_OK) return false;
#endif /* not DAC_ALIGN_12B_R */
	if (HAL_DAC_Start(&hDac, channel) != HAL_OK) return false;
	return true;
}
#endif /* DAC_TRIGGER_NONE */


#if !defined(STM32CUBEDUINO_DISABLE_PWM) && defined(TIM_CLOCKPRESCALER_DIV1) /* STM32 HAL TIM header was included */
/**
 * Internal helper function to write analog via PWM output.
 * 
 * @param[in] pin - named pin
 * @param[in] pinMap - timer to pin mapping
 * @param[in] value - raw analog value according to the write resolution
 * @return true on success, else false
 * @remarks pinMap is not automatically derived from pin to allow alternative timer mappings.
 */
_INTERNAL_ACCESS bool analogWriteViaPwm(const uint32_t pin, const _TimerPinMap pinMap, const uint32_t value) {
	if ( ! pinMap ) return false;
	if ( pinMap.isLowPowerInstance() ) {
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
		LPTIM_TypeDef * inst = reinterpret_cast<LPTIM_TypeDef *>(pinMap.getInstanceBaseAddr());
		LPTIM_HandleTypeDef * hLpTim = _TimerPinMap::getLpTimHandleFromId(inst);
		if (hLpTim == NULL) return false;
		pinModeEx(pin, ALTERNATE_FUNCTION, pinMap.getAfNum());
		hLpTim->Instance = inst;
		hLpTim->Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
		hLpTim->Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
		hLpTim->Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_LOW;
		hLpTim->Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
		hLpTim->Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
		SET_FOR_EXISTING_MEMBER(hLpTim->Init, RepetitionCounter, 0);
		/* default prescaler to the highest value */
		uint32_t prescaler = LPTIM_PRESCALER_DIV128;
		/* Round to the next higher possible number of periods per second. */
		/* __builtin_clz() translates to ASM CLR on ARM which return 32 for input value 0. This is different than x86 architecture for example. */
		switch ((sizeof(unsigned int) * 8) - __builtin_clz(pinMap.getTimerClockFrequency() / (_writeFreq * 0xFFFFUL))) {
		case 0:
		case 1: prescaler = LPTIM_PRESCALER_DIV1; break;
		case 2: prescaler = LPTIM_PRESCALER_DIV2; break;
		case 3: prescaler = LPTIM_PRESCALER_DIV4; break;
		case 4: prescaler = LPTIM_PRESCALER_DIV8; break;
		case 5: prescaler = LPTIM_PRESCALER_DIV16; break;
		case 6: prescaler = LPTIM_PRESCALER_DIV32; break;
		case 7: prescaler = LPTIM_PRESCALER_DIV64; break;
		default: break;
		}
		hLpTim->Init.Clock.Prescaler = prescaler;
		if (HAL_LPTIM_Init(hLpTim) != HAL_OK) return false;
		/* The interval from 0xFFFE to 0 also counts as a single interval which is why there are 0xFFFF intervals here. */
		if (HAL_LPTIM_PWM_Start(hLpTim, 0xFFFE, mapBitRange(value, _writeResolution, 16)) != HAL_OK) return false;
#endif /* LPTIM_PRESCALER_DIV1 */
	} else {
		TIM_TypeDef * inst = reinterpret_cast<TIM_TypeDef *>(pinMap.getInstanceBaseAddr());
		TIM_HandleTypeDef * hTim = _TimerPinMap::getTimHandleFromId(inst);
		if (hTim == NULL) return false;
		TIM_OC_InitTypeDef pwmChannel = {0};
		TIM_ClockConfigTypeDef clkSrc = {0};
		const uint32_t channel = pinMap.getChannel();
#ifdef STM32F1
		_TimerPinMap::f1PinModeTimer(pin, pinMap.getAfNum());
#else /* not STM32F1 */
		pinModeEx(pin, ALTERNATE_FUNCTION, pinMap.getAfNum());
#endif /* not STM32F1 */
		hTim->Instance = inst;
		/* Rounds to the next higher possible number of periods per second. */
		hTim->Init.Prescaler = uint32_t(pinMap.getTimerClockFrequency() / (_writeFreq * 0xFFFFUL));
		/* The interval from 0xFFFE to 0 also counts as a single interval which is why there are 0xFFFF intervals here. */
		hTim->Init.Period = 0xFFFE; /* no 32-bit timer specialization -> treat all as 16-bit timer */
		hTim->Init.CounterMode = TIM_COUNTERMODE_UP;
		hTim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		hTim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
		SET_FOR_EXISTING_MEMBER(hTim->Init, RepetitionCounter, 0);
		if (HAL_TIM_PWM_Init(hTim) != HAL_OK) return false;
		clkSrc.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(hTim, &clkSrc) != HAL_OK) return false;
		pwmChannel.OCMode = TIM_OCMODE_PWM1;
		pwmChannel.Pulse = mapBitRange(value, _writeResolution, 16);
		pwmChannel.OCPolarity = TIM_OCPOLARITY_HIGH;
		pwmChannel.OCFastMode = TIM_OCFAST_DISABLE;
#ifdef TIM_OCIDLESTATE_RESET
		SET_FOR_EXISTING_MEMBER(pwmChannel, OCIdleState, TIM_OCIDLESTATE_RESET);
#endif /* TIM_OCIDLESTATE_RESET */
#ifdef TIM_OCNPOLARITY_HIGH
		SET_FOR_EXISTING_MEMBER(pwmChannel, OCNPolarity, TIM_OCNPOLARITY_HIGH);
#endif /* TIM_OCNPOLARITY_HIGH */
#ifdef TIM_OCNIDLESTATE_RESET
		SET_FOR_EXISTING_MEMBER(pwmChannel, OCNIdleState, TIM_OCNIDLESTATE_RESET);
#endif /* TIM_OCNIDLESTATE_RESET */
		HAL_TIM_PWM_ConfigChannel(hTim, &pwmChannel, channel);
		if ( pinMap.getInvertedFlag() ) {
			if (HAL_TIMEx_PWMN_Start_Wrapper(hTim, channel) != HAL_OK) return false;
		} else {
			if (HAL_TIM_PWM_Start(hTim, channel) != HAL_OK) return false;
		}
	}
	return true;
}
#endif /* TIM_CLOCKPRESCALER_DIV1 */


/**
 * Sets an analog value at the given pin. The is done via DAC, PWM or digital.
 * In this order depending on the function availability.
 * 
 * @param[in] pin - named pin
 * @param[in] value - analog value according to the write resolution
 */
void analogWrite(const uint32_t pin, const uint32_t value) {
	if (_writeResolution < 1) return;

	/* DAC pin mapping is the same for all STM32 devices in the series F0, F1, F2, F3, F4, F7, G0, G4, H7, L0, L1, L4, L5, MP1 and WB. */
	/* DAC3 and DAC4 can only be connected to chip-peripherals (e.g. as OPAMP input). */
#if !defined(STM32CUBEDUINO_DISABLE_DAC) && defined(DAC_TRIGGER_NONE) /* STM32 HAL DAC header was included */
	switch (pin) {
#ifdef DAC_CR_EN1
	case PA_4:
		if ( analogWriteViaDac(pin, DAC1, DAC_CHANNEL_1, value) ) return;
		break;
#endif /* DAC_CR_EN1 */
#ifdef DAC_CR_EN2
	case PA_5:
		if ( analogWriteViaDac(pin, DAC1, DAC_CHANNEL_2, value) ) return;
		break;
#endif /* DAC_CR_EN2 */
#ifdef DAC2
	case PA_6:
		if ( analogWriteViaDac(pin, DAC2, DAC_CHANNEL_1, value) ) return;
		break;
#endif /* DAC2 */
	default: break; /* try next option below */
	}
#endif /* DAC_TRIGGER_NONE */

#if !defined(STM32CUBEDUINO_DISABLE_PWM) && defined(TIM_CLOCKPRESCALER_DIV1) /* STM32 HAL TIM header was included */
	/* Try to output the analog value as PWM signal. */
	if ( analogWriteViaPwm(pin, _TimerPinMap(pin), value) ) return;
#endif /* TIM_CLOCKPRESCALER_DIV1 */

	/* Fallback: Only write digital on/off value according to the write resolution. */
	pinMode(pin, OUTPUT);
	digitalWrite(pin, (value < (uint32_t(1) << (_writeResolution - 1))) ? LOW : HIGH);
}


#ifndef STM32CUBEDUINO_LEGACY_API
/**
 * Changes the PWM output period frequency to the given value in Hz.
 * The default is 1000Hz.
 * 
 * @param[in] freq - new PWM frequency
 */
void analogWriteFrequency(const uint32_t freq) {
	_writeFreq = freq;
}
#endif /* STM32CUBEDUINO_LEGACY_API */


extern "C" {


/* implementation of STM32 HAL related callback hooks */
#if !defined(STM32CUBEDUINO_DISABLE_ADC) && defined(ADC_SOFTWARE_START) /* STM32 HAL ADC header was included */
/**
 * Overwrites the STM32 HAL API handler for ADC MSP initialization request.
 * 
 * @param[in,out] hAdc - pointer to ADC handle
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef * hAdc) {
	switch (reinterpret_cast<uintptr_t>(hAdc->Instance)) {
#ifdef ADC1 /* ****************************************** */
	case ADC1_BASE:
#ifdef __HAL_RCC_ADC1_CLK_ENABLE
		__HAL_RCC_ADC1_CLK_ENABLE();
#endif /* __HAL_RCC_ADC1_CLK_ENABLE */
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
		__HAL_RCC_ADC12_CLK_ENABLE();
#endif /* __HAL_RCC_ADC12_CLK_ENABLE */
		break;
#endif /* ADC1 */
#ifdef ADC2 /* ****************************************** */
	case ADC2_BASE:
#ifdef __HAL_RCC_ADC2_CLK_ENABLE
		__HAL_RCC_ADC2_CLK_ENABLE();
#endif /* __HAL_RCC_ADC2_CLK_ENABLE */
#ifdef __HAL_RCC_ADC12_CLK_ENABLE
		__HAL_RCC_ADC12_CLK_ENABLE();
#endif /* __HAL_RCC_ADC12_CLK_ENABLE */
		break;
#endif /* ADC2 */
#ifdef ADC3 /* ****************************************** */
	case ADC3_BASE:
#ifdef __HAL_RCC_ADC3_CLK_ENABLE
		__HAL_RCC_ADC3_CLK_ENABLE();
#endif /* __HAL_RCC_ADC3_CLK_ENABLE */
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
		__HAL_RCC_ADC34_CLK_ENABLE();
#endif /* __HAL_RCC_ADC34_CLK_ENABLE */
#ifdef __HAL_RCC_ADC345_CLK_ENABLE
		__HAL_RCC_ADC345_CLK_ENABLE();
#endif /* __HAL_RCC_ADC345_CLK_ENABLE */
		break;
#endif /* ADC3 */
#ifdef ADC4 /* ****************************************** */
	case ADC4_BASE:
#ifdef __HAL_RCC_ADC34_CLK_ENABLE
		__HAL_RCC_ADC34_CLK_ENABLE();
#endif /* __HAL_RCC_ADC34_CLK_ENABLE */
#ifdef __HAL_RCC_ADC345_CLK_ENABLE
		__HAL_RCC_ADC345_CLK_ENABLE();
#endif /* __HAL_RCC_ADC345_CLK_ENABLE */
		break;
#endif /* ADC4 */
#ifdef ADC5 /* ****************************************** */
	case ADC5_BASE:
#ifdef __HAL_RCC_ADC345_CLK_ENABLE
		__HAL_RCC_ADC345_CLK_ENABLE();
#endif /* __HAL_RCC_ADC345_CLK_ENABLE */
		break;
#endif /* ADC5 */
	default: break;
	}
#ifdef __HAL_RCC_ADC_CLK_ENABLE
	__HAL_RCC_ADC_CLK_ENABLE();
#endif
	/* For STM32F1, STM32H7, and STM32MP1 ADC pre-scaler is configured in initVariant() (see board.cpp) */
#if defined(__HAL_RCC_ADC_CONFIG) && !defined(STM32F1) && !defined(STM32H7) && !defined(STM32MP1)
	__HAL_RCC_ADC_CONFIG(RCC_ADCCLKSOURCE_SYSCLK);
#endif
}


/**
 * Overwrites the STM32 HAL API handler for ADC MSP de-initialization request.
 * 
 * @param[in,out] hAdc - pointer to ADC handle
 */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef * hAdc) {
#ifdef __HAL_RCC_ADC_FORCE_RESET
	__HAL_RCC_ADC_FORCE_RESET();
#endif
#ifdef __HAL_RCC_ADC_RELEASE_RESET
	__HAL_RCC_ADC_RELEASE_RESET();
#endif
#ifndef ADC_CR2_ADON
	constexpr uint32_t enabledInstances = 0;
	(void)enabledInstances; /* suppress unused warning */
#else /* not ADC_CR2_ADON */
	uint32_t enabledInstances = 0;
#ifdef ADC1
	if( HAL_IS_BIT_SET(ADC1->CR2, ADC_CR2_ADON) ) enabledInstances |= (1 << 0);
#endif /* ADC1 */
#ifdef ADC2
	if( HAL_IS_BIT_SET(ADC2->CR2, ADC_CR2_ADON) ) enabledInstances |= (1 << 1);
#endif /* ADC2 */
#ifdef ADC3
	if( HAL_IS_BIT_SET(ADC3->CR2, ADC_CR2_ADON) ) enabledInstances |= (1 << 2);
#endif /* ADC3 */
#ifdef ADC4
	if( HAL_IS_BIT_SET(ADC4->CR2, ADC_CR2_ADON) ) enabledInstances |= (1 << 3);
#endif  /* ADC4 */
#ifdef ADC5
	if( HAL_IS_BIT_SET(ADC5->CR2, ADC_CR2_ADON) ) enabledInstances |= (1 << 4);
#endif /* ADC5 */
#endif /* not ADC_CR2_ADON */
	switch (reinterpret_cast<uintptr_t>(hAdc->Instance)) {
#ifdef ADC1 /* ****************************************** */
	case ADC1_BASE:
#ifdef __HAL_RCC_ADC1_FORCE_RESET
		__HAL_RCC_ADC1_FORCE_RESET();
#endif /* __HAL_RCC_ADC1_FORCE_RESET */
#ifdef __HAL_RCC_ADC1_RELEASE_RESET
		__HAL_RCC_ADC1_RELEASE_RESET();
#endif /* __HAL_RCC_ADC1_RELEASE_RESET */
#ifdef __HAL_RCC_ADC12_FORCE_RESET
		if ((enabledInstances & 0x3) == 0) __HAL_RCC_ADC12_FORCE_RESET();
#endif /* __HAL_RCC_ADC12_FORCE_RESET */
#ifdef __HAL_RCC_ADC1_CLK_DISABLE
		__HAL_RCC_ADC1_CLK_DISABLE();
#endif /* __HAL_RCC_ADC1_CLK_DISABLE */
#ifdef __HAL_RCC_ADC12_CLK_DISABLE
		if ((enabledInstances & 0x3) == 0) __HAL_RCC_ADC12_CLK_DISABLE();
#endif /* __HAL_RCC_ADC12_CLK_DISABLE */
		break;
#endif /* ADC1 */
#ifdef ADC2 /* ****************************************** */
	case ADC2_BASE:
#ifdef __HAL_RCC_ADC2_FORCE_RESET
		__HAL_RCC_ADC2_FORCE_RESET();
#endif /* __HAL_RCC_ADC2_FORCE_RESET */
#ifdef __HAL_RCC_ADC2_RELEASE_RESET
		__HAL_RCC_ADC2_RELEASE_RESET();
#endif /* __HAL_RCC_ADC2_RELEASE_RESET */
#ifdef __HAL_RCC_ADC12_FORCE_RESET
		if ((enabledInstances & 0x03) == 0) __HAL_RCC_ADC12_FORCE_RESET();
#endif /* __HAL_RCC_ADC12_FORCE_RESET */
#ifdef __HAL_RCC_ADC2_CLK_DISABLE
		__HAL_RCC_ADC2_CLK_DISABLE();
#endif /* __HAL_RCC_ADC2_CLK_DISABLE */
#ifdef __HAL_RCC_ADC12_CLK_DISABLE
		if ((enabledInstances & 0x03) == 0) __HAL_RCC_ADC12_CLK_DISABLE();
#endif /* __HAL_RCC_ADC12_CLK_DISABLE */
		break;
#endif /* ADC2 */
#ifdef ADC3 /* ****************************************** */
	case ADC3_BASE:
#ifdef __HAL_RCC_ADC3_FORCE_RESET
		__HAL_RCC_ADC3_FORCE_RESET();
#endif /* __HAL_RCC_ADC3_FORCE_RESET */
#ifdef __HAL_RCC_ADC3_RELEASE_RESET
		__HAL_RCC_ADC3_RELEASE_RESET();
#endif /* __HAL_RCC_ADC3_RELEASE_RESET */
#ifdef __HAL_RCC_ADC34_FORCE_RESET
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_FORCE_RESET();
#endif /* __HAL_RCC_ADC34_FORCE_RESET */
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_RELEASE_RESET();
#endif /* __HAL_RCC_ADC34_RELEASE_RESET */
#ifdef __HAL_RCC_ADC3_CLK_DISABLE
		__HAL_RCC_ADC3_CLK_DISABLE();
#endif /* __HAL_RCC_ADC3_CLK_DISABLE */
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_CLK_DISABLE();
#endif /* __HAL_RCC_ADC34_CLK_DISABLE */
#ifdef __HAL_RCC_ADC345_FORCE_RESET
		if ((enabledInstances & 0x1C) == 0) {
			__HAL_RCC_ADC345_FORCE_RESET();
			__HAL_RCC_ADC345_RELEASE_RESET();
			__HAL_RCC_ADC345_CLK_DISABLE();
		}
#endif /* __HAL_RCC_ADC345_FORCE_RESET */
		break;
#endif /* ADC3 */
#ifdef ADC4 /* ****************************************** */
	case ADC4_BASE:
#ifdef __HAL_RCC_ADC34_FORCE_RESET
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_FORCE_RESET();
#endif /* __HAL_RCC_ADC34_FORCE_RESET */
#ifdef __HAL_RCC_ADC34_RELEASE_RESET
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_RELEASE_RESET();
#endif /* __HAL_RCC_ADC34_RELEASE_RESET */
#ifdef __HAL_RCC_ADC34_CLK_DISABLE
		if ((enabledInstances & 0x0C) == 0) __HAL_RCC_ADC34_CLK_DISABLE();
#endif /* __HAL_RCC_ADC34_CLK_DISABLE */
#ifdef __HAL_RCC_ADC345_FORCE_RESET
		if ((enabledInstances & 0x1C) == 0) {
			__HAL_RCC_ADC345_FORCE_RESET();
			__HAL_RCC_ADC345_RELEASE_RESET();
			__HAL_RCC_ADC345_CLK_DISABLE();
		}
#endif /* __HAL_RCC_ADC345_FORCE_RESET */
		break;
#endif /* ADC4 */
#ifdef ADC5 /* ****************************************** */
	case ADC5_BASE:
#ifdef __HAL_RCC_ADC345_FORCE_RESET
		if ((enabledInstances & 0x1C) == 0) {
			__HAL_RCC_ADC345_FORCE_RESET();
			__HAL_RCC_ADC345_RELEASE_RESET();
			__HAL_RCC_ADC345_CLK_DISABLE();
		}
#endif /* __HAL_RCC_ADC345_FORCE_RESET */
		break;
#endif /* ADC5 */
	default: break;
	}
#ifdef __HAL_RCC_ADC_CLK_DISABLE
	if (enabledInstances == 0) {
		__HAL_RCC_ADC_FORCE_RESET();
		__HAL_RCC_ADC_RELEASE_RESET();
		__HAL_RCC_ADC_CLK_DISABLE();
	}
#endif /* __HAL_RCC_ADC_CLK_DISABLE */
}
#endif /* ADC_SOFTWARE_START */


#if !defined(STM32CUBEDUINO_DISABLE_DAC) && defined(DAC_TRIGGER_NONE) /* STM32 HAL DAC header was included */
/**
 * Overwrites the STM32 HAL API handler for DAC MSP initialization request.
 * 
 * @param[in,out] hDac - pointer to DAC handle
 */
void HAL_DAC_MspInit(DAC_HandleTypeDef * hDac) {
	switch (reinterpret_cast<uintptr_t>(hDac->Instance)) {
#ifdef DAC1 /* ****************************************** */
	case DAC1_BASE:
#ifdef __HAL_RCC_DAC_CLK_ENABLE
		__HAL_RCC_DAC_CLK_ENABLE();
#endif /* __HAL_RCC_DAC_CLK_ENABLE */
#ifdef __HAL_RCC_DAC1_CLK_ENABLE
		__HAL_RCC_DAC1_CLK_ENABLE();
#endif /* __HAL_RCC_DAC1_CLK_ENABLE */
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
		__HAL_RCC_DAC12_CLK_ENABLE();
#endif /* __HAL_RCC_DAC12_CLK_ENABLE */
		break;
#endif /* DAC1 */
#ifdef DAC2 /* ****************************************** */
	case DAC2_BASE:
#ifdef __HAL_RCC_DAC2_CLK_ENABLE
		__HAL_RCC_DAC2_CLK_ENABLE();
#endif /* __HAL_RCC_DAC2_CLK_ENABLE */
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
		__HAL_RCC_DAC12_CLK_ENABLE();
#endif /* __HAL_RCC_DAC12_CLK_ENABLE */
		break;
#endif /* DAC2 */
#ifdef DAC3 /* ****************************************** */
	case DAC3_BASE:
#ifdef __HAL_RCC_DAC3_CLK_ENABLE
		__HAL_RCC_DAC3_CLK_ENABLE();
#endif /* __HAL_RCC_DAC3_CLK_ENABLE */
		break;
#endif /* DAC3 */
#ifdef DAC4 /* ****************************************** */
	case DAC4_BASE:
#ifdef __HAL_RCC_DAC4_CLK_ENABLE
		__HAL_RCC_DAC4_CLK_ENABLE();
#endif /* __HAL_RCC_DAC4_CLK_ENABLE */
		break;
#endif /* DAC4 */
	default: break;
	}
}


/**
 * Overwrites the STM32 HAL API handler for DAC MSP de-initialization request.
 * 
 * @param[in,out] hDac - pointer to DAC handle
 */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef * hDac) {
	switch (reinterpret_cast<uintptr_t>(hDac->Instance)) {
#ifdef DAC1 /* ****************************************** */
	case DAC1_BASE:
#ifdef __HAL_RCC_DAC_FORCE_RESET
		__HAL_RCC_DAC_FORCE_RESET();
#endif /* __HAL_RCC_DAC_FORCE_RESET */
#ifdef __HAL_RCC_DAC1_FORCE_RESET
		__HAL_RCC_DAC1_FORCE_RESET();
#endif /* __HAL_RCC_DAC1_FORCE_RESET */
#ifdef __HAL_RCC_DAC12_FORCE_RESET
		__HAL_RCC_DAC12_FORCE_RESET();
#endif /* __HAL_RCC_DAC12_FORCE_RESET */
#ifdef __HAL_RCC_DAC_RELEASE_RESET
		__HAL_RCC_DAC_RELEASE_RESET();
#endif /* __HAL_RCC_DAC_RELEASE_RESET */
#ifdef __HAL_RCC_DAC1_RELEASE_RESET
		__HAL_RCC_DAC1_RELEASE_RESET();
#endif /* __HAL_RCC_DAC1_RELEASE_RESET */
#ifdef __HAL_RCC_DAC12_RELEASE_RESET
		__HAL_RCC_DAC12_RELEASE_RESET();
#endif /* __HAL_RCC_DAC12_RELEASE_RESET */
#ifdef __HAL_RCC_DAC_CLK_DISABLE
		__HAL_RCC_DAC_CLK_DISABLE();
#endif /* __HAL_RCC_DAC_CLK_DISABLE */
#ifdef __HAL_RCC_DAC1_CLK_DISABLE
		__HAL_RCC_DAC1_CLK_DISABLE();
#endif /* __HAL_RCC_DAC1_CLK_DISABLE */
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
		__HAL_RCC_DAC12_CLK_ENABLE();
#endif /* __HAL_RCC_DAC12_CLK_ENABLE */
		break;
#endif /* DAC1 */
#ifdef DAC2 /* ****************************************** */
	case DAC2_BASE:
#ifdef __HAL_RCC_DAC2_FORCE_RESET
		__HAL_RCC_DAC2_FORCE_RESET();
#endif /* __HAL_RCC_DAC2_FORCE_RESET */
#ifdef __HAL_RCC_DAC12_FORCE_RESET
		__HAL_RCC_DAC12_FORCE_RESET();
#endif /* __HAL_RCC_DAC12_FORCE_RESET */
#ifdef __HAL_RCC_DAC2_RELEASE_RESET
		__HAL_RCC_DAC2_RELEASE_RESET();
#endif /* __HAL_RCC_DAC2_RELEASE_RESET */
#ifdef __HAL_RCC_DAC12_RELEASE_RESET
		__HAL_RCC_DAC12_RELEASE_RESET();
#endif /* __HAL_RCC_DAC12_RELEASE_RESET */
#ifdef __HAL_RCC_DAC2_CLK_ENABLE
		__HAL_RCC_DAC2_CLK_ENABLE();
#endif /* __HAL_RCC_DAC2_CLK_ENABLE */
#ifdef __HAL_RCC_DAC12_CLK_ENABLE
		__HAL_RCC_DAC12_CLK_ENABLE();
#endif /* __HAL_RCC_DAC12_CLK_ENABLE */
		break;
#endif /* DAC2 */
#ifdef DAC3 /* ****************************************** */
	case DAC3_BASE:
#ifdef __HAL_RCC_DAC3_FORCE_RESET
		__HAL_RCC_DAC3_FORCE_RESET();
#endif /* __HAL_RCC_DAC3_FORCE_RESET */
#ifdef __HAL_RCC_DAC3_RELEASE_RESET
		__HAL_RCC_DAC3_RELEASE_RESET();
#endif /* __HAL_RCC_DAC3_RELEASE_RESET */
#ifdef __HAL_RCC_DAC3_CLK_DISABLE
		__HAL_RCC_DAC3_CLK_DISABLE();
#endif /* __HAL_RCC_DAC3_CLK_DISABLE */
		break;
#endif /* DAC3 */
#ifdef DAC4 /* ****************************************** */
	case DAC4_BASE:
#ifdef __HAL_RCC_DAC4_FORCE_RESET
		__HAL_RCC_DAC4_FORCE_RESET();
#endif /* __HAL_RCC_DAC4_FORCE_RESET */
#ifdef __HAL_RCC_DAC4_RELEASE_RESET
		__HAL_RCC_DAC4_RELEASE_RESET();
#endif /* __HAL_RCC_DAC4_RELEASE_RESET */
#ifdef __HAL_RCC_DAC4_CLK_DISABLE
		__HAL_RCC_DAC4_CLK_DISABLE();
#endif /* __HAL_RCC_DAC4_CLK_DISABLE */
		break;
#endif /* DAC4 */
	default: break;
	}
}
#endif /* DAC_TRIGGER_NONE */


} /* extern "C" */
