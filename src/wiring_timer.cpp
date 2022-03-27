/**
 * @file wiring_timer.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-10-21
 * @version 2022-03-25
 */
#include "Arduino.h"
#include "scdinternal/macro.h"
#include "wiring_private.h"


#ifdef LL_RCC_CALC_SYSCLK_FREQ
#define _GetClockFreqOfHCLK() RCC_GetHCLKClockFreq(LL_RCC_CALC_SYSCLK_FREQ(RCC_GetSystemClockFreq(), LL_RCC_GetSysPrescaler()))
#else /* not LL_RCC_CALC_SYSCLK_FREQ */
#define _GetClockFreqOfHCLK() RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq())
#endif /* not LL_RCC_CALC_SYSCLK_FREQ */


/* exported functions */
extern GPIO_TypeDef * enableGpioClock(const PortName port);


/* C++ SFINAE for handling the absence and all variances of HAL_RCC_GetPCLK1Freq() accordingly */
DEF_NO_FN_WRAPPER(uint32_t, HAL_RCC_GetPCLK1Freq_Wrapper, HAL_RCC_GetPCLK1Freq, 0)

/* C++ SFINAE for handling the absence and all variances of HAL_RCC_GetPCLK2Freq() accordingly */
DEF_NO_FN_WRAPPER(uint32_t, HAL_RCC_GetPCLK2Freq_Wrapper, HAL_RCC_GetPCLK2Freq, 0)

/* C++ SFINAE for handling the absence and all variances of HAL_RCC_GetPCLK3Freq() accordingly */
DEF_NO_FN_WRAPPER(uint32_t, HAL_RCC_GetPCLK3Freq_Wrapper, HAL_RCC_GetPCLK3Freq, 0)

/* C++ SFINAE for handling the absence and all variances of HAL_RCC_GetPCLK4Freq() accordingly */
DEF_NO_FN_WRAPPER(uint32_t, HAL_RCC_GetPCLK4Freq_Wrapper, HAL_RCC_GetPCLK4Freq, 0)


/* C++ SFINAE for handling the absence and all variances of RCC_GetPCLK1ClockFreq() accordingly */
#ifdef __LL_RCC_CALC_PCLK1_FREQ
template <typename ...Ts>
static inline auto RCC_GetPCLK1ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK1ClockFreq(_GetClockFreqOfHCLK(), args...)) {
	return RCC_GetPCLK1ClockFreq(_GetClockFreqOfHCLK(), args...);
}
#endif /* not __LL_RCC_CALC_PCLK1_FREQ */

template <typename ...Ts>
static inline auto RCC_GetPCLK1ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK1ClockFreq(args...)) {
	return RCC_GetPCLK1ClockFreq(args...);
}

template <typename ...Ts>
static inline auto RCC_GetPCLK1ClockFreq_Wrapper_(const float, Ts... args) -> uint32_t {
	return HAL_RCC_GetPCLK1Freq_Wrapper(); /* function does not exists -> use other fallback */
}

template <typename ...Ts>
static inline uint32_t RCC_GetPCLK1ClockFreq_Wrapper(Ts... args) {
	return RCC_GetPCLK1ClockFreq_Wrapper_(0 /* prefer int here */, args...);
}


/* C++ SFINAE for handling the absence and all variances of RCC_GetPCLK2ClockFreq() accordingly */
#ifdef __LL_RCC_CALC_PCLK2_FREQ
template <typename ...Ts>
static inline auto RCC_GetPCLK2ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK2ClockFreq(_GetClockFreqOfHCLK(), args...)) {
	return RCC_GetPCLK2ClockFreq(_GetClockFreqOfHCLK(), args...);
}
#endif /* not __LL_RCC_CALC_PCLK2_FREQ */

template <typename ...Ts>
static inline auto RCC_GetPCLK2ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK2ClockFreq(args...)) {
	return RCC_GetPCLK2ClockFreq(args...);
}

template <typename ...Ts>
static inline auto RCC_GetPCLK2ClockFreq_Wrapper_(const float, Ts... args) -> uint32_t {
	return HAL_RCC_GetPCLK2Freq_Wrapper(); /* function does not exists -> use other fallback */
}

template <typename ...Ts>
static inline uint32_t RCC_GetPCLK2ClockFreq_Wrapper(Ts... args) {
	return RCC_GetPCLK2ClockFreq_Wrapper_(0 /* prefer int here */, args...);
}


/* C++ SFINAE for handling the absence and all variances of RCC_GetPCLK3ClockFreq() accordingly */
#ifdef __LL_RCC_CALC_PCLK3_FREQ
template <typename ...Ts>
static inline auto RCC_GetPCLK3ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK3ClockFreq(_GetClockFreqOfHCLK(), args...)) {
	return RCC_GetPCLK3ClockFreq(_GetClockFreqOfHCLK(), args...);
}
#endif /* not __LL_RCC_CALC_PCLK3_FREQ */

template <typename ...Ts>
static inline auto RCC_GetPCLK3ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK3ClockFreq(args...)) {
	return RCC_GetPCLK3ClockFreq(args...);
}

template <typename ...Ts>
static inline auto RCC_GetPCLK3ClockFreq_Wrapper_(const float, Ts... args) -> uint32_t {
	return HAL_RCC_GetPCLK3Freq_Wrapper(); /* function does not exists -> use other fallback */
}

template <typename ...Ts>
static inline uint32_t RCC_GetPCLK3ClockFreq_Wrapper(Ts... args) {
	return RCC_GetPCLK3ClockFreq_Wrapper_(0 /* prefer int here */, args...);
}


/* C++ SFINAE for handling the absence and all variances of RCC_GetPCLK4ClockFreq() accordingly */
#ifdef __LL_RCC_CALC_PCLK4_FREQ
template <typename ...Ts>
static inline auto RCC_GetPCLK4ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK4ClockFreq(_GetClockFreqOfHCLK(), args...)) {
	return RCC_GetPCLK4ClockFreq(_GetClockFreqOfHCLK(), args...);
}
#endif /* not __LL_RCC_CALC_PCLK4_FREQ */

template <typename ...Ts>
static inline auto RCC_GetPCLK4ClockFreq_Wrapper_(const int, Ts... args) -> decltype(RCC_GetPCLK4ClockFreq(args...)) {
	return RCC_GetPCLK4ClockFreq(args...);
}

template <typename ...Ts>
static inline auto RCC_GetPCLK4ClockFreq_Wrapper_(const float, Ts... args) -> uint32_t {
	return HAL_RCC_GetPCLK4Freq_Wrapper(); /* function does not exists -> use other fallback */
}

template <typename ...Ts>
static inline uint32_t RCC_GetPCLK4ClockFreq_Wrapper(Ts... args) {
	return RCC_GetPCLK4ClockFreq_Wrapper_(0 /* prefer int here */, args...);
}


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
#ifdef TIM1
_TimerPinMap::TimHandle tim1Handle[1] = {0};
#endif /* TIM1 */
#ifdef TIM2
_TimerPinMap::TimHandle tim2Handle[1] = {0};
#endif /* TIM2 */
#ifdef TIM3
_TimerPinMap::TimHandle tim3Handle[1] = {0};
#endif /* TIM3 */
#ifdef TIM4
_TimerPinMap::TimHandle tim4Handle[1] = {0};
#endif /* TIM4 */
#ifdef TIM5
_TimerPinMap::TimHandle tim5Handle[1] = {0};
#endif /* TIM5 */
#ifdef TIM6
_TimerPinMap::TimHandle tim6Handle[1] = {0};
#endif /* TIM6 */
#ifdef TIM7
_TimerPinMap::TimHandle tim7Handle[1] = {0};
#endif /* TIM7 */
#ifdef TIM8
_TimerPinMap::TimHandle tim8Handle[1] = {0};
#endif /* TIM8 */
#ifdef TIM9
_TimerPinMap::TimHandle tim9Handle[1] = {0};
#endif /* TIM9 */
#ifdef TIM10
_TimerPinMap::TimHandle tim10Handle[1] = {0};
#endif /* TIM10 */
#ifdef TIM11
_TimerPinMap::TimHandle tim11Handle[1] = {0};
#endif /* TIM11 */
#ifdef TIM12
_TimerPinMap::TimHandle tim12Handle[1] = {0};
#endif /* TIM12 */
#ifdef TIM13
_TimerPinMap::TimHandle tim13Handle[1] = {0};
#endif /* TIM13 */
#ifdef TIM14
_TimerPinMap::TimHandle tim14Handle[1] = {0};
#endif /* TIM14 */
#ifdef TIM15
_TimerPinMap::TimHandle tim15Handle[1] = {0};
#endif /* TIM15 */
#ifdef TIM16
_TimerPinMap::TimHandle tim16Handle[1] = {0};
#endif /* TIM16 */
#ifdef TIM17
_TimerPinMap::TimHandle tim17Handle[1] = {0};
#endif /* TIM17 */
#ifdef TIM18
_TimerPinMap::TimHandle tim18Handle[1] = {0};
#endif /* TIM18 */
#ifdef TIM19
_TimerPinMap::TimHandle tim19Handle[1] = {0};
#endif /* TIM19 */
#ifdef TIM20
_TimerPinMap::TimHandle tim20Handle[1] = {0};
#endif /* TIM20 */
#ifdef TIM21
_TimerPinMap::TimHandle tim21Handle[1] = {0};
#endif /* TIM21 */
#ifdef TIM22
_TimerPinMap::TimHandle tim22Handle[1] = {0};
#endif /* TIM22 */
#ifdef TIM23
_TimerPinMap::TimHandle tim23Handle[1] = {0};
#endif /* TIM23 */
#ifdef TIM24
_TimerPinMap::TimHandle tim24Handle[1] = {0};
#endif /* TIM24 */
#endif /* TIM_CLOCKPRESCALER_DIV1 */

#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
#ifdef LPTIM1
_TimerPinMap::LpTimHandle lpTim1Handle[1] = {0};
#endif /* LPTIM1 */
#ifdef LPTIM2
_TimerPinMap::LpTimHandle lpTim2Handle[1] = {0};
#endif /* LPTIM2 */
#ifdef LPTIM3
_TimerPinMap::LpTimHandle lpTim3Handle[1] = {0};
#endif /* LPTIM3 */
#ifdef LPTIM4
_TimerPinMap::LpTimHandle lpTim4Handle[1] = {0};
#endif /* LPTIM4 */
#ifdef LPTIM5
_TimerPinMap::LpTimHandle lpTim5Handle[1] = {0};
#endif /* LPTIM5 */
#endif /* LPTIM_PRESCALER_DIV1 */


__attribute__((weak))
_TimerPinMap::_TimerPinMap(const uint32_t pin) {
	/* This enables the compiler to create a compact mapping table. */
	uint16_t res = 0;
	switch (pin) {
#if defined(STM32F030C6Tx) || defined(STM32F030C8Tx) || defined(STM32F030CCTx) || defined(STM32F030F4Px) || defined(STM32F030K6Tx) || defined(STM32F030R8Tx) || defined(STM32F030RCTx) || defined(STM32F070C6Tx) || defined(STM32F070CBTx) || defined(STM32F070F6Px) || defined(STM32F070RBTx)
	case PA_1:  res = _TimerPinMap{15, 0, 1,  5}.value; break; /* TIM15_CH1N/GPIO_AF5 */
	case PA_2:  res = _TimerPinMap{15, 0, 0,  0}.value; break; /* TIM15_CH1/GPIO_AF0 */
	case PA_3:  res = _TimerPinMap{15, 1, 0,  0}.value; break; /* TIM15_CH2/GPIO_AF0 */
	case PA_4:  res = _TimerPinMap{14, 0, 0,  4}.value; break; /* TIM14_CH1/GPIO_AF4 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{16, 0, 1,  2}.value; break; /* TIM16_CH1N/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  2}.value; break; /* TIM17_CH1N/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/GPIO_AF0 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  0}.value; break; /* TIM3_CH2/GPIO_AF0 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  0}.value; break; /* TIM3_CH3/GPIO_AF0 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  0}.value; break; /* TIM3_CH4/GPIO_AF0 */
#endif /* GPIOC_BASE */
#elif defined(STM32F031C4Tx) || defined(STM32F031C6Tx) || defined(STM32F031E6Yx) || defined(STM32F031F4Px) || defined(STM32F031F6Px) || defined(STM32F031G4Ux) || defined(STM32F031G6Ux) || defined(STM32F031K4Ux) || defined(STM32F031K6Ux) || defined(STM32F031K6Tx) || defined(STM32F038C6Tx) || defined(STM32F038E6Yx) || defined(STM32F038F6Px) || defined(STM32F038G6Ux) || defined(STM32F038K6Ux) || defined(STM32F042C4Tx) || defined(STM32F042C6Tx) || defined(STM32F042C4Ux) || defined(STM32F042C6Ux) || defined(STM32F042F4Px) || defined(STM32F042F6Px) || defined(STM32F042G4Ux) || defined(STM32F042G6Ux) || defined(STM32F042K4Tx) || defined(STM32F042K6Tx) || defined(STM32F042K4Ux) || defined(STM32F042K6Ux) || defined(STM32F042T6Yx) || defined(STM32F048C6Ux) || defined(STM32F048G6Ux) || defined(STM32F048T6Yx) || defined(STM32F051C4Tx) || defined(STM32F051C4Ux) || defined(STM32F051C6Tx) || defined(STM32F051C6Ux) || defined(STM32F051C8Tx) || defined(STM32F051C8Ux) || defined(STM32F051K4Tx) || defined(STM32F051K4Ux) || defined(STM32F051K6Tx) || defined(STM32F051K6Ux) || defined(STM32F051K8Tx) || defined(STM32F051K8Ux) || defined(STM32F051R4Tx) || defined(STM32F051R6Tx) || defined(STM32F051R8Hx) || defined(STM32F051R8Tx) || defined(STM32F051T8Yx) || defined(STM32F058C8Ux) || defined(STM32F058R8Hx) || defined(STM32F058R8Tx) || defined(STM32F058T8Yx) || defined(STM32F071C8Tx) || defined(STM32F071CBTx) || defined(STM32F071C8Ux) || defined(STM32F071CBUx) || defined(STM32F071CBYx) || defined(STM32F071RBTx) || defined(STM32F071V8Hx) || defined(STM32F071VBHx) || defined(STM32F071V8Tx) || defined(STM32F071VBTx) || defined(STM32F072C8Tx) || defined(STM32F072CBTx) || defined(STM32F072C8Ux) || defined(STM32F072CBUx) || defined(STM32F072CBYx) || defined(STM32F072R8Tx) || defined(STM32F072RBTx) || defined(STM32F072RBHx) || defined(STM32F072RBIx) || defined(STM32F072V8Hx) || defined(STM32F072VBHx) || defined(STM32F072V8Tx) || defined(STM32F072VBTx) || defined(STM32F078CBTx) || defined(STM32F078CBUx) || defined(STM32F078CBYx) || defined(STM32F078RBHx) || defined(STM32F078RBTx) || defined(STM32F078VBHx) || defined(STM32F078VBTx) || defined(STM32F091CBTx) || defined(STM32F091CCTx) || defined(STM32F091CBUx) || defined(STM32F091CCUx) || defined(STM32F091RBTx) || defined(STM32F091RCTx) || defined(STM32F091RCHx) || defined(STM32F091RCYx) || defined(STM32F091VBTx) || defined(STM32F091VCTx) || defined(STM32F091VCHx) || defined(STM32F098CCTx) || defined(STM32F098CCUx) || defined(STM32F098RCHx) || defined(STM32F098RCTx) || defined(STM32F098RCYx) || defined(STM32F098VCHx) || defined(STM32F098VCTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_4:  res = _TimerPinMap{14, 0, 0,  4}.value; break; /* TIM14_CH1/GPIO_AF4 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{16, 0, 1,  2}.value; break; /* TIM16_CH1N/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  2}.value; break; /* TIM17_CH1N/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/GPIO_AF0 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  0}.value; break; /* TIM3_CH2/GPIO_AF0 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  0}.value; break; /* TIM3_CH3/GPIO_AF0 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  0}.value; break; /* TIM3_CH4/GPIO_AF0 */
#endif /* GPIOC_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0,  0}.value; break; /* TIM16_CH1/GPIO_AF0 */
	case PE_1:  res = _TimerPinMap{17, 0, 0,  0}.value; break; /* TIM17_CH1/GPIO_AF0 */
	case PE_3:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/GPIO_AF0 */
	case PE_4:  res = _TimerPinMap{ 3, 1, 0,  0}.value; break; /* TIM3_CH2/GPIO_AF0 */
	case PE_5:  res = _TimerPinMap{ 3, 2, 0,  0}.value; break; /* TIM3_CH3/GPIO_AF0 */
	case PE_6:  res = _TimerPinMap{ 3, 3, 0,  0}.value; break; /* TIM3_CH4/GPIO_AF0 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  0}.value; break; /* TIM1_CH1N/GPIO_AF0 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  0}.value; break; /* TIM1_CH1/GPIO_AF0 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  0}.value; break; /* TIM1_CH2N/GPIO_AF0 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  0}.value; break; /* TIM1_CH2/GPIO_AF0 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  0}.value; break; /* TIM1_CH3N/GPIO_AF0 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  0}.value; break; /* TIM1_CH3/GPIO_AF0 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  0}.value; break; /* TIM1_CH4/GPIO_AF0 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_9:  res = _TimerPinMap{15, 0, 0,  0}.value; break; /* TIM15_CH1/GPIO_AF0 */
	case PF_10: res = _TimerPinMap{15, 1, 0,  0}.value; break; /* TIM15_CH2/GPIO_AF0 */
#endif /* GPIOF_BASE */
#elif defined(STM32F100C4Tx) || defined(STM32F100C6Tx) || defined(STM32F100R4Hx) || defined(STM32F100R6Hx) || defined(STM32F100R4Tx) || defined(STM32F100R6Tx) || defined(STM32F103C4Tx) || defined(STM32F103C6Tx) || defined(STM32F103C6Ux) || defined(STM32F103R4Hx) || defined(STM32F103R6Hx) || defined(STM32F103R4Tx) || defined(STM32F103R6Tx) || defined(STM32F103T4Ux) || defined(STM32F103T6Ux)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  4}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  4}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  3}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  3}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/default */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  3}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  3}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  6}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  6}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_6:  res = _TimerPinMap{16, 0, 1,  0}.value; break; /* TIM16_CH1N/default */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  0}.value; break; /* TIM17_CH1N/default */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  0}.value; break; /* TIM16_CH1/default */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  0}.value; break; /* TIM17_CH1/default */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  4}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  4}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  0}.value; break; /* TIM1_CH1N/default */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  0}.value; break; /* TIM1_CH2N/default */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  0}.value; break; /* TIM1_CH3N/default */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  5}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  5}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  5}.value; break; /* TIM3_CH3/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  5}.value; break; /* TIM3_CH4/__HAL_AFIO_REMAP_TIM3_ENABLE */
#endif /* GPIOC_BASE */
#elif defined(STM32F100C8Tx) || defined(STM32F100CBTx) || defined(STM32F100R8Hx) || defined(STM32F100RBHx) || defined(STM32F100R8Tx) || defined(STM32F100RBTx) || defined(STM32F100RCTx) || defined(STM32F100RDTx) || defined(STM32F100RETx) || defined(STM32F100V8Tx) || defined(STM32F100VBTx) || defined(STM32F100VCTx) || defined(STM32F100VDTx) || defined(STM32F100VETx) || defined(STM32F100ZCTx) || defined(STM32F100ZDTx) || defined(STM32F100ZETx) || defined(STM32F103C8Tx) || defined(STM32F103CBTx) || defined(STM32F103CBUx) || defined(STM32F103R8Hx) || defined(STM32F103RBHx) || defined(STM32F103R8Tx) || defined(STM32F103RBTx) || defined(STM32F103RCTx) || defined(STM32F103RDTx) || defined(STM32F103RETx) || defined(STM32F103RCYx) || defined(STM32F103RDYx) || defined(STM32F103REYx) || defined(STM32F103RFTx) || defined(STM32F103RGTx) || defined(STM32F103T8Ux) || defined(STM32F103TBUx) || defined(STM32F103V8Hx) || defined(STM32F103VBHx) || defined(STM32F103V8Tx) || defined(STM32F103VBTx) || defined(STM32F103VCHx) || defined(STM32F103VDHx) || defined(STM32F103VEHx) || defined(STM32F103VCTx) || defined(STM32F103VDTx) || defined(STM32F103VETx) || defined(STM32F103VFTx) || defined(STM32F103VGTx) || defined(STM32F103VBIx) || defined(STM32F103ZCHx) || defined(STM32F103ZDHx) || defined(STM32F103ZEHx) || defined(STM32F103ZCTx) || defined(STM32F103ZDTx) || defined(STM32F103ZETx) || defined(STM32F103ZFHx) || defined(STM32F103ZGHx) || defined(STM32F103ZFTx) || defined(STM32F103ZGTx) || defined(STM32F105R8Tx) || defined(STM32F105RBTx) || defined(STM32F105RCTx) || defined(STM32F105V8Hx) || defined(STM32F105VBHx) || defined(STM32F105V8Tx) || defined(STM32F105VBTx) || defined(STM32F105VCTx) || defined(STM32F107RBTx) || defined(STM32F107RCTx) || defined(STM32F107VBTx) || defined(STM32F107VCTx) || defined(STM32F107VCHx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  4}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  4}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  3}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  3}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/default */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  3}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/__HAL_AFIO_REMAP_TIM1_PARTIAL */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  3}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  6}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  6}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  0}.value; break; /* TIM4_CH1/default */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  0}.value; break; /* TIM4_CH2/default */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  0}.value; break; /* TIM4_CH3/default */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  0}.value; break; /* TIM4_CH4/default */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  4}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  4}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_12: res = _TimerPinMap{12, 0, 0, 11}.value; break; /* TIM12_CH1/__HAL_AFIO_REMAP_TIM12_ENABLE */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  0}.value; break; /* TIM1_CH1N/default */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  0}.value; break; /* TIM1_CH2N/default */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  0}.value; break; /* TIM1_CH3N/default */
#ifdef GPIOC_BASE
	case PC_4:  res = _TimerPinMap{12, 0, 0,  0}.value; break; /* TIM12_CH1/default */
	case PC_5:  res = _TimerPinMap{12, 1, 0,  0}.value; break; /* TIM12_CH2/default */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  5}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  5}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  5}.value; break; /* TIM3_CH3/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  5}.value; break; /* TIM3_CH4/__HAL_AFIO_REMAP_TIM3_ENABLE */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  7}.value; break; /* TIM4_CH1/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  7}.value; break; /* TIM4_CH2/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  7}.value; break; /* TIM4_CH3/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  7}.value; break; /* TIM4_CH4/__HAL_AFIO_REMAP_TIM4_ENABLE */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_5:  res = _TimerPinMap{ 9, 0, 0,  8}.value; break; /* TIM9_CH1/__HAL_AFIO_REMAP_TIM9_ENABLE */
	case PE_6:  res = _TimerPinMap{ 9, 1, 0,  8}.value; break; /* TIM9_CH2/__HAL_AFIO_REMAP_TIM9_ENABLE */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/__HAL_AFIO_REMAP_TIM1_ENABLE */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/__HAL_AFIO_REMAP_TIM1_ENABLE */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{10, 0, 0,  9}.value; break; /* TIM10_CH1/__HAL_AFIO_REMAP_TIM10_ENABLE */
	case PF_7:  res = _TimerPinMap{11, 0, 0, 10}.value; break; /* TIM11_CH1/__HAL_AFIO_REMAP_TIM11_ENABLE */
	case PF_8:  res = _TimerPinMap{13, 0, 0, 12}.value; break; /* TIM13_CH1/__HAL_AFIO_REMAP_TIM13_ENABLE */
	case PF_9:  res = _TimerPinMap{14, 0, 0, 13}.value; break; /* TIM14_CH1/__HAL_AFIO_REMAP_TIM14_ENABLE */
#endif /* GPIOF_BASE */
#elif defined(STM32F101C4Tx) || defined(STM32F101C6Tx) || defined(STM32F101C8Tx) || defined(STM32F101CBTx) || defined(STM32F101C8Ux) || defined(STM32F101CBUx) || defined(STM32F101R4Tx) || defined(STM32F101R6Tx) || defined(STM32F101R8Tx) || defined(STM32F101RBTx) || defined(STM32F101RCTx) || defined(STM32F101RDTx) || defined(STM32F101RETx) || defined(STM32F101RFTx) || defined(STM32F101RGTx) || defined(STM32F101RBHx) || defined(STM32F101T4Ux) || defined(STM32F101T6Ux) || defined(STM32F101T8Ux) || defined(STM32F101TBUx) || defined(STM32F101V8Tx) || defined(STM32F101VBTx) || defined(STM32F101VCTx) || defined(STM32F101VDTx) || defined(STM32F101VETx) || defined(STM32F101VFTx) || defined(STM32F101VGTx) || defined(STM32F101ZCTx) || defined(STM32F101ZDTx) || defined(STM32F101ZETx) || defined(STM32F101ZFTx) || defined(STM32F101ZGTx) || defined(STM32F102C4Tx) || defined(STM32F102C6Tx) || defined(STM32F102C8Tx) || defined(STM32F102CBTx) || defined(STM32F102R4Tx) || defined(STM32F102R6Tx) || defined(STM32F102R8Tx) || defined(STM32F102RBTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  4}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  4}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  3}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  3}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  0}.value; break; /* TIM3_CH1/default */
	case PA_7:  res = _TimerPinMap{ 3, 1, 0,  0}.value; break; /* TIM3_CH2/default */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  3}.value; break; /* TIM2_CH1/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_0:  res = _TimerPinMap{ 3, 2, 0,  6}.value; break; /* TIM3_CH3/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_1:  res = _TimerPinMap{ 3, 3, 0,  6}.value; break; /* TIM3_CH4/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  3}.value; break; /* TIM2_CH2/__HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  6}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  6}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_PARTIAL */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  0}.value; break; /* TIM4_CH1/default */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  0}.value; break; /* TIM4_CH2/default */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  0}.value; break; /* TIM4_CH3/default */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  0}.value; break; /* TIM4_CH4/default */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  4}.value; break; /* TIM2_CH3/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  4}.value; break; /* TIM2_CH4/__HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
	case PB_14: res = _TimerPinMap{12, 0, 0,  0}.value; break; /* TIM12_CH1/default */
	case PB_15: res = _TimerPinMap{12, 1, 0,  0}.value; break; /* TIM12_CH2/default */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  5}.value; break; /* TIM3_CH1/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  5}.value; break; /* TIM3_CH2/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  5}.value; break; /* TIM3_CH3/__HAL_AFIO_REMAP_TIM3_ENABLE */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  5}.value; break; /* TIM3_CH4/__HAL_AFIO_REMAP_TIM3_ENABLE */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  7}.value; break; /* TIM4_CH1/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  7}.value; break; /* TIM4_CH2/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  7}.value; break; /* TIM4_CH3/__HAL_AFIO_REMAP_TIM4_ENABLE */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  7}.value; break; /* TIM4_CH4/__HAL_AFIO_REMAP_TIM4_ENABLE */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_5:  res = _TimerPinMap{ 9, 0, 0,  8}.value; break; /* TIM9_CH1/__HAL_AFIO_REMAP_TIM9_ENABLE */
	case PE_6:  res = _TimerPinMap{ 9, 1, 0,  8}.value; break; /* TIM9_CH2/__HAL_AFIO_REMAP_TIM9_ENABLE */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{10, 0, 0,  9}.value; break; /* TIM10_CH1/__HAL_AFIO_REMAP_TIM10_ENABLE */
	case PF_7:  res = _TimerPinMap{11, 0, 0, 10}.value; break; /* TIM11_CH1/__HAL_AFIO_REMAP_TIM11_ENABLE */
	case PF_8:  res = _TimerPinMap{13, 0, 0, 12}.value; break; /* TIM13_CH1/__HAL_AFIO_REMAP_TIM13_ENABLE */
	case PF_9:  res = _TimerPinMap{14, 0, 0, 13}.value; break; /* TIM14_CH1/__HAL_AFIO_REMAP_TIM14_ENABLE */
#endif /* GPIOF_BASE */
#elif defined(STM32F205RBTx) || defined(STM32F205RCTx) || defined(STM32F205RETx) || defined(STM32F205RFTx) || defined(STM32F205RGTx) || defined(STM32F205REYx) || defined(STM32F205RGYx) || defined(STM32F205RGEx) || defined(STM32F205VBTx) || defined(STM32F205VCTx) || defined(STM32F205VETx) || defined(STM32F205VFTx) || defined(STM32F205VGTx) || defined(STM32F205ZCTx) || defined(STM32F205ZETx) || defined(STM32F205ZFTx) || defined(STM32F205ZGTx) || defined(STM32F207ICHx) || defined(STM32F207IEHx) || defined(STM32F207IFHx) || defined(STM32F207IGHx) || defined(STM32F207ICTx) || defined(STM32F207IETx) || defined(STM32F207IFTx) || defined(STM32F207IGTx) || defined(STM32F207VCTx) || defined(STM32F207VETx) || defined(STM32F207VFTx) || defined(STM32F207VGTx) || defined(STM32F207ZCTx) || defined(STM32F207ZETx) || defined(STM32F207ZFTx) || defined(STM32F207ZGTx) || defined(STM32F215RETx) || defined(STM32F215RGTx) || defined(STM32F215VETx) || defined(STM32F215VGTx) || defined(STM32F215ZETx) || defined(STM32F215ZGTx) || defined(STM32F217IEHx) || defined(STM32F217IGHx) || defined(STM32F217IETx) || defined(STM32F217IGTx) || defined(STM32F217VETx) || defined(STM32F217VGTx) || defined(STM32F217ZETx) || defined(STM32F217ZGTx) || defined(STM32F401CBUx) || defined(STM32F401CCUx) || defined(STM32F401CBYx) || defined(STM32F401CCYx) || defined(STM32F401CDUx) || defined(STM32F401CEUx) || defined(STM32F401CDYx) || defined(STM32F401CEYx) || defined(STM32F401CCFx) || defined(STM32F401RBTx) || defined(STM32F401RCTx) || defined(STM32F401RDTx) || defined(STM32F401RETx) || defined(STM32F401VBHx) || defined(STM32F401VCHx) || defined(STM32F401VBTx) || defined(STM32F401VCTx) || defined(STM32F401VDHx) || defined(STM32F401VEHx) || defined(STM32F401VDTx) || defined(STM32F401VETx) || defined(STM32F405OEYx) || defined(STM32F405OGYx) || defined(STM32F405RGTx) || defined(STM32F405VGTx) || defined(STM32F405ZGTx) || defined(STM32F407IEHx) || defined(STM32F407IGHx) || defined(STM32F407IETx) || defined(STM32F407IGTx) || defined(STM32F407VETx) || defined(STM32F407VGTx) || defined(STM32F407ZETx) || defined(STM32F407ZGTx) || defined(STM32F411CCUx) || defined(STM32F411CEUx) || defined(STM32F411CCYx) || defined(STM32F411CEYx) || defined(STM32F411RCTx) || defined(STM32F411RETx) || defined(STM32F411VCHx) || defined(STM32F411VEHx) || defined(STM32F411VCTx) || defined(STM32F411VETx) || defined(STM32F412CEUx) || defined(STM32F412CGUx) || defined(STM32F412RETx) || defined(STM32F412RGTx) || defined(STM32F412REYx) || defined(STM32F412RGYx) || defined(STM32F412REYxP) || defined(STM32F412RGYxP) || defined(STM32F412VEHx) || defined(STM32F412VGHx) || defined(STM32F412VETx) || defined(STM32F412VGTx) || defined(STM32F412ZEJx) || defined(STM32F412ZGJx) || defined(STM32F412ZETx) || defined(STM32F412ZGTx) || defined(STM32F413CGUx) || defined(STM32F413CHUx) || defined(STM32F413MGYx) || defined(STM32F413MHYx) || defined(STM32F413RGTx) || defined(STM32F413RHTx) || defined(STM32F413VGHx) || defined(STM32F413VHHx) || defined(STM32F413VGTx) || defined(STM32F413VHTx) || defined(STM32F413ZGJx) || defined(STM32F413ZHJx) || defined(STM32F413ZGTx) || defined(STM32F413ZHTx) || defined(STM32F415OGYx) || defined(STM32F415RGTx) || defined(STM32F415VGTx) || defined(STM32F415ZGTx) || defined(STM32F417IEHx) || defined(STM32F417IGHx) || defined(STM32F417IETx) || defined(STM32F417IGTx) || defined(STM32F417VETx) || defined(STM32F417VGTx) || defined(STM32F417ZETx) || defined(STM32F417ZGTx) || defined(STM32F423CHUx) || defined(STM32F423MHYx) || defined(STM32F423RHTx) || defined(STM32F423VHHx) || defined(STM32F423VHTx) || defined(STM32F423ZHJx) || defined(STM32F423ZHTx) || defined(STM32F427AGHx) || defined(STM32F427AIHx) || defined(STM32F427IGHx) || defined(STM32F427IIHx) || defined(STM32F427IGTx) || defined(STM32F427IITx) || defined(STM32F427VGTx) || defined(STM32F427VITx) || defined(STM32F427ZGTx) || defined(STM32F427ZITx) || defined(STM32F429AGHx) || defined(STM32F429AIHx) || defined(STM32F429BETx) || defined(STM32F429BGTx) || defined(STM32F429BITx) || defined(STM32F429IETx) || defined(STM32F429IGTx) || defined(STM32F429IEHx) || defined(STM32F429IGHx) || defined(STM32F429IIHx) || defined(STM32F429IITx) || defined(STM32F429NEHx) || defined(STM32F429NGHx) || defined(STM32F429NIHx) || defined(STM32F429VETx) || defined(STM32F429VGTx) || defined(STM32F429VITx) || defined(STM32F429ZETx) || defined(STM32F429ZGTx) || defined(STM32F429ZGYx) || defined(STM32F429ZITx) || defined(STM32F429ZIYx) || defined(STM32F437AIHx) || defined(STM32F437IGHx) || defined(STM32F437IIHx) || defined(STM32F437IGTx) || defined(STM32F437IITx) || defined(STM32F437VGTx) || defined(STM32F437VITx) || defined(STM32F437ZGTx) || defined(STM32F437ZITx) || defined(STM32F439AIHx) || defined(STM32F439BGTx) || defined(STM32F439BITx) || defined(STM32F439IGHx) || defined(STM32F439IIHx) || defined(STM32F439IGTx) || defined(STM32F439IITx) || defined(STM32F439NGHx) || defined(STM32F439NIHx) || defined(STM32F439VGTx) || defined(STM32F439VITx) || defined(STM32F439ZGTx) || defined(STM32F439ZITx) || defined(STM32F439ZGYx) || defined(STM32F439ZIYx) || defined(STM32F469AEHx) || defined(STM32F469AGHx) || defined(STM32F469AIHx) || defined(STM32F469AEYx) || defined(STM32F469AGYx) || defined(STM32F469AIYx) || defined(STM32F469BETx) || defined(STM32F469BGTx) || defined(STM32F469BITx) || defined(STM32F469IETx) || defined(STM32F469IGTx) || defined(STM32F469IEHx) || defined(STM32F469IGHx) || defined(STM32F469IIHx) || defined(STM32F469IITx) || defined(STM32F469NEHx) || defined(STM32F469NGHx) || defined(STM32F469NIHx) || defined(STM32F469VETx) || defined(STM32F469VGTx) || defined(STM32F469VITx) || defined(STM32F469ZETx) || defined(STM32F469ZGTx) || defined(STM32F469ZITx) || defined(STM32F479AGHx) || defined(STM32F479AIHx) || defined(STM32F479AGYx) || defined(STM32F479AIYx) || defined(STM32F479BGTx) || defined(STM32F479BITx) || defined(STM32F479IGHx) || defined(STM32F479IIHx) || defined(STM32F479IGTx) || defined(STM32F479IITx) || defined(STM32F479NGHx) || defined(STM32F479NIHx) || defined(STM32F479VGTx) || defined(STM32F479VITx) || defined(STM32F479ZGTx) || defined(STM32F479ZITx) || defined(STM32F722ICKx) || defined(STM32F722IEKx) || defined(STM32F722ICTx) || defined(STM32F722IETx) || defined(STM32F722RCTx) || defined(STM32F722RETx) || defined(STM32F722VCTx) || defined(STM32F722VETx) || defined(STM32F722ZCTx) || defined(STM32F722ZETx) || defined(STM32F723ICKx) || defined(STM32F723IEKx) || defined(STM32F723ICTx) || defined(STM32F723IETx) || defined(STM32F723VCTx) || defined(STM32F723VETx) || defined(STM32F723VCYx) || defined(STM32F723VEYx) || defined(STM32F723ZCIx) || defined(STM32F723ZEIx) || defined(STM32F723ZCTx) || defined(STM32F723ZETx) || defined(STM32F730I8Kx) || defined(STM32F730R8Tx) || defined(STM32F730V8Tx) || defined(STM32F730Z8Tx) || defined(STM32F732IEKx) || defined(STM32F732IETx) || defined(STM32F732RETx) || defined(STM32F732VETx) || defined(STM32F732ZETx) || defined(STM32F733IEKx) || defined(STM32F733IETx) || defined(STM32F733VETx) || defined(STM32F733VEYx) || defined(STM32F733ZEIx) || defined(STM32F733ZETx) || defined(STM32F745IEKx) || defined(STM32F745IGKx) || defined(STM32F745IETx) || defined(STM32F745IGTx) || defined(STM32F745VEHx) || defined(STM32F745VGHx) || defined(STM32F745VETx) || defined(STM32F745VGTx) || defined(STM32F745ZETx) || defined(STM32F745ZGTx) || defined(STM32F746BETx) || defined(STM32F746BGTx) || defined(STM32F746IEKx) || defined(STM32F746IGKx) || defined(STM32F746IETx) || defined(STM32F746IGTx) || defined(STM32F746NEHx) || defined(STM32F746NGHx) || defined(STM32F746VEHx) || defined(STM32F746VGHx) || defined(STM32F746VETx) || defined(STM32F746VGTx) || defined(STM32F746ZEYx) || defined(STM32F746ZGYx) || defined(STM32F746ZETx) || defined(STM32F746ZGTx) || defined(STM32F750N8Hx) || defined(STM32F750V8Tx) || defined(STM32F750Z8Tx) || defined(STM32F756BGTx) || defined(STM32F756IGKx) || defined(STM32F756IGTx) || defined(STM32F756NGHx) || defined(STM32F756VGHx) || defined(STM32F756VGTx) || defined(STM32F756ZGTx) || defined(STM32F756ZGYx) || defined(STM32F765BGTx) || defined(STM32F765BITx) || defined(STM32F765IGKx) || defined(STM32F765IIKx) || defined(STM32F765IGTx) || defined(STM32F765IITx) || defined(STM32F765NGHx) || defined(STM32F765NIHx) || defined(STM32F765VGHx) || defined(STM32F765VIHx) || defined(STM32F765VGTx) || defined(STM32F765VITx) || defined(STM32F765ZGTx) || defined(STM32F765ZITx) || defined(STM32F767BGTx) || defined(STM32F767BITx) || defined(STM32F767IGKx) || defined(STM32F767IIKx) || defined(STM32F767IGTx) || defined(STM32F767IITx) || defined(STM32F767NGHx) || defined(STM32F767NIHx) || defined(STM32F767VGHx) || defined(STM32F767VGTx) || defined(STM32F767VIHx) || defined(STM32F767VITx) || defined(STM32F767ZGTx) || defined(STM32F767ZITx) || defined(STM32F768AIYx) || defined(STM32F769AGYx) || defined(STM32F769AIYx) || defined(STM32F769BGTx) || defined(STM32F769BITx) || defined(STM32F769IGTx) || defined(STM32F769IITx) || defined(STM32F769NGHx) || defined(STM32F769NIHx) || defined(STM32F777BITx) || defined(STM32F777IIKx) || defined(STM32F777IITx) || defined(STM32F777NIHx) || defined(STM32F777VIHx) || defined(STM32F777VITx) || defined(STM32F777ZITx) || defined(STM32F778AIYx) || defined(STM32F779AIYx) || defined(STM32F779BITx) || defined(STM32F779IITx) || defined(STM32F779NIHx) || defined(STM32H725REVx) || defined(STM32H725RGVx) || defined(STM32H725VGYx) || defined(STM32H735RGVx) || defined(STM32H735VGYx) || defined(STM32H7A3RGTx) || defined(STM32H7A3RITx) || defined(STM32H7B0RBTx) || defined(STM32H7B3RITx) || defined(STM32L471RETx) || defined(STM32L471RGTx) || defined(STM32L475RCTx) || defined(STM32L475RETx) || defined(STM32L475RGTx) || defined(STM32L476JEYx) || defined(STM32L476JGYx) || defined(STM32L476JGYxP) || defined(STM32L476MEYx) || defined(STM32L476MGYx) || defined(STM32L476RCTx) || defined(STM32L476RETx) || defined(STM32L476RGTx) || defined(STM32L485JCYx) || defined(STM32L485JEYx) || defined(STM32L486JGYx) || defined(STM32L486RGTx) || defined(STM32L496RETx) || defined(STM32L496RGTx) || defined(STM32L496RGTxP) || defined(STM32L4A6RGTx) || defined(STM32L4A6RGTxP) || defined(STM32L4P5CGTx) || defined(STM32L4P5CETx) || defined(STM32L4P5CGUx) || defined(STM32L4P5CEUx) || defined(STM32L4P5CGTxP) || defined(STM32L4P5CGUxP) || defined(STM32L4P5RGTx) || defined(STM32L4P5RETx) || defined(STM32L4P5RGTxP) || defined(STM32L4Q5CGTx) || defined(STM32L4Q5CGUx) || defined(STM32L4Q5RGTx) || defined(STM32L552CCTx) || defined(STM32L552CETx) || defined(STM32L552CCUx) || defined(STM32L552CEUx) || defined(STM32L552CETxP) || defined(STM32L552CEUxP) || defined(STM32L552MEYxP) || defined(STM32L552MEYxQ) || defined(STM32L552RCTx) || defined(STM32L552RETx) || defined(STM32L552RETxP) || defined(STM32L552RETxQ) || defined(STM32L562CETx) || defined(STM32L562CETxP) || defined(STM32L562CEUx) || defined(STM32L562CEUxP) || defined(STM32L562MEYxP) || defined(STM32L562MEYxQ) || defined(STM32L562RETx) || defined(STM32L562RETxP) || defined(STM32L562RETxQ)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PC_3:  res = _TimerPinMap{27, 0, 0,  2}.value; break; /* LPTIM3_OUT/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{15, 0, 0,  2}.value; break; /* TIM15_CH1/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_4:  res = _TimerPinMap{15, 0, 1,  4}.value; break; /* TIM15_CH1N/GPIO_AF4 */
	case PE_5:  res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PE_6:  res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_3:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PF_4:  res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PF_5:  res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PF_6:  res = _TimerPinMap{10, 0, 0,  3}.value; break; /* TIM10_CH1/GPIO_AF3 */
	case PF_7:  res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
	case PF_8:  res = _TimerPinMap{13, 0, 0,  9}.value; break; /* TIM13_CH1/GPIO_AF9 */
	case PF_9:  res = _TimerPinMap{14, 0, 0,  9}.value; break; /* TIM14_CH1/GPIO_AF9 */
	case PF_10: res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_9:  res = _TimerPinMap{15, 0, 1, 14}.value; break; /* TIM15_CH1N/GPIO_AF14 */
	case PG_10: res = _TimerPinMap{15, 0, 0, 14}.value; break; /* TIM15_CH1/GPIO_AF14 */
	case PG_11: res = _TimerPinMap{15, 1, 0, 14}.value; break; /* TIM15_CH2/GPIO_AF14 */
	case PG_13: res = _TimerPinMap{25, 0, 0,  3}.value; break; /* LPTIM1_OUT/GPIO_AF3 */
	case PG_15: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PH_6:  res = _TimerPinMap{12, 0, 0,  9}.value; break; /* TIM12_CH1/GPIO_AF9 */
	case PH_9:  res = _TimerPinMap{12, 1, 0,  9}.value; break; /* TIM12_CH2/GPIO_AF9 */
	case PH_10: res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PH_11: res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PH_12: res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PH_13: res = _TimerPinMap{ 8, 0, 1,  3}.value; break; /* TIM8_CH1N/GPIO_AF3 */
	case PH_14: res = _TimerPinMap{ 8, 1, 1,  3}.value; break; /* TIM8_CH2N/GPIO_AF3 */
	case PH_15: res = _TimerPinMap{ 8, 2, 1,  3}.value; break; /* TIM8_CH3N/GPIO_AF3 */
#endif /* GPIOH_BASE */
#ifdef GPIOI_BASE
	case PI_0:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PI_2:  res = _TimerPinMap{ 8, 3, 0,  3}.value; break; /* TIM8_CH4/GPIO_AF3 */
	case PI_5:  res = _TimerPinMap{ 8, 0, 0,  3}.value; break; /* TIM8_CH1/GPIO_AF3 */
	case PI_6:  res = _TimerPinMap{ 8, 1, 0,  3}.value; break; /* TIM8_CH2/GPIO_AF3 */
	case PI_7:  res = _TimerPinMap{ 8, 2, 0,  3}.value; break; /* TIM8_CH3/GPIO_AF3 */
#endif /* GPIOI_BASE */
#elif defined(STM32F301C6Tx) || defined(STM32F301C8Tx) || defined(STM32F301C8Yx) || defined(STM32F301K6Tx) || defined(STM32F301K8Tx) || defined(STM32F301K6Ux) || defined(STM32F301K8Ux) || defined(STM32F301R6Tx) || defined(STM32F301R8Tx) || defined(STM32F302C6Tx) || defined(STM32F302C8Tx) || defined(STM32F302C8Yx) || defined(STM32F302K6Ux) || defined(STM32F302K8Ux) || defined(STM32F302R6Tx) || defined(STM32F302R8Tx) || defined(STM32F318C8Tx) || defined(STM32F318C8Yx) || defined(STM32F318K8Ux)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{16, 0, 1,  1}.value; break; /* TIM16_CH1N/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{17, 0, 0, 10}.value; break; /* TIM17_CH1/GPIO_AF10 */
	case PB_6:  res = _TimerPinMap{16, 0, 1,  1}.value; break; /* TIM16_CH1N/GPIO_AF1 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  1}.value; break; /* TIM17_CH1N/GPIO_AF1 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  1}.value; break; /* TIM17_CH1/GPIO_AF1 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
#endif /* GPIOF_BASE */
#elif defined(STM32F302CBTx) || defined(STM32F302CCTx) || defined(STM32F302RBTx) || defined(STM32F302RCTx) || defined(STM32F302RDTx) || defined(STM32F302RETx) || defined(STM32F302VBTx) || defined(STM32F302VCTx) || defined(STM32F302VDHx) || defined(STM32F302VEHx) || defined(STM32F302VDTx) || defined(STM32F302VETx) || defined(STM32F302VCYx) || defined(STM32F302ZDTx) || defined(STM32F302ZETx) || defined(STM32F303CBTx) || defined(STM32F303CCTx) || defined(STM32F303RBTx) || defined(STM32F303RCTx) || defined(STM32F303RDTx) || defined(STM32F303RETx) || defined(STM32F303VBTx) || defined(STM32F303VCTx) || defined(STM32F303VDHx) || defined(STM32F303VEHx) || defined(STM32F303VDTx) || defined(STM32F303VETx) || defined(STM32F303VCYx) || defined(STM32F303VEYx) || defined(STM32F303ZDTx) || defined(STM32F303ZETx) || defined(STM32F358CCTx) || defined(STM32F358RCTx) || defined(STM32F358VCTx) || defined(STM32F398VETx) || defined(STM32G431K6Tx) || defined(STM32G431K8Tx) || defined(STM32G431KBTx) || defined(STM32G431K6Ux) || defined(STM32G431K8Ux) || defined(STM32G431KBUx) || defined(STM32G441KBTx) || defined(STM32G441KBUx) || defined(STM32G491KCUx) || defined(STM32G491KEUx) || defined(STM32G4A1KEUx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{ 4, 2, 0, 10}.value; break; /* TIM4_CH3/GPIO_AF10 */
	case PA_14: res = _TimerPinMap{ 8, 1, 0,  5}.value; break; /* TIM8_CH2/GPIO_AF5 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 8, 0, 1,  4}.value; break; /* TIM8_CH1N/GPIO_AF4 */
	case PC_11: res = _TimerPinMap{ 8, 1, 1,  4}.value; break; /* TIM8_CH2N/GPIO_AF4 */
	case PC_12: res = _TimerPinMap{ 8, 2, 1,  4}.value; break; /* TIM8_CH3N/GPIO_AF4 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_1:  res = _TimerPinMap{ 8, 3, 0,  4}.value; break; /* TIM8_CH4/GPIO_AF4 */
	case PD_3:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PD_6:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PD_7:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0,  4}.value; break; /* TIM16_CH1/GPIO_AF4 */
	case PE_1:  res = _TimerPinMap{17, 0, 0,  4}.value; break; /* TIM17_CH1/GPIO_AF4 */
	case PE_2:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_3:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{20, 2, 1,  6}.value; break; /* TIM20_CH3N/GPIO_AF6 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PF_2:  res = _TimerPinMap{20, 2, 0,  2}.value; break; /* TIM20_CH3/GPIO_AF2 */
	case PF_3:  res = _TimerPinMap{20, 3, 0,  2}.value; break; /* TIM20_CH4/GPIO_AF2 */
	case PF_4:  res = _TimerPinMap{20, 0, 1,  3}.value; break; /* TIM20_CH1N/GPIO_AF3 */
	case PF_5:  res = _TimerPinMap{20, 1, 1,  2}.value; break; /* TIM20_CH2N/GPIO_AF2 */
	case PF_6:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PF_9:  res = _TimerPinMap{15, 0, 0,  3}.value; break; /* TIM15_CH1/GPIO_AF3 */
	case PF_10: res = _TimerPinMap{15, 1, 0,  3}.value; break; /* TIM15_CH2/GPIO_AF3 */
	case PF_12: res = _TimerPinMap{20, 0, 0,  2}.value; break; /* TIM20_CH1/GPIO_AF2 */
	case PF_13: res = _TimerPinMap{20, 1, 0,  2}.value; break; /* TIM20_CH2/GPIO_AF2 */
	case PF_14: res = _TimerPinMap{20, 2, 0,  2}.value; break; /* TIM20_CH3/GPIO_AF2 */
	case PF_15: res = _TimerPinMap{20, 3, 0,  2}.value; break; /* TIM20_CH4/GPIO_AF2 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_0:  res = _TimerPinMap{20, 0, 1,  2}.value; break; /* TIM20_CH1N/GPIO_AF2 */
	case PG_1:  res = _TimerPinMap{20, 1, 1,  2}.value; break; /* TIM20_CH2N/GPIO_AF2 */
	case PG_2:  res = _TimerPinMap{20, 2, 1,  2}.value; break; /* TIM20_CH3N/GPIO_AF2 */
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PH_0:  res = _TimerPinMap{20, 0, 0,  2}.value; break; /* TIM20_CH1/GPIO_AF2 */
	case PH_1:  res = _TimerPinMap{20, 1, 0,  2}.value; break; /* TIM20_CH2/GPIO_AF2 */
#endif /* GPIOH_BASE */
#elif defined(STM32F303C6Tx) || defined(STM32F303C8Tx) || defined(STM32F303C8Yx) || defined(STM32F303K6Tx) || defined(STM32F303K8Tx) || defined(STM32F303K6Ux) || defined(STM32F303K8Ux) || defined(STM32F303R6Tx) || defined(STM32F303R8Tx) || defined(STM32F328C8Tx) || defined(STM32F334C4Tx) || defined(STM32F334C6Tx) || defined(STM32F334C8Tx) || defined(STM32F334C8Yx) || defined(STM32F334K4Tx) || defined(STM32F334K6Tx) || defined(STM32F334K8Tx) || defined(STM32F334K4Ux) || defined(STM32F334K6Ux) || defined(STM32F334K8Ux) || defined(STM32F334R6Tx) || defined(STM32F334R8Tx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{16, 0, 1,  1}.value; break; /* TIM16_CH1N/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{16, 0, 1,  1}.value; break; /* TIM16_CH1N/GPIO_AF1 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  1}.value; break; /* TIM17_CH1/GPIO_AF1 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
#endif /* GPIOF_BASE */
#elif defined(STM32F373C8Tx) || defined(STM32F373CBTx) || defined(STM32F373CCTx) || defined(STM32F373R8Tx) || defined(STM32F373RBTx) || defined(STM32F373RCTx) || defined(STM32F373V8Hx) || defined(STM32F373VBHx) || defined(STM32F373VCHx) || defined(STM32F373V8Tx) || defined(STM32F373VBTx) || defined(STM32F373VCTx) || defined(STM32F378CCTx) || defined(STM32F378RCTx) || defined(STM32F378RCYx) || defined(STM32F378VCHx) || defined(STM32F378VCTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 2, 2, 0, 10}.value; break; /* TIM2_CH3/GPIO_AF10 */
	case PA_10: res = _TimerPinMap{ 2, 3, 0, 10}.value; break; /* TIM2_CH4/GPIO_AF10 */
	case PA_11: res = _TimerPinMap{ 4, 0, 0, 10}.value; break; /* TIM4_CH1/GPIO_AF10 */
	case PA_12: res = _TimerPinMap{ 4, 1, 0, 10}.value; break; /* TIM4_CH2/GPIO_AF10 */
	case PA_13: res = _TimerPinMap{ 4, 2, 0, 10}.value; break; /* TIM4_CH3/GPIO_AF10 */
	case PA_14: res = _TimerPinMap{12, 0, 0, 10}.value; break; /* TIM12_CH1/GPIO_AF10 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 3, 2, 0, 10}.value; break; /* TIM3_CH3/GPIO_AF10 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{12, 0, 0,  9}.value; break; /* TIM12_CH1/GPIO_AF9 */
	case PB_15: res = _TimerPinMap{12, 1, 0,  9}.value; break; /* TIM12_CH2/GPIO_AF9 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PC_4:  res = _TimerPinMap{13, 0, 0,  2}.value; break; /* TIM13_CH1/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{19, 0, 0,  2}.value; break; /* TIM19_CH1/GPIO_AF2 */
	case PC_11: res = _TimerPinMap{19, 1, 0,  2}.value; break; /* TIM19_CH2/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{19, 2, 0,  2}.value; break; /* TIM19_CH3/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{19, 3, 0,  2}.value; break; /* TIM19_CH4/GPIO_AF2 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PF_9:  res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
#endif /* GPIOF_BASE */
#elif defined(STM32F410C8Tx) || defined(STM32F410CBTx) || defined(STM32F410C8Ux) || defined(STM32F410CBUx) || defined(STM32F410R8Ix) || defined(STM32F410RBIx) || defined(STM32F410R8Tx) || defined(STM32F410RBTx) || defined(STM32F410T8Yx) || defined(STM32F410TBYx)
	case PA_0:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_8:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
	case PB_11: res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PB_12: res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PC_4:  res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PC_5:  res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PC_10: res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PC_11: res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
#endif /* GPIOC_BASE */
#elif defined(STM32F446MCYx) || defined(STM32F446MEYx) || defined(STM32F446RCTx) || defined(STM32F446RETx) || defined(STM32F446VCTx) || defined(STM32F446VETx) || defined(STM32F446ZCHx) || defined(STM32F446ZEHx) || defined(STM32F446ZCJx) || defined(STM32F446ZEJx) || defined(STM32F446ZCTx) || defined(STM32F446ZETx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_5:  res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PE_6:  res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{10, 0, 0,  3}.value; break; /* TIM10_CH1/GPIO_AF3 */
	case PF_7:  res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
	case PF_8:  res = _TimerPinMap{13, 0, 0,  9}.value; break; /* TIM13_CH1/GPIO_AF9 */
	case PF_9:  res = _TimerPinMap{14, 0, 0,  9}.value; break; /* TIM14_CH1/GPIO_AF9 */
#endif /* GPIOF_BASE */
#elif defined(STM32G030C6Tx) || defined(STM32G030C8Tx) || defined(STM32G030F6Px) || defined(STM32G030J6Mx) || defined(STM32G030K6Tx) || defined(STM32G030K8Tx) || defined(STM32G031F4Px) || defined(STM32G031F6Px) || defined(STM32G031F8Px) || defined(STM32G031J4Mx) || defined(STM32G031J6Mx) || defined(STM32G031Y8Yx) || defined(STM32G041F6Px) || defined(STM32G041F8Px) || defined(STM32G041J6Mx) || defined(STM32G041Y8Yx) || defined(STM32G071EBYx) || defined(STM32G081EBYx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_4:  res = _TimerPinMap{14, 0, 0,  4}.value; break; /* TIM14_CH1/GPIO_AF4 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  5}.value; break; /* LPTIM1_OUT/GPIO_AF5 */
	case PB_3:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  2}.value; break; /* TIM17_CH1N/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PD_1:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PD_2:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PD_3:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
#endif /* GPIOF_BASE */
#elif defined(STM32G031C4Tx) || defined(STM32G031C6Tx) || defined(STM32G031C8Tx) || defined(STM32G031C4Ux) || defined(STM32G031C6Ux) || defined(STM32G031C8Ux) || defined(STM32G031G4Ux) || defined(STM32G031G6Ux) || defined(STM32G031G8Ux) || defined(STM32G031K4Tx) || defined(STM32G031K6Tx) || defined(STM32G031K8Tx) || defined(STM32G031K4Ux) || defined(STM32G031K6Ux) || defined(STM32G031K8Ux) || defined(STM32G041C6Tx) || defined(STM32G041C8Tx) || defined(STM32G041C6Ux) || defined(STM32G041C8Ux) || defined(STM32G041G6Ux) || defined(STM32G041G8Ux) || defined(STM32G041K6Tx) || defined(STM32G041K8Tx) || defined(STM32G041K6Ux) || defined(STM32G041K8Ux) || defined(STM32G071C6Tx) || defined(STM32G071C8Tx) || defined(STM32G071CBTx) || defined(STM32G071C6Ux) || defined(STM32G071C8Ux) || defined(STM32G071CBUx) || defined(STM32G071G6Ux) || defined(STM32G071G8Ux) || defined(STM32G071GBUx) || defined(STM32G071G8UxN) || defined(STM32G071GBUxN) || defined(STM32G071K6Tx) || defined(STM32G071K8Tx) || defined(STM32G071KBTx) || defined(STM32G071K6Ux) || defined(STM32G071K8Ux) || defined(STM32G071KBUx) || defined(STM32G071K8TxN) || defined(STM32G071KBTxN) || defined(STM32G071K8UxN) || defined(STM32G071KBUxN) || defined(STM32G071R6Tx) || defined(STM32G071R8Tx) || defined(STM32G071RBTx) || defined(STM32G071RBIx) || defined(STM32G081CBTx) || defined(STM32G081CBUx) || defined(STM32G081GBUx) || defined(STM32G081GBUxN) || defined(STM32G081KBTx) || defined(STM32G081KBTxN) || defined(STM32G081KBUx) || defined(STM32G081KBUxN) || defined(STM32G081RBIx) || defined(STM32G081RBTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_4:  res = _TimerPinMap{14, 0, 0,  4}.value; break; /* TIM14_CH1/GPIO_AF4 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  5}.value; break; /* LPTIM1_OUT/GPIO_AF5 */
	case PB_3:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  2}.value; break; /* TIM17_CH1N/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{15, 0, 0,  2}.value; break; /* TIM15_CH1/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{15, 1, 0,  2}.value; break; /* TIM15_CH2/GPIO_AF2 */
	case PC_4:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PC_5:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PD_1:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PD_2:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PD_3:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PD_6:  res = _TimerPinMap{26, 0, 0,  2}.value; break; /* LPTIM2_OUT/GPIO_AF2 */
	case PD_8:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
	case PF_1:  res = _TimerPinMap{15, 0, 1,  2}.value; break; /* TIM15_CH1N/GPIO_AF2 */
#endif /* GPIOF_BASE */
#elif defined(STM32G070CBTx) || defined(STM32G070KBTx) || defined(STM32G070RBTx)
	case PA_1:  res = _TimerPinMap{15, 0, 1,  5}.value; break; /* TIM15_CH1N/GPIO_AF5 */
	case PA_2:  res = _TimerPinMap{15, 0, 0,  5}.value; break; /* TIM15_CH1/GPIO_AF5 */
	case PA_3:  res = _TimerPinMap{15, 1, 0,  5}.value; break; /* TIM15_CH2/GPIO_AF5 */
	case PA_4:  res = _TimerPinMap{14, 0, 0,  4}.value; break; /* TIM14_CH1/GPIO_AF4 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PB_7:  res = _TimerPinMap{17, 0, 1,  2}.value; break; /* TIM17_CH1N/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{15, 0, 0,  2}.value; break; /* TIM15_CH1/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{15, 1, 0,  2}.value; break; /* TIM15_CH2/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  1}.value; break; /* TIM3_CH1/GPIO_AF1 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  1}.value; break; /* TIM3_CH2/GPIO_AF1 */
	case PC_8:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_11: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{16, 0, 0,  2}.value; break; /* TIM16_CH1/GPIO_AF2 */
	case PD_1:  res = _TimerPinMap{17, 0, 0,  2}.value; break; /* TIM17_CH1/GPIO_AF2 */
	case PD_2:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PD_3:  res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{14, 0, 0,  2}.value; break; /* TIM14_CH1/GPIO_AF2 */
	case PF_1:  res = _TimerPinMap{15, 0, 1,  2}.value; break; /* TIM15_CH1N/GPIO_AF2 */
#endif /* GPIOF_BASE */
#elif defined(STM32G431C6Tx) || defined(STM32G431C8Tx) || defined(STM32G431CBTx) || defined(STM32G431C6Ux) || defined(STM32G431C8Ux) || defined(STM32G431CBUx) || defined(STM32G431CBYx) || defined(STM32G431M6Tx) || defined(STM32G431M8Tx) || defined(STM32G431MBTx) || defined(STM32G431R6Ix) || defined(STM32G431R8Ix) || defined(STM32G431RBIx) || defined(STM32G431R6Tx) || defined(STM32G431R8Tx) || defined(STM32G431RBTx) || defined(STM32G431V6Tx) || defined(STM32G431V8Tx) || defined(STM32G431VBTx) || defined(STM32G441CBTx) || defined(STM32G441CBUx) || defined(STM32G441CBYx) || defined(STM32G441MBTx) || defined(STM32G441RBIx) || defined(STM32G441RBTx) || defined(STM32G441VBTx) || defined(STM32GBK1CBTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{ 4, 2, 0, 10}.value; break; /* TIM4_CH3/GPIO_AF10 */
	case PA_14: res = _TimerPinMap{ 8, 1, 0,  5}.value; break; /* TIM8_CH2/GPIO_AF5 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 1, 2, 1, 12}.value; break; /* TIM1_CH3N/GPIO_AF12 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_5:  res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 8, 0, 1,  4}.value; break; /* TIM8_CH1N/GPIO_AF4 */
	case PC_11: res = _TimerPinMap{ 8, 1, 1,  4}.value; break; /* TIM8_CH2N/GPIO_AF4 */
	case PC_12: res = _TimerPinMap{ 8, 2, 1,  4}.value; break; /* TIM8_CH3N/GPIO_AF4 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{ 8, 3, 1,  6}.value; break; /* TIM8_CH4N/GPIO_AF6 */
	case PD_1:  res = _TimerPinMap{ 8, 3, 0,  4}.value; break; /* TIM8_CH4/GPIO_AF4 */
	case PD_3:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PD_6:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PD_7:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0,  4}.value; break; /* TIM16_CH1/GPIO_AF4 */
	case PE_1:  res = _TimerPinMap{17, 0, 0,  4}.value; break; /* TIM17_CH1/GPIO_AF4 */
	case PE_2:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_3:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PE_15: res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PF_9:  res = _TimerPinMap{15, 0, 0,  3}.value; break; /* TIM15_CH1/GPIO_AF3 */
	case PF_10: res = _TimerPinMap{15, 1, 0,  3}.value; break; /* TIM15_CH2/GPIO_AF3 */
#endif /* GPIOF_BASE */
#elif defined(STM32G471CCTx) || defined(STM32G471CETx) || defined(STM32G471CCUx) || defined(STM32G471CEUx) || defined(STM32G471MCTx) || defined(STM32G471METx) || defined(STM32G471MEYx) || defined(STM32G471QCTx) || defined(STM32G471QETx) || defined(STM32G471RCTx) || defined(STM32G471RETx) || defined(STM32G471VCHx) || defined(STM32G471VEHx) || defined(STM32G471VCIx) || defined(STM32G471VEIx) || defined(STM32G471VCTx) || defined(STM32G471VETx) || defined(STM32G473CBTx) || defined(STM32G473CCTx) || defined(STM32G473CETx) || defined(STM32G473CBUx) || defined(STM32G473CCUx) || defined(STM32G473CEUx) || defined(STM32G473MBTx) || defined(STM32G473MCTx) || defined(STM32G473METx) || defined(STM32G473MEYx) || defined(STM32G473PBIx) || defined(STM32G473PCIx) || defined(STM32G473PEIx) || defined(STM32G473QBTx) || defined(STM32G473QCTx) || defined(STM32G473QETx) || defined(STM32G473RBTx) || defined(STM32G473RCTx) || defined(STM32G473RETx) || defined(STM32G473VBHx) || defined(STM32G473VCHx) || defined(STM32G473VEHx) || defined(STM32G473VBIx) || defined(STM32G473VCIx) || defined(STM32G473VEIx) || defined(STM32G473VBTx) || defined(STM32G473VCTx) || defined(STM32G473VETx) || defined(STM32G474CBTx) || defined(STM32G474CCTx) || defined(STM32G474CETx) || defined(STM32G474CBUx) || defined(STM32G474CCUx) || defined(STM32G474CEUx) || defined(STM32G474MBTx) || defined(STM32G474MCTx) || defined(STM32G474METx) || defined(STM32G474MEYx) || defined(STM32G474PBIx) || defined(STM32G474PCIx) || defined(STM32G474PEIx) || defined(STM32G474QBTx) || defined(STM32G474QCTx) || defined(STM32G474QETx) || defined(STM32G474RBTx) || defined(STM32G474RCTx) || defined(STM32G474RETx) || defined(STM32G474VBHx) || defined(STM32G474VCHx) || defined(STM32G474VEHx) || defined(STM32G474VBIx) || defined(STM32G474VCIx) || defined(STM32G474VEIx) || defined(STM32G474VBTx) || defined(STM32G474VCTx) || defined(STM32G474VETx) || defined(STM32G483CETx) || defined(STM32G483CEUx) || defined(STM32G483METx) || defined(STM32G483MEYx) || defined(STM32G483PEIx) || defined(STM32G483QETx) || defined(STM32G483RETx) || defined(STM32G483VEHx) || defined(STM32G483VEIx) || defined(STM32G483VETx) || defined(STM32G484CETx) || defined(STM32G484CEUx) || defined(STM32G484METx) || defined(STM32G484MEYx) || defined(STM32G484PEIx) || defined(STM32G484QETx) || defined(STM32G484RETx) || defined(STM32G484VEHx) || defined(STM32G484VEIx) || defined(STM32G484VETx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{ 4, 2, 0, 10}.value; break; /* TIM4_CH3/GPIO_AF10 */
	case PA_14: res = _TimerPinMap{ 8, 1, 0,  5}.value; break; /* TIM8_CH2/GPIO_AF5 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_2:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 1, 2, 1, 12}.value; break; /* TIM1_CH3N/GPIO_AF12 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_5:  res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 8, 0, 1,  4}.value; break; /* TIM8_CH1N/GPIO_AF4 */
	case PC_11: res = _TimerPinMap{ 8, 1, 1,  4}.value; break; /* TIM8_CH2N/GPIO_AF4 */
	case PC_12: res = _TimerPinMap{ 5, 1, 0,  1}.value; break; /* TIM5_CH2/GPIO_AF1 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{ 8, 3, 1,  6}.value; break; /* TIM8_CH4N/GPIO_AF6 */
	case PD_1:  res = _TimerPinMap{ 8, 3, 0,  4}.value; break; /* TIM8_CH4/GPIO_AF4 */
	case PD_3:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PD_6:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PD_7:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0,  4}.value; break; /* TIM16_CH1/GPIO_AF4 */
	case PE_1:  res = _TimerPinMap{17, 0, 0,  4}.value; break; /* TIM17_CH1/GPIO_AF4 */
	case PE_2:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_3:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{20, 2, 1,  6}.value; break; /* TIM20_CH3N/GPIO_AF6 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PE_15: res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PF_2:  res = _TimerPinMap{20, 2, 0,  2}.value; break; /* TIM20_CH3/GPIO_AF2 */
	case PF_3:  res = _TimerPinMap{20, 3, 0,  2}.value; break; /* TIM20_CH4/GPIO_AF2 */
	case PF_4:  res = _TimerPinMap{20, 0, 1,  3}.value; break; /* TIM20_CH1N/GPIO_AF3 */
	case PF_5:  res = _TimerPinMap{20, 1, 1,  2}.value; break; /* TIM20_CH2N/GPIO_AF2 */
	case PF_6:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PF_7:  res = _TimerPinMap{ 5, 1, 0,  6}.value; break; /* TIM5_CH2/GPIO_AF6 */
	case PF_8:  res = _TimerPinMap{ 5, 2, 0,  6}.value; break; /* TIM5_CH3/GPIO_AF6 */
	case PF_9:  res = _TimerPinMap{ 5, 3, 0,  6}.value; break; /* TIM5_CH4/GPIO_AF6 */
	case PF_10: res = _TimerPinMap{15, 1, 0,  3}.value; break; /* TIM15_CH2/GPIO_AF3 */
	case PF_12: res = _TimerPinMap{20, 0, 0,  2}.value; break; /* TIM20_CH1/GPIO_AF2 */
	case PF_13: res = _TimerPinMap{20, 1, 0,  2}.value; break; /* TIM20_CH2/GPIO_AF2 */
	case PF_14: res = _TimerPinMap{20, 2, 0,  2}.value; break; /* TIM20_CH3/GPIO_AF2 */
	case PF_15: res = _TimerPinMap{20, 3, 0,  2}.value; break; /* TIM20_CH4/GPIO_AF2 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_0:  res = _TimerPinMap{20, 0, 1,  2}.value; break; /* TIM20_CH1N/GPIO_AF2 */
	case PG_1:  res = _TimerPinMap{20, 1, 1,  2}.value; break; /* TIM20_CH2N/GPIO_AF2 */
	case PG_2:  res = _TimerPinMap{20, 2, 1,  2}.value; break; /* TIM20_CH3N/GPIO_AF2 */
	case PG_3:  res = _TimerPinMap{20, 3, 1,  6}.value; break; /* TIM20_CH4N/GPIO_AF6 */
	case PG_9:  res = _TimerPinMap{15, 0, 1, 14}.value; break; /* TIM15_CH1N/GPIO_AF14 */
#endif /* GPIOG_BASE */
#elif defined(STM32G491CCTx) || defined(STM32G491CETx) || defined(STM32G491CCUx) || defined(STM32G491CEUx) || defined(STM32G491MCSx) || defined(STM32G491MESx) || defined(STM32G491MCTx) || defined(STM32G491METx) || defined(STM32G491RCIx) || defined(STM32G491REIx) || defined(STM32G491RCTx) || defined(STM32G491RETx) || defined(STM32G491REYx) || defined(STM32G491VCTx) || defined(STM32G491VETx) || defined(STM32G4A1CETx) || defined(STM32G4A1CEUx) || defined(STM32G4A1MESx) || defined(STM32G4A1METx) || defined(STM32G4A1REIx) || defined(STM32G4A1RETx) || defined(STM32G4A1REYx) || defined(STM32G4A1VETx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  6}.value; break; /* TIM1_CH1/GPIO_AF6 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  6}.value; break; /* TIM1_CH2/GPIO_AF6 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  6}.value; break; /* TIM1_CH3/GPIO_AF6 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0, 11}.value; break; /* TIM1_CH4/GPIO_AF11 */
	case PA_12: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PA_13: res = _TimerPinMap{ 4, 2, 0, 10}.value; break; /* TIM4_CH3/GPIO_AF10 */
	case PA_14: res = _TimerPinMap{ 8, 1, 0,  5}.value; break; /* TIM8_CH2/GPIO_AF5 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PB_2:  res = _TimerPinMap{20, 0, 0,  3}.value; break; /* TIM20_CH1/GPIO_AF3 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 3, 3, 0, 10}.value; break; /* TIM3_CH4/GPIO_AF10 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 1, 2, 1, 12}.value; break; /* TIM1_CH3N/GPIO_AF12 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  6}.value; break; /* TIM1_CH1N/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  6}.value; break; /* TIM1_CH2N/GPIO_AF6 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  4}.value; break; /* TIM1_CH3N/GPIO_AF4 */
#ifdef GPIOC_BASE
	case PC_0:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PC_1:  res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PC_2:  res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PC_3:  res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PC_5:  res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_10: res = _TimerPinMap{ 8, 0, 1,  4}.value; break; /* TIM8_CH1N/GPIO_AF4 */
	case PC_11: res = _TimerPinMap{ 8, 1, 1,  4}.value; break; /* TIM8_CH2N/GPIO_AF4 */
	case PC_12: res = _TimerPinMap{ 8, 2, 1,  4}.value; break; /* TIM8_CH3N/GPIO_AF4 */
	case PC_13: res = _TimerPinMap{ 1, 0, 1,  4}.value; break; /* TIM1_CH1N/GPIO_AF4 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{ 8, 3, 1,  6}.value; break; /* TIM8_CH4N/GPIO_AF6 */
	case PD_1:  res = _TimerPinMap{ 8, 3, 0,  4}.value; break; /* TIM8_CH4/GPIO_AF4 */
	case PD_3:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PD_4:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PD_6:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PD_7:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0,  4}.value; break; /* TIM16_CH1/GPIO_AF4 */
	case PE_1:  res = _TimerPinMap{17, 0, 0,  4}.value; break; /* TIM17_CH1/GPIO_AF4 */
	case PE_2:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_3:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{20, 2, 1,  6}.value; break; /* TIM20_CH3N/GPIO_AF6 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  2}.value; break; /* TIM1_CH1N/GPIO_AF2 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  2}.value; break; /* TIM1_CH1/GPIO_AF2 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  2}.value; break; /* TIM1_CH2N/GPIO_AF2 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  2}.value; break; /* TIM1_CH2/GPIO_AF2 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  2}.value; break; /* TIM1_CH3N/GPIO_AF2 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  2}.value; break; /* TIM1_CH3/GPIO_AF2 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  2}.value; break; /* TIM1_CH4/GPIO_AF2 */
	case PE_15: res = _TimerPinMap{ 1, 3, 1,  6}.value; break; /* TIM1_CH4N/GPIO_AF6 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{ 1, 2, 1,  6}.value; break; /* TIM1_CH3N/GPIO_AF6 */
	case PF_2:  res = _TimerPinMap{20, 2, 0,  2}.value; break; /* TIM20_CH3/GPIO_AF2 */
	case PF_9:  res = _TimerPinMap{15, 0, 0,  3}.value; break; /* TIM15_CH1/GPIO_AF3 */
	case PF_10: res = _TimerPinMap{15, 1, 0,  3}.value; break; /* TIM15_CH2/GPIO_AF3 */
#endif /* GPIOF_BASE */
#elif defined(STM32H723VEHx) || defined(STM32H723VETx) || defined(STM32H723VGHx) || defined(STM32H723VGTx) || defined(STM32H723ZEIx) || defined(STM32H723ZETx) || defined(STM32H723ZGIx) || defined(STM32H723ZGTx) || defined(STM32H725AEIx) || defined(STM32H725AGIx) || defined(STM32H725IEKx) || defined(STM32H725IETx) || defined(STM32H725IGKx) || defined(STM32H725IGTx) || defined(STM32H725VETx) || defined(STM32H725VGTx) || defined(STM32H725ZETx) || defined(STM32H725ZGTx) || defined(STM32H730ABIxQ) || defined(STM32H730IBKxQ) || defined(STM32H730IBTxQ) || defined(STM32H730VBHx) || defined(STM32H730VBTx) || defined(STM32H730ZBIx) || defined(STM32H730ZBTx) || defined(STM32H733VGHx) || defined(STM32H733VGTx) || defined(STM32H733ZGIx) || defined(STM32H733ZGTx) || defined(STM32H735AGIx) || defined(STM32H735IGKx) || defined(STM32H735IGTx) || defined(STM32H735VGTx) || defined(STM32H735ZGTx) || defined(STM32H742VGHx) || defined(STM32H742VIHx) || defined(STM32H742VGTx) || defined(STM32H742VITx) || defined(STM32H743VGHx) || defined(STM32H743VIHx) || defined(STM32H743VGTx) || defined(STM32H743VITx) || defined(STM32H747ZIYx) || defined(STM32H750VBTx) || defined(STM32H753VIHx) || defined(STM32H753VITx) || defined(STM32H757ZIYx) || defined(STM32H7A3VGHx) || defined(STM32H7A3VIHx) || defined(STM32H7A3VGHxQ) || defined(STM32H7A3VIHxQ) || defined(STM32H7A3VGTx) || defined(STM32H7A3VITx) || defined(STM32H7A3VGTxQ) || defined(STM32H7A3VITxQ) || defined(STM32H7B0VBTx) || defined(STM32H7B3VIHx) || defined(STM32H7B3VIHxQ) || defined(STM32H7B3VITx) || defined(STM32H7B3VITxQ)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{15, 0, 0,  2}.value; break; /* TIM15_CH1/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_4:  res = _TimerPinMap{15, 0, 1,  4}.value; break; /* TIM15_CH1N/GPIO_AF4 */
	case PE_5:  res = _TimerPinMap{15, 0, 0,  4}.value; break; /* TIM15_CH1/GPIO_AF4 */
	case PE_6:  res = _TimerPinMap{15, 1, 0,  4}.value; break; /* TIM15_CH2/GPIO_AF4 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_0:  res = _TimerPinMap{23, 0, 0, 13}.value; break; /* TIM23_CH1/GPIO_AF13 */
	case PF_1:  res = _TimerPinMap{23, 1, 0, 13}.value; break; /* TIM23_CH2/GPIO_AF13 */
	case PF_2:  res = _TimerPinMap{23, 2, 0, 13}.value; break; /* TIM23_CH3/GPIO_AF13 */
	case PF_3:  res = _TimerPinMap{23, 3, 0, 13}.value; break; /* TIM23_CH4/GPIO_AF13 */
	case PF_6:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PF_7:  res = _TimerPinMap{17, 0, 0,  1}.value; break; /* TIM17_CH1/GPIO_AF1 */
	case PF_8:  res = _TimerPinMap{13, 0, 0,  9}.value; break; /* TIM13_CH1/GPIO_AF9 */
	case PF_9:  res = _TimerPinMap{14, 0, 0,  9}.value; break; /* TIM14_CH1/GPIO_AF9 */
	case PF_11: res = _TimerPinMap{24, 0, 0, 14}.value; break; /* TIM24_CH1/GPIO_AF14 */
	case PF_12: res = _TimerPinMap{24, 1, 0, 14}.value; break; /* TIM24_CH2/GPIO_AF14 */
	case PF_13: res = _TimerPinMap{24, 2, 0, 14}.value; break; /* TIM24_CH3/GPIO_AF14 */
	case PF_14: res = _TimerPinMap{24, 3, 0, 14}.value; break; /* TIM24_CH4/GPIO_AF14 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_12: res = _TimerPinMap{23, 0, 0, 13}.value; break; /* TIM23_CH1/GPIO_AF13 */
	case PG_13: res = _TimerPinMap{23, 1, 0, 13}.value; break; /* TIM23_CH2/GPIO_AF13 */
	case PG_14: res = _TimerPinMap{23, 2, 0, 13}.value; break; /* TIM23_CH3/GPIO_AF13 */
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PH_6:  res = _TimerPinMap{12, 0, 0,  2}.value; break; /* TIM12_CH1/GPIO_AF2 */
	case PH_9:  res = _TimerPinMap{12, 1, 0,  2}.value; break; /* TIM12_CH2/GPIO_AF2 */
	case PH_10: res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PH_11: res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PH_12: res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PH_13: res = _TimerPinMap{ 8, 0, 1,  3}.value; break; /* TIM8_CH1N/GPIO_AF3 */
	case PH_14: res = _TimerPinMap{ 8, 1, 1,  3}.value; break; /* TIM8_CH2N/GPIO_AF3 */
	case PH_15: res = _TimerPinMap{ 8, 2, 1,  3}.value; break; /* TIM8_CH3N/GPIO_AF3 */
#endif /* GPIOH_BASE */
#ifdef GPIOJ_BASE
	case PJ_8:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PJ_9:  res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PJ_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PJ_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
#endif /* GPIOJ_BASE */
#ifdef GPIOK_BASE
	case PK_0:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PK_1:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
#endif /* GPIOK_BASE */
#elif defined(STM32H742AGIx) || defined(STM32H742AIIx) || defined(STM32H742BGTx) || defined(STM32H742BITx) || defined(STM32H742IGKx) || defined(STM32H742IIKx) || defined(STM32H742IGTx) || defined(STM32H742IITx) || defined(STM32H742XGHx) || defined(STM32H742XIHx) || defined(STM32H742ZGTx) || defined(STM32H742ZITx) || defined(STM32H743AGIx) || defined(STM32H743AIIx) || defined(STM32H743BGTx) || defined(STM32H743BITx) || defined(STM32H743IGKx) || defined(STM32H743IGTx) || defined(STM32H743IIKx) || defined(STM32H743IITx) || defined(STM32H743XGHx) || defined(STM32H743XIHx) || defined(STM32H743ZGTx) || defined(STM32H743ZITx) || defined(STM32H745BGTx) || defined(STM32H745BITx) || defined(STM32H745IGKx) || defined(STM32H745IGTx) || defined(STM32H745IIKx) || defined(STM32H745IITx) || defined(STM32H745XGHx) || defined(STM32H745XIHx) || defined(STM32H745ZGTx) || defined(STM32H745ZITx) || defined(STM32H747AGIx) || defined(STM32H747AIIx) || defined(STM32H747BGTx) || defined(STM32H747BITx) || defined(STM32H747IGTx) || defined(STM32H747IITx) || defined(STM32H747XGHx) || defined(STM32H747XIHx) || defined(STM32H750IBKx) || defined(STM32H750IBTx) || defined(STM32H750XBHx) || defined(STM32H750ZBTx) || defined(STM32H753AIIx) || defined(STM32H753BITx) || defined(STM32H753IIKx) || defined(STM32H753IITx) || defined(STM32H753XIHx) || defined(STM32H753ZITx) || defined(STM32H755BITx) || defined(STM32H755IIKx) || defined(STM32H755IITx) || defined(STM32H755XIHx) || defined(STM32H755ZITx) || defined(STM32H757AIIx) || defined(STM32H757BITx) || defined(STM32H757IITx) || defined(STM32H757XIHx) || defined(STM32H7A3AGIxQ) || defined(STM32H7A3AIIxQ) || defined(STM32H7A3IGKx) || defined(STM32H7A3IIKx) || defined(STM32H7A3IGKxQ) || defined(STM32H7A3IIKxQ) || defined(STM32H7A3IGTx) || defined(STM32H7A3IITx) || defined(STM32H7A3IGTxQ) || defined(STM32H7A3IITxQ) || defined(STM32H7A3LGHxQ) || defined(STM32H7A3LIHxQ) || defined(STM32H7A3NGHx) || defined(STM32H7A3NIHx) || defined(STM32H7A3QIYxQ) || defined(STM32H7A3ZGTx) || defined(STM32H7A3ZITx) || defined(STM32H7A3ZGTxQ) || defined(STM32H7A3ZITxQ) || defined(STM32H7B0ABIxQ) || defined(STM32H7B0IBKxQ) || defined(STM32H7B0IBTx) || defined(STM32H7B0ZBTx) || defined(STM32H7B3AIIxQ) || defined(STM32H7B3IIKx) || defined(STM32H7B3IIKxQ) || defined(STM32H7B3IITx) || defined(STM32H7B3IITxQ) || defined(STM32H7B3LIHxQ) || defined(STM32H7B3NIHx) || defined(STM32H7B3QIYxQ) || defined(STM32H7B3ZITx) || defined(STM32H7B3ZITxQ) || defined(STM32MP151AAAx) || defined(STM32MP151AABx) || defined(STM32MP151AACx) || defined(STM32MP151AADx) || defined(STM32MP151CAAx) || defined(STM32MP151CABx) || defined(STM32MP151CACx) || defined(STM32MP151CADx) || defined(STM32MP151DAAx) || defined(STM32MP151DABx) || defined(STM32MP151DACx) || defined(STM32MP151DADx) || defined(STM32MP151FAAx) || defined(STM32MP151FABx) || defined(STM32MP151FACx) || defined(STM32MP151FADx) || defined(STM32MP153AAAx) || defined(STM32MP153AABx) || defined(STM32MP153AACx) || defined(STM32MP153AADx) || defined(STM32MP153CAAx) || defined(STM32MP153CABx) || defined(STM32MP153CACx) || defined(STM32MP153CADx) || defined(STM32MP153DAAx) || defined(STM32MP153DABx) || defined(STM32MP153DACx) || defined(STM32MP153DADx) || defined(STM32MP153FAAx) || defined(STM32MP153FABx) || defined(STM32MP153FACx) || defined(STM32MP153FADx) || defined(STM32MP157AAAx) || defined(STM32MP157AABx) || defined(STM32MP157AACx) || defined(STM32MP157AADx) || defined(STM32MP157CAAx) || defined(STM32MP157CABx) || defined(STM32MP157CACx) || defined(STM32MP157CADx) || defined(STM32MP157DAAx) || defined(STM32MP157DABx) || defined(STM32MP157DACx) || defined(STM32MP157DADx) || defined(STM32MP157FAAx) || defined(STM32MP157FABx) || defined(STM32MP157FACx) || defined(STM32MP157FADx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PC_12: res = _TimerPinMap{15, 0, 0,  2}.value; break; /* TIM15_CH1/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_6:  res = _TimerPinMap{16, 0, 1,  1}.value; break; /* TIM16_CH1N/GPIO_AF1 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_4:  res = _TimerPinMap{15, 0, 1,  4}.value; break; /* TIM15_CH1N/GPIO_AF4 */
	case PE_5:  res = _TimerPinMap{15, 0, 0,  4}.value; break; /* TIM15_CH1/GPIO_AF4 */
	case PE_6:  res = _TimerPinMap{15, 1, 0,  4}.value; break; /* TIM15_CH2/GPIO_AF4 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{16, 0, 0,  1}.value; break; /* TIM16_CH1/GPIO_AF1 */
	case PF_7:  res = _TimerPinMap{17, 0, 0,  1}.value; break; /* TIM17_CH1/GPIO_AF1 */
	case PF_8:  res = _TimerPinMap{13, 0, 0,  9}.value; break; /* TIM13_CH1/GPIO_AF9 */
	case PF_9:  res = _TimerPinMap{14, 0, 0,  9}.value; break; /* TIM14_CH1/GPIO_AF9 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_8:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PG_13: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PH_6:  res = _TimerPinMap{12, 0, 0,  2}.value; break; /* TIM12_CH1/GPIO_AF2 */
	case PH_9:  res = _TimerPinMap{12, 1, 0,  2}.value; break; /* TIM12_CH2/GPIO_AF2 */
	case PH_10: res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PH_11: res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PH_12: res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PH_13: res = _TimerPinMap{ 8, 0, 1,  3}.value; break; /* TIM8_CH1N/GPIO_AF3 */
	case PH_14: res = _TimerPinMap{ 8, 1, 1,  3}.value; break; /* TIM8_CH2N/GPIO_AF3 */
	case PH_15: res = _TimerPinMap{ 8, 2, 1,  3}.value; break; /* TIM8_CH3N/GPIO_AF3 */
#endif /* GPIOH_BASE */
#ifdef GPIOI_BASE
	case PI_0:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PI_2:  res = _TimerPinMap{ 8, 3, 0,  3}.value; break; /* TIM8_CH4/GPIO_AF3 */
	case PI_5:  res = _TimerPinMap{ 8, 0, 0,  3}.value; break; /* TIM8_CH1/GPIO_AF3 */
	case PI_6:  res = _TimerPinMap{ 8, 1, 0,  3}.value; break; /* TIM8_CH2/GPIO_AF3 */
	case PI_7:  res = _TimerPinMap{ 8, 2, 0,  3}.value; break; /* TIM8_CH3/GPIO_AF3 */
#endif /* GPIOI_BASE */
#ifdef GPIOJ_BASE
	case PJ_6:  res = _TimerPinMap{ 8, 1, 0,  3}.value; break; /* TIM8_CH2/GPIO_AF3 */
	case PJ_7:  res = _TimerPinMap{ 8, 1, 1,  3}.value; break; /* TIM8_CH2N/GPIO_AF3 */
	case PJ_8:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PJ_9:  res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PJ_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PJ_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
#endif /* GPIOJ_BASE */
#ifdef GPIOK_BASE
	case PK_0:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PK_1:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
#endif /* GPIOK_BASE */
#elif defined(STM32L010C6Tx) || defined(STM32L010F4Px) || defined(STM32L010K8Tx) || defined(STM32L011D3Px) || defined(STM32L011D4Px) || defined(STM32L011F3Px) || defined(STM32L011F4Px) || defined(STM32L021D4Px) || defined(STM32L021F4Px)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_7:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_9:  res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_10: res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PA_11: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PB_0:  res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PB_1:  res = _TimerPinMap{ 2, 3, 0,  5}.value; break; /* TIM2_CH4/GPIO_AF5 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_14: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
#elif defined(STM32L010K4Tx) || defined(STM32L010R8Tx) || defined(STM32L011E3Yx) || defined(STM32L011E4Yx) || defined(STM32L011F3Ux) || defined(STM32L011F4Ux) || defined(STM32L011G3Ux) || defined(STM32L011G4Ux) || defined(STM32L011K3Tx) || defined(STM32L011K4Tx) || defined(STM32L011K3Ux) || defined(STM32L011K4Ux) || defined(STM32L021F4Ux) || defined(STM32L021G4Ux) || defined(STM32L021K4Tx) || defined(STM32L021K4Ux)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_7:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_9:  res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_10: res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PA_11: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PB_0:  res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PB_1:  res = _TimerPinMap{ 2, 3, 0,  5}.value; break; /* TIM2_CH4/GPIO_AF5 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_6:  res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PB_7:  res = _TimerPinMap{ 2, 3, 0,  5}.value; break; /* TIM2_CH4/GPIO_AF5 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{21, 0, 0,  6}.value; break; /* TIM21_CH1/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{21, 1, 0,  6}.value; break; /* TIM21_CH2/GPIO_AF6 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  0}.value; break; /* LPTIM1_OUT/GPIO_AF0 */
#endif /* GPIOC_BASE */
#elif defined(STM32L010RBTx) || defined(STM32L031E4Yx) || defined(STM32L031E6Yx) || defined(STM32L031F4Px) || defined(STM32L031F6Px) || defined(STM32L031G4Ux) || defined(STM32L031G6Ux) || defined(STM32L031G6UxS) || defined(STM32L031K4Tx) || defined(STM32L031K6Tx) || defined(STM32L031K4Ux) || defined(STM32L031K6Ux) || defined(STM32L041E6Yx) || defined(STM32L041F6Px) || defined(STM32L041G6Ux) || defined(STM32L041G6UxS) || defined(STM32L041K6Tx) || defined(STM32L041K6Ux) || defined(STM32L051C6Tx) || defined(STM32L051C8Tx) || defined(STM32L051C6Ux) || defined(STM32L051C8Ux) || defined(STM32L051K6Tx) || defined(STM32L051K8Tx) || defined(STM32L051K6Ux) || defined(STM32L051K8Ux) || defined(STM32L051R6Hx) || defined(STM32L051R8Hx) || defined(STM32L051R6Tx) || defined(STM32L051R8Tx) || defined(STM32L051T6Yx) || defined(STM32L051T8Yx) || defined(STM32L052C6Tx) || defined(STM32L052C8Tx) || defined(STM32L052C6Ux) || defined(STM32L052C8Ux) || defined(STM32L052K6Tx) || defined(STM32L052K8Tx) || defined(STM32L052K6Ux) || defined(STM32L052K8Ux) || defined(STM32L052R6Hx) || defined(STM32L052R8Hx) || defined(STM32L052R6Tx) || defined(STM32L052R8Tx) || defined(STM32L052T6Yx) || defined(STM32L052T8Yx) || defined(STM32L052T8Fx) || defined(STM32L053C6Tx) || defined(STM32L053C8Tx) || defined(STM32L053C6Ux) || defined(STM32L053C8Ux) || defined(STM32L053R6Hx) || defined(STM32L053R8Hx) || defined(STM32L053R6Tx) || defined(STM32L053R8Tx) || defined(STM32L062C8Ux) || defined(STM32L062K8Tx) || defined(STM32L062K8Ux) || defined(STM32L063C8Tx) || defined(STM32L063C8Ux) || defined(STM32L063R8Tx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_6:  res = _TimerPinMap{22, 0, 0,  5}.value; break; /* TIM22_CH1/GPIO_AF5 */
	case PA_7:  res = _TimerPinMap{22, 1, 0,  5}.value; break; /* TIM22_CH2/GPIO_AF5 */
	case PA_8:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_9:  res = _TimerPinMap{22, 0, 0,  5}.value; break; /* TIM22_CH1/GPIO_AF5 */
	case PA_10: res = _TimerPinMap{22, 1, 0,  5}.value; break; /* TIM22_CH2/GPIO_AF5 */
	case PA_11: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PB_0:  res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PB_1:  res = _TimerPinMap{ 2, 3, 0,  5}.value; break; /* TIM2_CH4/GPIO_AF5 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_4:  res = _TimerPinMap{22, 0, 0,  4}.value; break; /* TIM22_CH1/GPIO_AF4 */
	case PB_5:  res = _TimerPinMap{22, 1, 0,  4}.value; break; /* TIM22_CH2/GPIO_AF4 */
	case PB_6:  res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{21, 0, 0,  6}.value; break; /* TIM21_CH1/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{21, 1, 0,  6}.value; break; /* TIM21_CH2/GPIO_AF6 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  0}.value; break; /* LPTIM1_OUT/GPIO_AF0 */
	case PC_6:  res = _TimerPinMap{22, 0, 0,  0}.value; break; /* TIM22_CH1/GPIO_AF0 */
	case PC_7:  res = _TimerPinMap{22, 1, 0,  0}.value; break; /* TIM22_CH2/GPIO_AF0 */
#endif /* GPIOC_BASE */
#elif defined(STM32L031C4Tx) || defined(STM32L031C6Tx) || defined(STM32L031C4Ux) || defined(STM32L031C6Ux) || defined(STM32L041C4Tx) || defined(STM32L041C6Tx) || defined(STM32L041C6Ux)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_6:  res = _TimerPinMap{22, 0, 0,  5}.value; break; /* TIM22_CH1/GPIO_AF5 */
	case PA_7:  res = _TimerPinMap{22, 1, 0,  5}.value; break; /* TIM22_CH2/GPIO_AF5 */
	case PA_8:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_9:  res = _TimerPinMap{22, 0, 0,  5}.value; break; /* TIM22_CH1/GPIO_AF5 */
	case PA_10: res = _TimerPinMap{22, 1, 0,  5}.value; break; /* TIM22_CH2/GPIO_AF5 */
	case PA_11: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PB_0:  res = _TimerPinMap{ 2, 2, 0,  5}.value; break; /* TIM2_CH3/GPIO_AF5 */
	case PB_1:  res = _TimerPinMap{ 2, 3, 0,  5}.value; break; /* TIM2_CH4/GPIO_AF5 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_4:  res = _TimerPinMap{22, 0, 0,  4}.value; break; /* TIM22_CH1/GPIO_AF4 */
	case PB_5:  res = _TimerPinMap{22, 1, 0,  4}.value; break; /* TIM22_CH2/GPIO_AF4 */
	case PB_6:  res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{21, 0, 0,  5}.value; break; /* TIM21_CH1/GPIO_AF5 */
	case PB_14: res = _TimerPinMap{21, 1, 0,  5}.value; break; /* TIM21_CH2/GPIO_AF5 */
#elif defined(STM32L071CBTx) || defined(STM32L071CZTx) || defined(STM32L071CBUx) || defined(STM32L071CZUx) || defined(STM32L071CBYx) || defined(STM32L071CZYx) || defined(STM32L071C8Tx) || defined(STM32L071C8Ux) || defined(STM32L071KBTx) || defined(STM32L071KZTx) || defined(STM32L071KBUx) || defined(STM32L071KZUx) || defined(STM32L071K8Ux) || defined(STM32L071RBHx) || defined(STM32L071RZHx) || defined(STM32L071RBTx) || defined(STM32L071RZTx) || defined(STM32L071VBIx) || defined(STM32L071VZIx) || defined(STM32L071VBTx) || defined(STM32L071VZTx) || defined(STM32L071V8Ix) || defined(STM32L071V8Tx) || defined(STM32L072CBTx) || defined(STM32L072CZTx) || defined(STM32L072CBUx) || defined(STM32L072CZUx) || defined(STM32L072CBYx) || defined(STM32L072CZYx) || defined(STM32L072CZEx) || defined(STM32L072KBTx) || defined(STM32L072KZTx) || defined(STM32L072KBUx) || defined(STM32L072KZUx) || defined(STM32L072RBHx) || defined(STM32L072RZHx) || defined(STM32L072RBIx) || defined(STM32L072RZIx) || defined(STM32L072RBTx) || defined(STM32L072RZTx) || defined(STM32L072VBIx) || defined(STM32L072VZIx) || defined(STM32L072VBTx) || defined(STM32L072VZTx) || defined(STM32L072V8Ix) || defined(STM32L072V8Tx) || defined(STM32L073CBTx) || defined(STM32L073CZTx) || defined(STM32L073CBUx) || defined(STM32L073CZUx) || defined(STM32L073RBHx) || defined(STM32L073RZHx) || defined(STM32L073RBTx) || defined(STM32L073RZTx) || defined(STM32L073RZIx) || defined(STM32L073VBIx) || defined(STM32L073VZIx) || defined(STM32L073VBTx) || defined(STM32L073VZTx) || defined(STM32L073V8Ix) || defined(STM32L073V8Tx) || defined(STM32L081CBTx) || defined(STM32L081CZTx) || defined(STM32L081CZUx) || defined(STM32L081KZTx) || defined(STM32L081KZUx) || defined(STM32L082CZUx) || defined(STM32L082CZYx) || defined(STM32L082KBTx) || defined(STM32L082KZTx) || defined(STM32L082KBUx) || defined(STM32L082KZUx) || defined(STM32L083CBTx) || defined(STM32L083CZTx) || defined(STM32L083CZUx) || defined(STM32L083RBHx) || defined(STM32L083RZHx) || defined(STM32L083RBTx) || defined(STM32L083RZTx) || defined(STM32L083VBIx) || defined(STM32L083VZIx) || defined(STM32L083VBTx) || defined(STM32L083VZTx) || defined(STM32L083V8Ix) || defined(STM32L083V8Tx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  2}.value; break; /* TIM2_CH1/GPIO_AF2 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  5}.value; break; /* TIM2_CH1/GPIO_AF5 */
	case PB_0:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  2}.value; break; /* LPTIM1_OUT/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  2}.value; break; /* TIM2_CH2/GPIO_AF2 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  4}.value; break; /* TIM3_CH2/GPIO_AF4 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  2}.value; break; /* TIM2_CH3/GPIO_AF2 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  2}.value; break; /* TIM2_CH4/GPIO_AF2 */
	case PB_13: res = _TimerPinMap{21, 0, 0,  6}.value; break; /* TIM21_CH1/GPIO_AF6 */
	case PB_14: res = _TimerPinMap{21, 1, 0,  6}.value; break; /* TIM21_CH2/GPIO_AF6 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  0}.value; break; /* LPTIM1_OUT/GPIO_AF0 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{21, 0, 0,  0}.value; break; /* TIM21_CH1/GPIO_AF0 */
	case PD_7:  res = _TimerPinMap{21, 1, 0,  1}.value; break; /* TIM21_CH2/GPIO_AF1 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_3:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_9:  res = _TimerPinMap{ 2, 0, 0,  0}.value; break; /* TIM2_CH1/GPIO_AF0 */
	case PE_10: res = _TimerPinMap{ 2, 1, 0,  0}.value; break; /* TIM2_CH2/GPIO_AF0 */
	case PE_11: res = _TimerPinMap{ 2, 2, 0,  0}.value; break; /* TIM2_CH3/GPIO_AF0 */
	case PE_12: res = _TimerPinMap{ 2, 3, 0,  0}.value; break; /* TIM2_CH4/GPIO_AF0 */
#endif /* GPIOE_BASE */
#elif defined(STM32L100C6Ux) || defined(STM32L100C6UxA) || defined(STM32L100R8Tx) || defined(STM32L100RBTx) || defined(STM32L100R8TxA) || defined(STM32L100RBTxA) || defined(STM32L100RCTx) || defined(STM32L151C6Tx) || defined(STM32L151C8Tx) || defined(STM32L151CBTx) || defined(STM32L151C6TxA) || defined(STM32L151C8TxA) || defined(STM32L151CBTxA) || defined(STM32L151C6Ux) || defined(STM32L151C8Ux) || defined(STM32L151CBUx) || defined(STM32L151C6UxA) || defined(STM32L151C8UxA) || defined(STM32L151CBUxA) || defined(STM32L151CCTx) || defined(STM32L151CCUx) || defined(STM32L151QCHx) || defined(STM32L151QDHx) || defined(STM32L151QEHx) || defined(STM32L151R6Hx) || defined(STM32L151R8Hx) || defined(STM32L151RBHx) || defined(STM32L151R6HxA) || defined(STM32L151R8HxA) || defined(STM32L151RBHxA) || defined(STM32L151R6Tx) || defined(STM32L151R8Tx) || defined(STM32L151RBTx) || defined(STM32L151R6TxA) || defined(STM32L151R8TxA) || defined(STM32L151RBTxA) || defined(STM32L151RCTx) || defined(STM32L151RCTxA) || defined(STM32L151RCYx) || defined(STM32L151RDTx) || defined(STM32L151RDYx) || defined(STM32L151RETx) || defined(STM32L151UCYx) || defined(STM32L151V8Hx) || defined(STM32L151VBHx) || defined(STM32L151V8HxA) || defined(STM32L151VBHxA) || defined(STM32L151V8Tx) || defined(STM32L151VBTx) || defined(STM32L151V8TxA) || defined(STM32L151VBTxA) || defined(STM32L151VCHx) || defined(STM32L151VCTx) || defined(STM32L151VCTxA) || defined(STM32L151VDTx) || defined(STM32L151VDTxX) || defined(STM32L151VDYxX) || defined(STM32L151VETx) || defined(STM32L151VEYx) || defined(STM32L151ZCTx) || defined(STM32L151ZDTx) || defined(STM32L151ZETx) || defined(STM32L152C6Tx) || defined(STM32L152C8Tx) || defined(STM32L152CBTx) || defined(STM32L152C6TxA) || defined(STM32L152C8TxA) || defined(STM32L152CBTxA) || defined(STM32L152C6Ux) || defined(STM32L152C8Ux) || defined(STM32L152CBUx) || defined(STM32L152C6UxA) || defined(STM32L152C8UxA) || defined(STM32L152CBUxA) || defined(STM32L152CCTx) || defined(STM32L152CCUx) || defined(STM32L152QCHx) || defined(STM32L152QDHx) || defined(STM32L152QEHx) || defined(STM32L152R6Hx) || defined(STM32L152R8Hx) || defined(STM32L152RBHx) || defined(STM32L152R6HxA) || defined(STM32L152R8HxA) || defined(STM32L152RBHxA) || defined(STM32L152R6Tx) || defined(STM32L152R8Tx) || defined(STM32L152RBTx) || defined(STM32L152R6TxA) || defined(STM32L152R8TxA) || defined(STM32L152RBTxA) || defined(STM32L152RCTx) || defined(STM32L152RCTxA) || defined(STM32L152RDTx) || defined(STM32L152RDYx) || defined(STM32L152RETx) || defined(STM32L152UCYx) || defined(STM32L152V8Hx) || defined(STM32L152VBHx) || defined(STM32L152V8HxA) || defined(STM32L152VBHxA) || defined(STM32L152V8Tx) || defined(STM32L152VBTx) || defined(STM32L152V8TxA) || defined(STM32L152VBTxA) || defined(STM32L152VCHx) || defined(STM32L152VCTx) || defined(STM32L152VCTxA) || defined(STM32L152VDTx) || defined(STM32L152VDTxX) || defined(STM32L152VETx) || defined(STM32L152VEYx) || defined(STM32L152ZCTx) || defined(STM32L152ZDTx) || defined(STM32L152ZETx) || defined(STM32L162QCHx) || defined(STM32L162QDHx) || defined(STM32L162RCTx) || defined(STM32L162RCTxA) || defined(STM32L162RDTx) || defined(STM32L162RDYx) || defined(STM32L162RETx) || defined(STM32L162VCHx) || defined(STM32L162VCTx) || defined(STM32L162VCTxA) || defined(STM32L162VDTx) || defined(STM32L162VDYxX) || defined(STM32L162VETx) || defined(STM32L162VEYx) || defined(STM32L162ZCTx) || defined(STM32L162ZDTx) || defined(STM32L162ZETx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PB_1:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_12: res = _TimerPinMap{10, 0, 0,  3}.value; break; /* TIM10_CH1/GPIO_AF3 */
	case PB_13: res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PB_14: res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PB_15: res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
#ifdef GPIOC_BASE
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_0:  res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PD_7:  res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{10, 0, 0,  3}.value; break; /* TIM10_CH1/GPIO_AF3 */
	case PE_1:  res = _TimerPinMap{11, 0, 0,  3}.value; break; /* TIM11_CH1/GPIO_AF3 */
	case PE_3:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 9, 0, 0,  3}.value; break; /* TIM9_CH1/GPIO_AF3 */
	case PE_6:  res = _TimerPinMap{ 9, 1, 0,  3}.value; break; /* TIM9_CH2/GPIO_AF3 */
	case PE_9:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_6:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PF_7:  res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PF_8:  res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PF_9:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
#endif /* GPIOF_BASE */
#elif defined(STM32L412C8Tx) || defined(STM32L412C8Ux) || defined(STM32L412CBTx) || defined(STM32L412CBTxP) || defined(STM32L412CBUx) || defined(STM32L412CBUxP) || defined(STM32L412K8Tx) || defined(STM32L412K8Ux) || defined(STM32L412KBTx) || defined(STM32L412KBUx) || defined(STM32L412R8Ix) || defined(STM32L412R8Tx) || defined(STM32L412RBIx) || defined(STM32L412RBIxP) || defined(STM32L412RBTx) || defined(STM32L412RBTxP) || defined(STM32L412T8Yx) || defined(STM32L412TBYx) || defined(STM32L412TBYxP) || defined(STM32L422CBTx) || defined(STM32L422CBUx) || defined(STM32L422KBTx) || defined(STM32L422KBUx) || defined(STM32L422RBIx) || defined(STM32L422RBTx) || defined(STM32L422TBYx) || defined(STM32L431CBTx) || defined(STM32L431CCTx) || defined(STM32L431CBUx) || defined(STM32L431CCUx) || defined(STM32L431CBYx) || defined(STM32L431CCYx) || defined(STM32L431KBUx) || defined(STM32L431KCUx) || defined(STM32L431RBIx) || defined(STM32L431RCIx) || defined(STM32L431RBTx) || defined(STM32L431RCTx) || defined(STM32L431RBYx) || defined(STM32L431RCYx) || defined(STM32L431VCIx) || defined(STM32L431VCTx) || defined(STM32L432KBUx) || defined(STM32L432KCUx) || defined(STM32L433CBTx) || defined(STM32L433CCTx) || defined(STM32L433CBUx) || defined(STM32L433CCUx) || defined(STM32L433CBYx) || defined(STM32L433CCYx) || defined(STM32L433RBIx) || defined(STM32L433RCIx) || defined(STM32L433RBTx) || defined(STM32L433RCTx) || defined(STM32L433RBYx) || defined(STM32L433RCYx) || defined(STM32L433RCTxP) || defined(STM32L433VCIx) || defined(STM32L433VCTx) || defined(STM32L442KCUx) || defined(STM32L443CCTx) || defined(STM32L443CCUx) || defined(STM32L443CCYx) || defined(STM32L443RCIx) || defined(STM32L443RCTx) || defined(STM32L443RCYx) || defined(STM32L443VCIx) || defined(STM32L443VCTx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{16, 0, 1, 14}.value; break; /* TIM16_CH1N/GPIO_AF14 */
	case PB_8:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_13: res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#elif defined(STM32L451CCUx) || defined(STM32L451CEUx) || defined(STM32L451RCIx) || defined(STM32L451REIx) || defined(STM32L451RCTx) || defined(STM32L451RETx) || defined(STM32L451RCYx) || defined(STM32L451REYx) || defined(STM32L451VCIx) || defined(STM32L451VEIx) || defined(STM32L451VCTx) || defined(STM32L451VETx) || defined(STM32L452CCUx) || defined(STM32L452CEUx) || defined(STM32L452RCIx) || defined(STM32L452REIx) || defined(STM32L452RCTx) || defined(STM32L452RETx) || defined(STM32L452RCYx) || defined(STM32L452REYx) || defined(STM32L452RETxP) || defined(STM32L452VCIx) || defined(STM32L452VEIx) || defined(STM32L452VCTx) || defined(STM32L452VETx) || defined(STM32L462CEUx) || defined(STM32L462REIx) || defined(STM32L462RETx) || defined(STM32L462REYx) || defined(STM32L462VEIx) || defined(STM32L462VETx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{16, 0, 1, 14}.value; break; /* TIM16_CH1N/GPIO_AF14 */
	case PB_8:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_13: res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PE_3:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#elif defined(STM32L471QEIx) || defined(STM32L471QGIx) || defined(STM32L471VETx) || defined(STM32L471VGTx) || defined(STM32L471ZEJx) || defined(STM32L471ZGJx) || defined(STM32L471ZETx) || defined(STM32L471ZGTx) || defined(STM32L475VCTx) || defined(STM32L475VETx) || defined(STM32L475VGTx) || defined(STM32L476QEIx) || defined(STM32L476QGIx) || defined(STM32L476VCTx) || defined(STM32L476VETx) || defined(STM32L476VGTx) || defined(STM32L476ZETx) || defined(STM32L476ZGTx) || defined(STM32L476ZGJx) || defined(STM32L476ZGTxP) || defined(STM32L486QGIx) || defined(STM32L486VGTx) || defined(STM32L486ZGTx) || defined(STM32L496AEIx) || defined(STM32L496AGIx) || defined(STM32L496AGIxP) || defined(STM32L496QEIx) || defined(STM32L496QGIx) || defined(STM32L496QGIxP) || defined(STM32L496VETx) || defined(STM32L496VGTx) || defined(STM32L496VGTxP) || defined(STM32L496VGYx) || defined(STM32L496VGYxP) || defined(STM32L496WGYxP) || defined(STM32L496ZETx) || defined(STM32L496ZGTx) || defined(STM32L496ZGTxP) || defined(STM32L4A6AGIx) || defined(STM32L4A6AGIxP) || defined(STM32L4A6QGIx) || defined(STM32L4A6QGIxP) || defined(STM32L4A6VGTx) || defined(STM32L4A6VGTxP) || defined(STM32L4A6VGYx) || defined(STM32L4A6VGYxP) || defined(STM32L4A6ZGTx) || defined(STM32L4A6ZGTxP) || defined(STM32L4P5AGIx) || defined(STM32L4P5AEIx) || defined(STM32L4P5AGIxP) || defined(STM32L4P5QGIx) || defined(STM32L4P5QEIx) || defined(STM32L4P5QGIxP) || defined(STM32L4P5VGTx) || defined(STM32L4P5VETx) || defined(STM32L4P5VGYx) || defined(STM32L4P5VEYx) || defined(STM32L4P5VGTxP) || defined(STM32L4P5VGYxP) || defined(STM32L4P5ZGTx) || defined(STM32L4P5ZETx) || defined(STM32L4P5ZGTxP) || defined(STM32L4Q5AGIx) || defined(STM32L4Q5QGIx) || defined(STM32L4Q5VGTx) || defined(STM32L4Q5VGYx) || defined(STM32L4Q5ZGTx) || defined(STM32L4R5AGIx) || defined(STM32L4R5AIIx) || defined(STM32L4R5QGIx) || defined(STM32L4R5QIIx) || defined(STM32L4R5VGTx) || defined(STM32L4R5VITx) || defined(STM32L4R5ZGTx) || defined(STM32L4R5ZITx) || defined(STM32L4R5ZGYx) || defined(STM32L4R5ZIYx) || defined(STM32L4R5ZITxP) || defined(STM32L4R7AIIx) || defined(STM32L4R7VITx) || defined(STM32L4R7ZITx) || defined(STM32L4R9AGIx) || defined(STM32L4R9AIIx) || defined(STM32L4R9VGTx) || defined(STM32L4R9VITx) || defined(STM32L4R9ZGJx) || defined(STM32L4R9ZIJx) || defined(STM32L4R9ZGTx) || defined(STM32L4R9ZITx) || defined(STM32L4R9ZGYx) || defined(STM32L4R9ZIYx) || defined(STM32L4R9ZIYxP) || defined(STM32L4S5AIIx) || defined(STM32L4S5QIIx) || defined(STM32L4S5VITx) || defined(STM32L4S5ZITx) || defined(STM32L4S5ZIYx) || defined(STM32L4S7AIIx) || defined(STM32L4S7VITx) || defined(STM32L4S7ZITx) || defined(STM32L4S9AIIx) || defined(STM32L4S9VITx) || defined(STM32L4S9ZIJx) || defined(STM32L4S9ZITx) || defined(STM32L4S9ZIYx) || defined(STM32L552QCIxQ) || defined(STM32L552QEIxQ) || defined(STM32L552QEIx) || defined(STM32L552QEIxP) || defined(STM32L552VCTxQ) || defined(STM32L552VETxQ) || defined(STM32L552VETx) || defined(STM32L552ZCTxQ) || defined(STM32L552ZETxQ) || defined(STM32L552ZETx) || defined(STM32L562QEIx) || defined(STM32L562QEIxP) || defined(STM32L562QEIxQ) || defined(STM32L562VETx) || defined(STM32L562VETxQ) || defined(STM32L562ZETx) || defined(STM32L562ZETxQ)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_0:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_1:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_4:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PB_5:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PB_6:  res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PB_7:  res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PB_8:  res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PB_9:  res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PC_3:  res = _TimerPinMap{27, 0, 0,  2}.value; break; /* LPTIM3_OUT/GPIO_AF2 */
	case PC_6:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PC_7:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PC_8:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PC_9:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_12: res = _TimerPinMap{ 4, 0, 0,  2}.value; break; /* TIM4_CH1/GPIO_AF2 */
	case PD_13: res = _TimerPinMap{ 4, 1, 0,  2}.value; break; /* TIM4_CH2/GPIO_AF2 */
	case PD_14: res = _TimerPinMap{ 4, 2, 0,  2}.value; break; /* TIM4_CH3/GPIO_AF2 */
	case PD_15: res = _TimerPinMap{ 4, 3, 0,  2}.value; break; /* TIM4_CH4/GPIO_AF2 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PE_1:  res = _TimerPinMap{17, 0, 0, 14}.value; break; /* TIM17_CH1/GPIO_AF14 */
	case PE_3:  res = _TimerPinMap{ 3, 0, 0,  2}.value; break; /* TIM3_CH1/GPIO_AF2 */
	case PE_4:  res = _TimerPinMap{ 3, 1, 0,  2}.value; break; /* TIM3_CH2/GPIO_AF2 */
	case PE_5:  res = _TimerPinMap{ 3, 2, 0,  2}.value; break; /* TIM3_CH3/GPIO_AF2 */
	case PE_6:  res = _TimerPinMap{ 3, 3, 0,  2}.value; break; /* TIM3_CH4/GPIO_AF2 */
	case PE_8:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PE_9:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PE_10: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PE_11: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PE_12: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PE_13: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PE_14: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	case PF_5:  res = _TimerPinMap{27, 0, 0,  2}.value; break; /* LPTIM3_OUT/GPIO_AF2 */
	case PF_6:  res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PF_7:  res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PF_8:  res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PF_9:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PF_10: res = _TimerPinMap{15, 1, 0, 14}.value; break; /* TIM15_CH2/GPIO_AF14 */
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	case PG_9:  res = _TimerPinMap{15, 0, 1, 14}.value; break; /* TIM15_CH1N/GPIO_AF14 */
	case PG_10: res = _TimerPinMap{15, 0, 0, 14}.value; break; /* TIM15_CH1/GPIO_AF14 */
	case PG_11: res = _TimerPinMap{15, 1, 0, 14}.value; break; /* TIM15_CH2/GPIO_AF14 */
	case PG_15: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PH_10: res = _TimerPinMap{ 5, 0, 0,  2}.value; break; /* TIM5_CH1/GPIO_AF2 */
	case PH_11: res = _TimerPinMap{ 5, 1, 0,  2}.value; break; /* TIM5_CH2/GPIO_AF2 */
	case PH_12: res = _TimerPinMap{ 5, 2, 0,  2}.value; break; /* TIM5_CH3/GPIO_AF2 */
	case PH_13: res = _TimerPinMap{ 8, 0, 1,  3}.value; break; /* TIM8_CH1N/GPIO_AF3 */
	case PH_14: res = _TimerPinMap{ 8, 1, 1,  3}.value; break; /* TIM8_CH2N/GPIO_AF3 */
	case PH_15: res = _TimerPinMap{ 8, 2, 1,  3}.value; break; /* TIM8_CH3N/GPIO_AF3 */
#endif /* GPIOH_BASE */
#ifdef GPIOI_BASE
	case PI_0:  res = _TimerPinMap{ 5, 3, 0,  2}.value; break; /* TIM5_CH4/GPIO_AF2 */
	case PI_2:  res = _TimerPinMap{ 8, 3, 0,  3}.value; break; /* TIM8_CH4/GPIO_AF3 */
	case PI_5:  res = _TimerPinMap{ 8, 0, 0,  3}.value; break; /* TIM8_CH1/GPIO_AF3 */
	case PI_6:  res = _TimerPinMap{ 8, 1, 0,  3}.value; break; /* TIM8_CH2/GPIO_AF3 */
	case PI_7:  res = _TimerPinMap{ 8, 2, 0,  3}.value; break; /* TIM8_CH3/GPIO_AF3 */
#endif /* GPIOI_BASE */
#elif defined(STM32WB30CEUx) || defined(STM32WB35CCUx) || defined(STM32WB35CEUx) || defined(STM32WB50CGUx) || defined(STM32WB55CCUx) || defined(STM32WB55CEUx) || defined(STM32WB55CGUx) || defined(STM32WB55RCVx) || defined(STM32WB55REVx) || defined(STM32WB55RGVx) || defined(STM32WB55VCQx) || defined(STM32WB55VCYx) || defined(STM32WB55VEQx) || defined(STM32WB55VEYx) || defined(STM32WB55VGQx) || defined(STM32WB55VGYx) || defined(STM32WB55VYYx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{16, 0, 1, 14}.value; break; /* TIM16_CH1N/GPIO_AF14 */
	case PB_7:  res = _TimerPinMap{17, 0, 1, 14}.value; break; /* TIM17_CH1N/GPIO_AF14 */
	case PB_8:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_13: res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PD_14: res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PD_15: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PE_1:  res = _TimerPinMap{17, 0, 0, 14}.value; break; /* TIM17_CH1/GPIO_AF14 */
#endif /* GPIOE_BASE */
#elif defined(STM32WB5MMGHx)
	case PA_0:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_1:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PA_2:  res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PA_3:  res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PA_4:  res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PA_5:  res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PA_6:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PA_7:  res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PA_8:  res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PA_9:  res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
	case PA_10: res = _TimerPinMap{ 1, 2, 0,  1}.value; break; /* TIM1_CH3/GPIO_AF1 */
	case PA_11: res = _TimerPinMap{ 1, 3, 0,  1}.value; break; /* TIM1_CH4/GPIO_AF1 */
	case PA_14: res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PA_15: res = _TimerPinMap{ 2, 0, 0,  1}.value; break; /* TIM2_CH1/GPIO_AF1 */
	case PB_2:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
	case PB_3:  res = _TimerPinMap{ 2, 1, 0,  1}.value; break; /* TIM2_CH2/GPIO_AF1 */
	case PB_6:  res = _TimerPinMap{16, 0, 1, 14}.value; break; /* TIM16_CH1N/GPIO_AF14 */
	case PB_7:  res = _TimerPinMap{17, 0, 1, 14}.value; break; /* TIM17_CH1N/GPIO_AF14 */
	case PB_8:  res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_9:  res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
	case PB_10: res = _TimerPinMap{ 2, 2, 0,  1}.value; break; /* TIM2_CH3/GPIO_AF1 */
	case PB_11: res = _TimerPinMap{ 2, 3, 0,  1}.value; break; /* TIM2_CH4/GPIO_AF1 */
	case PB_13: res = _TimerPinMap{ 1, 0, 1,  1}.value; break; /* TIM1_CH1N/GPIO_AF1 */
	case PB_14: res = _TimerPinMap{ 1, 1, 1,  1}.value; break; /* TIM1_CH2N/GPIO_AF1 */
	case PB_15: res = _TimerPinMap{ 1, 2, 1,  1}.value; break; /* TIM1_CH3N/GPIO_AF1 */
#ifdef GPIOC_BASE
	case PC_1:  res = _TimerPinMap{25, 0, 0,  1}.value; break; /* LPTIM1_OUT/GPIO_AF1 */
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	case PD_13: res = _TimerPinMap{26, 0, 0, 14}.value; break; /* LPTIM2_OUT/GPIO_AF14 */
	case PD_14: res = _TimerPinMap{ 1, 0, 0,  1}.value; break; /* TIM1_CH1/GPIO_AF1 */
	case PD_15: res = _TimerPinMap{ 1, 1, 0,  1}.value; break; /* TIM1_CH2/GPIO_AF1 */
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	case PE_0:  res = _TimerPinMap{16, 0, 0, 14}.value; break; /* TIM16_CH1/GPIO_AF14 */
	case PE_1:  res = _TimerPinMap{17, 0, 0, 14}.value; break; /* TIM17_CH1/GPIO_AF14 */
#endif /* GPIOE_BASE */
#else
#warning There is no timer pin mapping defined for this target. analogWrite() via PWM is disabled.
#endif
	default: break;
	}
	this->value = res;
}


uint32_t _TimerPinMap::getInstanceBaseAddr() const {
	uint32_t res = 0;
	switch (this->getInstanceNum()) {
#ifdef TIM1
	case  1: res = TIM1_BASE; break;
#endif /* TIM1 */
#ifdef TIM2
	case  2: res = TIM2_BASE; break;
#endif /* TIM2 */
#ifdef TIM3
	case  3: res = TIM3_BASE; break;
#endif /* TIM3 */
#ifdef TIM4
	case  4: res = TIM4_BASE; break;
#endif /* TIM4 */
#ifdef TIM5
	case  5: res = TIM5_BASE; break;
#endif /* TIM5 */
#ifdef TIM6
	case  6: res = TIM6_BASE; break;
#endif /* TIM6 */
#ifdef TIM7
	case  7: res = TIM7_BASE; break;
#endif /* TIM7 */
#ifdef TIM8
	case  8: res = TIM8_BASE; break;
#endif /* TIM8 */
#ifdef TIM9
	case  9: res = TIM9_BASE; break;
#endif /* TIM9 */
#ifdef TIM10
	case 10: res = TIM10_BASE; break;
#endif /* TIM10 */
#ifdef TIM11
	case 11: res = TIM11_BASE; break;
#endif /* TIM11 */
#ifdef TIM12
	case 12: res = TIM12_BASE; break;
#endif /* TIM12 */
#ifdef TIM13
	case 13: res = TIM13_BASE; break;
#endif /* TIM13 */
#ifdef TIM14
	case 14: res = TIM14_BASE; break;
#endif /* TIM14 */
#ifdef TIM15
	case 15: res = TIM15_BASE; break;
#endif /* TIM15 */
#ifdef TIM16
	case 16: res = TIM16_BASE; break;
#endif /* TIM16 */
#ifdef TIM17
	case 17: res = TIM17_BASE; break;
#endif /* TIM17 */
#ifdef TIM18
	case 18: res = TIM18_BASE; break;
#endif /* TIM18 */
#ifdef TIM19
	case 19: res = TIM19_BASE; break;
#endif /* TIM19 */
#ifdef TIM20
	case 20: res = TIM20_BASE; break;
#endif /* TIM20 */
#ifdef TIM21
	case 21: res = TIM21_BASE; break;
#endif /* TIM21 */
#ifdef TIM22
	case 22: res = TIM22_BASE; break;
#endif /* TIM22 */
#ifdef TIM23
	case 23: res = TIM23_BASE; break;
#endif /* TIM23 */
#ifdef TIM24
	case 24: res = TIM24_BASE; break;
#endif /* TIM24 */
#ifdef LPTIM1
	case 25: res = LPTIM1_BASE; break;
#endif /* LPTIM1 */
#ifdef LPTIM2
	case 26: res = LPTIM2_BASE; break;
#endif /* LPTIM2 */
#ifdef LPTIM3
	case 27: res = LPTIM3_BASE; break;
#endif /* LPTIM3 */
#ifdef LPTIM4
	case 28: res = LPTIM4_BASE; break;
#endif /* LPTIM4 */
#ifdef LPTIM5
	case 29: res = LPTIM5_BASE; break;
#endif /* LPTIM5 */
	default: break;
	}
	return res;
}


_TimerPinMap::ClockSource _TimerPinMap::getClockSource(const uint16_t instNum) {
	if (instNum <= 0 || instNum >= 30) return ClockSource_Unknown;
#ifndef LPTIM_PRESCALER_DIV1
	if (instNum >= 25) return ClockSource_Unknown;
#else /* LPTIM_PRESCALER_DIV1 */
	if (instNum >= 25) {
		ClockSource res = ClockSource_Unknown;
		switch (instNum) {
#ifdef RCC_PERIPHCLK_LPTIM1
		case 25: /* LPTIM1 */
			switch (__HAL_RCC_GET_LPTIM1_SOURCE()) {
#ifdef RCC_LPTIM1CLKSOURCE_CLKP
			case RCC_LPTIM1CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM1CLKSOURCE_CLKP */
#ifdef RCC_LPTIM1CLKSOURCE_D2PCLK1
			case RCC_LPTIM1CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM1CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM1CLKSOURCE_D3PCLK1
			case RCC_LPTIM1CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM1CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM1CLKSOURCE_HSI
			case RCC_LPTIM1CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM1CLKSOURCE_HSI */
#ifdef RCC_LPTIM1CLKSOURCE_LSE
			case RCC_LPTIM1CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM1CLKSOURCE_LSE */
#ifdef RCC_LPTIM1CLKSOURCE_LSI
			case RCC_LPTIM1CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM1CLKSOURCE_LSI */
#ifdef RCC_LPTIM1CLKSOURCE_PCLK1
#if !defined(RCC_LPTIM1CLKSOURCE_D2PCLK1) || RCC_LPTIM1CLKSOURCE_PCLK1 != RCC_LPTIM1CLKSOURCE_D2PCLK1
			case RCC_LPTIM1CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif
#endif /* RCC_LPTIM1CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM1CLKSOURCE_PCLK2
			case RCC_LPTIM1CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM1CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM1CLKSOURCE_PCLK3
			case RCC_LPTIM1CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM1CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM1CLKSOURCE_PCLK4
			case RCC_LPTIM1CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM1CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM1CLKSOURCE_PLL2
			case RCC_LPTIM1CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM1CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM1CLKSOURCE_PLL3
			case RCC_LPTIM1CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM1CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#endif /* RCC_PERIPHCLK_LPTIM1 */
#ifdef RCC_PERIPHCLK_LPTIM2
		case 26: /* LPTIM2 */
			switch (__HAL_RCC_GET_LPTIM2_SOURCE()) {
#ifdef RCC_LPTIM2CLKSOURCE_CLKP
			case RCC_LPTIM2CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM2CLKSOURCE_CLKP */
#ifdef RCC_LPTIM2CLKSOURCE_D2PCLK1
			case RCC_LPTIM2CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM2CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM2CLKSOURCE_D3PCLK1
			case RCC_LPTIM2CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM2CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM2CLKSOURCE_HSI
			case RCC_LPTIM2CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM2CLKSOURCE_HSI */
#ifdef RCC_LPTIM2CLKSOURCE_LSE
			case RCC_LPTIM2CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM2CLKSOURCE_LSE */
#ifdef RCC_LPTIM2CLKSOURCE_LSI
			case RCC_LPTIM2CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM2CLKSOURCE_LSI */
#ifdef RCC_LPTIM2CLKSOURCE_PCLK1
			case RCC_LPTIM2CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM2CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM2CLKSOURCE_PCLK2
			case RCC_LPTIM2CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM2CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM2CLKSOURCE_PCLK3
			case RCC_LPTIM2CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM2CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM2CLKSOURCE_PCLK4
#if !defined(RCC_LPTIM2CLKSOURCE_D3PCLK1) || RCC_LPTIM2CLKSOURCE_PCLK4 != RCC_LPTIM2CLKSOURCE_D3PCLK1
			case RCC_LPTIM2CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif
#endif /* RCC_LPTIM2CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM2CLKSOURCE_PLL2
			case RCC_LPTIM2CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM2CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM2CLKSOURCE_PLL3
			case RCC_LPTIM2CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM2CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#endif /* RCC_PERIPHCLK_LPTIM2 */
#ifdef RCC_PERIPHCLK_LPTIM345
		case 27: /* LPTIM3 */
		case 28: /* LPTIM4 */
		case 29: /* LPTIM5 */
			switch (__HAL_RCC_GET_LPTIM345_SOURCE()) {
#ifdef RCC_LPTIM345CLKSOURCE_CLKP
			case RCC_LPTIM345CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM345CLKSOURCE_CLKP */
#ifdef RCC_LPTIM345CLKSOURCE_D2PCLK1
			case RCC_LPTIM345CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM345CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM345CLKSOURCE_D3PCLK1
			case RCC_LPTIM345CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM345CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM345CLKSOURCE_HSI
			case RCC_LPTIM345CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM345CLKSOURCE_HSI */
#ifdef RCC_LPTIM345CLKSOURCE_LSE
			case RCC_LPTIM345CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM345CLKSOURCE_LSE */
#ifdef RCC_LPTIM345CLKSOURCE_LSI
			case RCC_LPTIM345CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM345CLKSOURCE_LSI */
#ifdef RCC_LPTIM345CLKSOURCE_PCLK1
			case RCC_LPTIM345CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM345CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM345CLKSOURCE_PCLK2
			case RCC_LPTIM345CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM345CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM345CLKSOURCE_PCLK3
			case RCC_LPTIM345CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM345CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM345CLKSOURCE_PCLK4
#if !defined(RCC_LPTIM2CLKSOURCE_D3PCLK1) || RCC_LPTIM345CLKSOURCE_PCLK4 != RCC_LPTIM2CLKSOURCE_D3PCLK1
			case RCC_LPTIM345CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif
#endif /* RCC_LPTIM345CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM345CLKSOURCE_PLL2
			case RCC_LPTIM345CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM345CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM345CLKSOURCE_PLL3
			case RCC_LPTIM345CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM345CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#else /* not RCC_PERIPHCLK_LPTIM345 */
#ifdef RCC_PERIPHCLK_LPTIM3
		case 27: /* LPTIM3 */
			switch (__HAL_RCC_GET_LPTIM3_SOURCE()) {
#ifdef RCC_LPTIM3CLKSOURCE_CLKP
			case RCC_LPTIM3CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM3CLKSOURCE_CLKP */
#ifdef RCC_LPTIM3CLKSOURCE_D2PCLK1
			case RCC_LPTIM3CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM3CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM3CLKSOURCE_D3PCLK1
			case RCC_LPTIM3CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM3CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM3CLKSOURCE_HSI
			case RCC_LPTIM3CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM3CLKSOURCE_HSI */
#ifdef RCC_LPTIM3CLKSOURCE_LSE
			case RCC_LPTIM3CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM3CLKSOURCE_LSE */
#ifdef RCC_LPTIM3CLKSOURCE_LSI
			case RCC_LPTIM3CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM3CLKSOURCE_LSI */
#ifdef RCC_LPTIM3CLKSOURCE_PCLK1
			case RCC_LPTIM3CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM3CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM3CLKSOURCE_PCLK2
			case RCC_LPTIM3CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM3CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM3CLKSOURCE_PCLK3
			case RCC_LPTIM3CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM3CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM3CLKSOURCE_PCLK4
			case RCC_LPTIM3CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM3CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM3CLKSOURCE_PLL2
			case RCC_LPTIM3CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM3CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM3CLKSOURCE_PLL3
			case RCC_LPTIM3CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM3CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#endif /* RCC_PERIPHCLK_LPTIM3 */
#ifdef RCC_PERIPHCLK_LPTIM4
		case 28: /* LPTIM4 */
			switch (__HAL_RCC_GET_LPTIM4_SOURCE()) {
#ifdef RCC_LPTIM4CLKSOURCE_CLKP
			case RCC_LPTIM4CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM4CLKSOURCE_CLKP */
#ifdef RCC_LPTIM4CLKSOURCE_D2PCLK1
			case RCC_LPTIM4CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM4CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM4CLKSOURCE_D3PCLK1
			case RCC_LPTIM4CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM4CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM4CLKSOURCE_HSI
			case RCC_LPTIM4CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM4CLKSOURCE_HSI */
#ifdef RCC_LPTIM4CLKSOURCE_LSE
			case RCC_LPTIM4CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM4CLKSOURCE_LSE */
#ifdef RCC_LPTIM4CLKSOURCE_LSI
			case RCC_LPTIM4CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM4CLKSOURCE_LSI */
#ifdef RCC_LPTIM4CLKSOURCE_PCLK1
			case RCC_LPTIM4CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM4CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM4CLKSOURCE_PCLK2
			case RCC_LPTIM4CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM4CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM4CLKSOURCE_PCLK3
			case RCC_LPTIM4CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM4CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM4CLKSOURCE_PCLK4
			case RCC_LPTIM4CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM4CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM4CLKSOURCE_PLL2
			case RCC_LPTIM4CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM4CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM4CLKSOURCE_PLL3
			case RCC_LPTIM4CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM4CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#endif /* RCC_PERIPHCLK_LPTIM4 */
#ifdef RCC_PERIPHCLK_LPTIM5
		case 29: /* LPTIM5 */
			switch (__HAL_RCC_GET_LPTIM5_SOURCE()) {
#ifdef RCC_LPTIM5CLKSOURCE_CLKP
			case RCC_LPTIM5CLKSOURCE_CLKP:    res = ClockSource_CLKP; break;
#endif /* RCC_LPTIM5CLKSOURCE_CLKP */
#ifdef RCC_LPTIM5CLKSOURCE_D2PCLK1
			case RCC_LPTIM5CLKSOURCE_D2PCLK1: res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM5CLKSOURCE_D2PCLK1 */
#ifdef RCC_LPTIM5CLKSOURCE_D3PCLK1
			case RCC_LPTIM5CLKSOURCE_D3PCLK1: res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM5CLKSOURCE_D3PCLK1 */
#ifdef RCC_LPTIM5CLKSOURCE_HSI
			case RCC_LPTIM5CLKSOURCE_HSI:     res = ClockSource_HSI; break;
#endif /* RCC_LPTIM5CLKSOURCE_HSI */
#ifdef RCC_LPTIM5CLKSOURCE_LSE
			case RCC_LPTIM5CLKSOURCE_LSE:     res = ClockSource_LSE; break;
#endif /* RCC_LPTIM5CLKSOURCE_LSE */
#ifdef RCC_LPTIM5CLKSOURCE_LSI
			case RCC_LPTIM5CLKSOURCE_LSI:     res = ClockSource_LSI; break;
#endif /* RCC_LPTIM5CLKSOURCE_LSI */
#ifdef RCC_LPTIM5CLKSOURCE_PCLK1
			case RCC_LPTIM5CLKSOURCE_PCLK1:   res = ClockSource_PCLK1; break;
#endif /* RCC_LPTIM5CLKSOURCE_PCLK1 */
#ifdef RCC_LPTIM5CLKSOURCE_PCLK2
			case RCC_LPTIM5CLKSOURCE_PCLK2:   res = ClockSource_PCLK2; break;
#endif /* RCC_LPTIM5CLKSOURCE_PCLK2 */
#ifdef RCC_LPTIM5CLKSOURCE_PCLK3
			case RCC_LPTIM5CLKSOURCE_PCLK3:   res = ClockSource_PCLK3; break;
#endif /* RCC_LPTIM5CLKSOURCE_PCLK3 */
#ifdef RCC_LPTIM5CLKSOURCE_PCLK4
			case RCC_LPTIM5CLKSOURCE_PCLK4:   res = ClockSource_PCLK4; break;
#endif /* RCC_LPTIM5CLKSOURCE_PCLK4 */
#ifdef RCC_LPTIM5CLKSOURCE_PLL2
			case RCC_LPTIM5CLKSOURCE_PLL2:    res = ClockSource_PLL2; break;
#endif /* RCC_LPTIM5CLKSOURCE_PLL2 */
#ifdef RCC_LPTIM5CLKSOURCE_PLL3
			case RCC_LPTIM5CLKSOURCE_PLL3:    res = ClockSource_PLL3; break;
#endif /* RCC_LPTIM5CLKSOURCE_PLL3 */
			default: break;
			}
			break;
#endif /* RCC_PERIPHCLK_LPTIM5 */
#endif /* not RCC_PERIPHCLK_LPTIM345 */
		default:
			break;
		}
		return res;
	}
#endif /* LPTIM_PRESCALER_DIV1 */	
#ifdef STM32F0
	return ClockSource_PCLK1;
#else /* not STM32F0 */
#define MAP_INST(instNum, src) | (uint32_t((src) >> 1) << ((instNum) - 1))
	/* get source clock depending on the timer instance (LSB == instance 1) (PCLK1 or PCLK2) */
	const uint32_t clkSrcMap = uint32_t(0
		MAP_INST(1, 2)
		MAP_INST(2, 1)
		MAP_INST(3, 1)
		MAP_INST(4, 1)
		MAP_INST(5, 1)
		MAP_INST(6, 1)
		MAP_INST(7, 1)
		MAP_INST(8, 2)
		MAP_INST(9, 2)
		MAP_INST(10, 2)
		MAP_INST(11, 2)
		MAP_INST(12, 1)
		MAP_INST(13, 1)
		MAP_INST(14, 1)
		MAP_INST(15, 2)
		MAP_INST(16, 2)
		MAP_INST(17, 2)
		MAP_INST(18, 1)
		MAP_INST(19, 2)
		MAP_INST(20, 2)
		MAP_INST(21, 2)
		MAP_INST(22, 2)
		MAP_INST(23, 1)
		MAP_INST(24, 1)
		/* low power timers were handled previously */
	);
	return (((clkSrcMap >> (instNum - 1)) & 1) != 0) ? ClockSource_PCLK2 : ClockSource_PCLK1;
#undef MAP_INST
#endif /* not STM32F0 */
}


__attribute__((weak))
uint32_t _TimerPinMap::getClockFrequency(const _TimerPinMap::ClockSource clkSrc) {
	switch (clkSrc) {
	case ClockSource_SYSCLK:
		return SystemCoreClock;
		break;
#ifdef LL_RCC_CLKP_CLKSOURCE
	case ClockSource_CLKP:
		return LL_RCC_GetCLKPClockFreq(LL_RCC_CLKP_CLKSOURCE);
		break;
#endif /* LL_RCC_CLKP_CLKSOURCE */
#ifdef CSI_VALUE
	case ClockSource_CSI:
		return CSI_VALUE;
		break;
#endif /* CSI_VALUE */
#ifdef MSI_VALUE
	case ClockSource_MSI:
		return MSI_VALUE;
		break;
#endif /* MSI_VALUE */
#ifdef HSI_VALUE
	case ClockSource_HSI:
		return HSI_VALUE;
		break;
#endif /* HSI_VALUE */
#ifdef LSE_VALUE
	case ClockSource_LSE:
		return LSE_VALUE;
		break;
#endif /* LSE_VALUE */
#ifdef LSI_VALUE
	case ClockSource_LSI:
#ifdef LL_RCC_LSI_PREDIV_128
		if (LL_RCC_LSI_GetPrediv() == LL_RCC_LSI_PREDIV_128) return LSI_VALUE / 128U;
#endif /* LL_RCC_LSI_PREDIV_128 */
		return LSI_VALUE;
		break;
#endif /* LSI_VALUE */
#ifdef STM32MP1
	case ClockSource_PCLK1:
	case ClockSource_PCLK2:
		return HAL_RCCEx_GetPeriphCLKFreq((clkSrc == ClockSource_PCLK1) ? RCC_PERIPHCLK_TIMG1 : RCC_PERIPHCLK_TIMG2);
#else /* not STM32MP1 */
	case ClockSource_PCLK1:
		return RCC_GetPCLK1ClockFreq_Wrapper();
		break;
	case ClockSource_PCLK2:
		return RCC_GetPCLK2ClockFreq_Wrapper();
		break;
	case ClockSource_PCLK3:
		return RCC_GetPCLK3ClockFreq_Wrapper();
		break;
	case ClockSource_PCLK4:
		return RCC_GetPCLK4ClockFreq_Wrapper();
		break;
#endif /* not STM32MP1 */
#ifdef LL_RCC_PLL2_IsReady
	case ClockSource_PLL2:
		{
			LL_PLL_ClocksTypeDef pllClocks;
			LL_RCC_GetPLL2ClockFreq(&pllClocks);
			return pllClocks.PLL_P_Frequency;
		}
		break;
#endif /* LL_RCC_PLL2_IsReady */
#ifdef LL_RCC_PLL3_IsReady
	case ClockSource_PLL3:
		{
			LL_PLL_ClocksTypeDef pllClocks;
			LL_RCC_GetPLL3ClockFreq(&pllClocks);
			return pllClocks.PLL_R_Frequency;
		}
		break;
#endif /* LL_RCC_PLL3_IsReady */
	default:
		break;
	}
	return 0;
}


uint32_t _TimerPinMap::getChannel() const {
	uint32_t res = 0;
	switch (this->getChannelNum()) {
#ifdef TIM_CHANNEL_1
	case 0: res = TIM_CHANNEL_1; break;
#endif /* TIM_CHANNEL_1 */
#ifdef TIM_CHANNEL_2
	case 1: res = TIM_CHANNEL_2; break;
#endif /* TIM_CHANNEL_2 */
#ifdef TIM_CHANNEL_3
	case 2: res = TIM_CHANNEL_3; break;
#endif /* TIM_CHANNEL_3 */
#ifdef TIM_CHANNEL_4
	case 3: res = TIM_CHANNEL_4; break;
#endif /* TIM_CHANNEL_4 */
	default: break;
	}
	return res;
}


#if defined(LL_TIM_CHANNEL_CH1) || defined(LL_TIM_CHANNEL_CH1N) /* STM32 HAL LL TIM header was included */
uint32_t _TimerPinMap::getChannelLL() const {
	uint32_t res = 0;
	if (this->getInvertedFlag() == 0) {
		switch (this->getChannelNum()) {
#ifdef LL_TIM_CHANNEL_CH1
		case 0: res = LL_TIM_CHANNEL_CH1; break;
#endif /* LL_TIM_CHANNEL_CH1 */
#ifdef LL_TIM_CHANNEL_CH2
		case 1: res = LL_TIM_CHANNEL_CH2; break;
#endif /* LL_TIM_CHANNEL_CH2 */
#ifdef LL_TIM_CHANNEL_CH3
		case 2: res = LL_TIM_CHANNEL_CH3; break;
#endif /* LL_TIM_CHANNEL_CH3 */
#ifdef LL_TIM_CHANNEL_CH4
		case 3: res = LL_TIM_CHANNEL_CH4; break;
#endif /* LL_TIM_CHANNEL_CH4 */
		default: break;
		}
	}
#if defined(LL_TIM_CHANNEL_CH1N) || defined(LL_TIM_CHANNEL_CH2N) || defined(LL_TIM_CHANNEL_CH3N) || defined(LL_TIM_CHANNEL_CH4N)
	else {
		switch (this->getChannelNum()) {
#ifdef LL_TIM_CHANNEL_CH1N
		case 0: res = LL_TIM_CHANNEL_CH1N; break;
#endif /* LL_TIM_CHANNEL_CH1N */
#ifdef LL_TIM_CHANNEL_CH2N
		case 1: res = LL_TIM_CHANNEL_CH2N; break;
#endif /* LL_TIM_CHANNEL_CH2N */
#ifdef LL_TIM_CHANNEL_CH3N
		case 2: res = LL_TIM_CHANNEL_CH3N; break;
#endif /* LL_TIM_CHANNEL_CH3N */
#ifdef LL_TIM_CHANNEL_CH4N
		case 3: res = LL_TIM_CHANNEL_CH4N; break;
#endif /* LL_TIM_CHANNEL_CH4N */
		default: break;
		}
	}
#endif /* LL_TIM_CHANNEL_CH1N or LL_TIM_CHANNEL_CH2N or LL_TIM_CHANNEL_CH3N or LL_TIM_CHANNEL_CH4N */
	return res;
}
#endif /* STM32 HAL LL TIM header was included */


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
_TimerPinMap::TimHandle * _TimerPinMap::getInternalTimHandleFromId(TIM_TypeDef * hTim) {
	TimHandle * res = NULL;
	switch (reinterpret_cast<uintptr_t>(hTim)) {
#ifdef TIM1
	case TIM1_BASE: res = tim1Handle; break;
#endif /* TIM1 */
#ifdef TIM2
	case TIM2_BASE: res = tim2Handle; break;
#endif /* TIM2 */
#ifdef TIM3
	case TIM3_BASE: res = tim3Handle; break;
#endif /* TIM3 */
#ifdef TIM4
	case TIM4_BASE: res = tim4Handle; break;
#endif /* TIM4 */
#ifdef TIM5
	case TIM5_BASE: res = tim5Handle; break;
#endif /* TIM5 */
#ifdef TIM6
	case TIM6_BASE: res = tim6Handle; break;
#endif /* TIM6 */
#ifdef TIM7
	case TIM7_BASE: res = tim7Handle; break;
#endif /* TIM7 */
#ifdef TIM8
	case TIM8_BASE: res = tim8Handle; break;
#endif /* TIM8 */
#ifdef TIM9
	case TIM9_BASE: res = tim9Handle; break;
#endif /* TIM9 */
#ifdef TIM10
	case TIM10_BASE: res = tim10Handle; break;
#endif /* TIM10 */
#ifdef TIM11
	case TIM11_BASE: res = tim11Handle; break;
#endif /* TIM11 */
#ifdef TIM12
	case TIM12_BASE: res = tim12Handle; break;
#endif /* TIM12 */
#ifdef TIM13
	case TIM13_BASE: res = tim13Handle; break;
#endif /* TIM13 */
#ifdef TIM14
	case TIM14_BASE: res = tim14Handle; break;
#endif /* TIM14 */
#ifdef TIM15
	case TIM15_BASE: res = tim15Handle; break;
#endif /* TIM15 */
#ifdef TIM16
	case TIM16_BASE: res = tim16Handle; break;
#endif /* TIM16 */
#ifdef TIM17
	case TIM17_BASE: res = tim17Handle; break;
#endif /* TIM17 */
#ifdef TIM18
	case TIM18_BASE: res = tim18Handle; break;
#endif /* TIM18 */
#ifdef TIM19
	case TIM19_BASE: res = tim19Handle; break;
#endif /* TIM19 */
#ifdef TIM20
	case TIM20_BASE: res = tim20Handle; break;
#endif /* TIM20 */
#ifdef TIM21
	case TIM21_BASE: res = tim21Handle; break;
#endif /* TIM21 */
#ifdef TIM22
	case TIM22_BASE: res = tim22Handle; break;
#endif /* TIM22 */
#ifdef TIM23
	case TIM23_BASE: res = tim23Handle; break;
#endif /* TIM23 */
#ifdef TIM24
	case TIM24_BASE: res = tim24Handle; break;
#endif /* TIM24 */
	default: break;
	}
	if (res != NULL && res->tim.Instance == NULL) {
		res->tim.Instance = hTim;
		res->tim.State = HAL_TIM_STATE_RESET;
	}
	return res;
}


uint32_t _TimerPinMap::getInstanceNumFromId(TIM_TypeDef * hTim) {
	uint8_t res = 0;
	switch (reinterpret_cast<uintptr_t>(hTim)) {
#ifdef TIM1
	case TIM1_BASE: res = 1; break;
#endif /* TIM1 */
#ifdef TIM2
	case TIM2_BASE: res = 2; break;
#endif /* TIM2 */
#ifdef TIM3
	case TIM3_BASE: res = 3; break;
#endif /* TIM3 */
#ifdef TIM4
	case TIM4_BASE: res = 4; break;
#endif /* TIM4 */
#ifdef TIM5
	case TIM5_BASE: res = 5; break;
#endif /* TIM5 */
#ifdef TIM6
	case TIM6_BASE: res = 6; break;
#endif /* TIM6 */
#ifdef TIM7
	case TIM7_BASE: res = 7; break;
#endif /* TIM7 */
#ifdef TIM8
	case TIM8_BASE: res = 8; break;
#endif /* TIM8 */
#ifdef TIM9
	case TIM9_BASE: res = 9; break;
#endif /* TIM9 */
#ifdef TIM10
	case TIM10_BASE: res = 10; break;
#endif /* TIM10 */
#ifdef TIM11
	case TIM11_BASE: res = 11; break;
#endif /* TIM11 */
#ifdef TIM12
	case TIM12_BASE: res = 12; break;
#endif /* TIM12 */
#ifdef TIM13
	case TIM13_BASE: res = 13; break;
#endif /* TIM13 */
#ifdef TIM14
	case TIM14_BASE: res = 14; break;
#endif /* TIM14 */
#ifdef TIM15
	case TIM15_BASE: res = 15; break;
#endif /* TIM15 */
#ifdef TIM16
	case TIM16_BASE: res = 16; break;
#endif /* TIM16 */
#ifdef TIM17
	case TIM17_BASE: res = 17; break;
#endif /* TIM17 */
#ifdef TIM18
	case TIM18_BASE: res = 18; break;
#endif /* TIM18 */
#ifdef TIM19
	case TIM19_BASE: res = 19; break;
#endif /* TIM19 */
#ifdef TIM20
	case TIM20_BASE: res = 20; break;
#endif /* TIM20 */
#ifdef TIM21
	case TIM21_BASE: res = 21; break;
#endif /* TIM21 */
#ifdef TIM22
	case TIM22_BASE: res = 22; break;
#endif /* TIM22 */
#ifdef TIM23
	case TIM23_BASE: res = 23; break;
#endif /* TIM23 */
#ifdef TIM24
	case TIM24_BASE: res = 24; break;
#endif /* TIM24 */
	default: break;
	}
	return uint32_t(res);
}
#endif /* TIM_CLOCKPRESCALER_DIV1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
_TimerPinMap::LpTimHandle * _TimerPinMap::getInternalLpTimHandleFromId(LPTIM_TypeDef * hLpTim) {
	LpTimHandle * res = NULL;
	switch (reinterpret_cast<uintptr_t>(hLpTim)) {
#ifdef LPTIM1
	case LPTIM1_BASE: res = lpTim1Handle; break;
#endif /* LPTIM1 */
#ifdef LPTIM2
	case LPTIM2_BASE: res = lpTim2Handle; break;
#endif /* LPTIM2 */
#ifdef LPTIM3
	case LPTIM3_BASE: res = lpTim3Handle; break;
#endif /* LPTIM3 */
#ifdef LPTIM4
	case LPTIM4_BASE: res = lpTim4Handle; break;
#endif /* LPTIM4 */
#ifdef LPTIM5
	case LPTIM5_BASE: res = lpTim5Handle; break;
#endif /* LPTIM5 */
	default: break;
	}
	if (res != NULL && res->lptim.Instance == NULL) {
		res->lptim.Instance = hLpTim;
		res->lptim.State = HAL_LPTIM_STATE_RESET;
	}
	return res;
}


uint32_t _TimerPinMap::getInstanceNumFromId(LPTIM_TypeDef * hLpTim) {
	uint8_t res = 0;
	switch (reinterpret_cast<uintptr_t>(hLpTim)) {
#ifdef LPTIM1
	case LPTIM1_BASE: res = 25; break;
#endif /* LPTIM1 */
#ifdef LPTIM2
	case LPTIM2_BASE: res = 26; break;
#endif /* LPTIM2 */
#ifdef LPTIM3
	case LPTIM3_BASE: res = 27; break;
#endif /* LPTIM3 */
#ifdef LPTIM4
	case LPTIM4_BASE: res = 28; break;
#endif /* LPTIM4 */
#ifdef LPTIM5
	case LPTIM5_BASE: res = 29; break;
#endif /* LPTIM5 */
	default: break;
	}
	return uint32_t(res);
}
#endif /* LPTIM_PRESCALER_DIV1 */


#ifdef STM32F1
void _TimerPinMap::f1PinModeTimer(const uint32_t pin, const uint16_t timer) {
	const uint32_t llPin = getInternalPin(pin);
#if defined(LL_SYSCFG_PIN_RMP_PA11) && defined(LL_SYSCFG_PIN_RMP_PA12)
	/* handle pin remap */
	if (pin >= PA_9 && pin <= PA_12) {
		switch (pin) {
		case PA_9:
			LL_SYSCFG_EnablePinRemap(LL_SYSCFG_PIN_RMP_PA11);
			break;
		case PA_11:
			LL_SYSCFG_DisablePinRemap(LL_SYSCFG_PIN_RMP_PA11);
			break;
		case PA_10:
			LL_SYSCFG_EnablePinRemap(LL_SYSCFG_PIN_RMP_PA12);
			break;
		case PA_12:
			LL_SYSCFG_DisablePinRemap(LL_SYSCFG_PIN_RMP_PA12);
			break;
		default:
			break;
		}
	}
#endif
	GPIO_TypeDef * gpioPort = enableGpioClock(digitalPinToPort(pin));
	if (gpioPort == NULL) return;
#ifdef LL_GPIO_SPEED_FREQ_VERY_HIGH
	LL_GPIO_SetPinSpeed(gpioPort, llPin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
#else
	LL_GPIO_SetPinSpeed(gpioPort, llPin, LL_GPIO_SPEED_FREQ_HIGH);
#endif
	LL_GPIO_SetPinOutputType(gpioPort, llPin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_ALTERNATE);
	__HAL_RCC_AFIO_CLK_ENABLE();
	switch (timer) {
	case 0: /* default remap block */ break;
#ifdef __HAL_AFIO_REMAP_TIM1_ENABLE
	case 1: __HAL_AFIO_REMAP_TIM1_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM1_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM1_PARTIAL
	case 2: __HAL_AFIO_REMAP_TIM1_PARTIAL(); break;
#endif /* __HAL_AFIO_REMAP_TIM1_PARTIAL */
#ifdef __HAL_AFIO_REMAP_TIM2_PARTIAL_1
	case 3: __HAL_AFIO_REMAP_TIM2_PARTIAL_1(); break;
#endif /* __HAL_AFIO_REMAP_TIM2_PARTIAL_1 */
#ifdef __HAL_AFIO_REMAP_TIM2_PARTIAL_2
	case 4: __HAL_AFIO_REMAP_TIM2_PARTIAL_2(); break;
#endif /* __HAL_AFIO_REMAP_TIM2_PARTIAL_2 */
#ifdef __HAL_AFIO_REMAP_TIM3_ENABLE
	case 5: __HAL_AFIO_REMAP_TIM3_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM3_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM3_PARTIAL
	case 6: __HAL_AFIO_REMAP_TIM3_PARTIAL(); break;
#endif /* __HAL_AFIO_REMAP_TIM3_PARTIAL */
#ifdef __HAL_AFIO_REMAP_TIM4_ENABLE
	case 7: __HAL_AFIO_REMAP_TIM4_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM4_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM9_ENABLE
	case 8: __HAL_AFIO_REMAP_TIM9_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM9_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM10_ENABLE
	case 9: __HAL_AFIO_REMAP_TIM10_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM10_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM11_ENABLE
	case 10: __HAL_AFIO_REMAP_TIM11_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM11_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM12_ENABLE
	case 11: __HAL_AFIO_REMAP_TIM12_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM12_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM13_ENABLE
	case 12: __HAL_AFIO_REMAP_TIM13_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM13_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM14_ENABLE
	case 13: __HAL_AFIO_REMAP_TIM14_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM14_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM15_ENABLE
	case 14: __HAL_AFIO_REMAP_TIM15_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM15_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM16_ENABLE
	case 15: __HAL_AFIO_REMAP_TIM16_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM16_ENABLE */
#ifdef __HAL_AFIO_REMAP_TIM17_ENABLE
	case 16: __HAL_AFIO_REMAP_TIM17_ENABLE(); break;
#endif /* __HAL_AFIO_REMAP_TIM17_ENABLE */
	default: break;
	}
}
#endif /* STM32F1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
_TimerConfig::_TimerConfig(const uint32_t ticks, const uint32_t factor1, const bool lowPowerPrescaler) {
#else /* not LPTIM_PRESCALER_DIV1 */
_TimerConfig::_TimerConfig(const uint32_t ticks, const uint32_t factor1, const bool) {
#endif /* not LPTIM_PRESCALER_DIV1 */
	const uint32_t fullPrescalerX = uint32_t(ticks + factor1 - 1) / factor1; /* rounded up */
	const uint32_t fullPrescaler = (fullPrescalerX > 0) ? fullPrescalerX : 1; /* ensure that the divisor is not 0 */
	uint32_t factor2;
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( ! lowPowerPrescaler ) {
#endif /* LPTIM_PRESCALER_DIV1 */
		this->prescaler = uint32_t(fullPrescaler - 1);
		factor2 = (ticks + (fullPrescaler / 2)) / fullPrescaler; /* rounded to the closest */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	}
	if ( lowPowerPrescaler ) {
		uint8_t quantizedPrescaler;
		if (fullPrescaler > 64) {
			quantizedPrescaler = 128;
			this->prescaler = LPTIM_PRESCALER_DIV128;
		} else if (fullPrescaler > 32) {
			quantizedPrescaler = 64;
			this->prescaler = LPTIM_PRESCALER_DIV64;
		} else if (fullPrescaler > 16) {
			quantizedPrescaler = 32;
			this->prescaler = LPTIM_PRESCALER_DIV32;
		} else if (fullPrescaler > 8) {
			quantizedPrescaler = 16;
			this->prescaler = LPTIM_PRESCALER_DIV16;
		} else if (fullPrescaler > 4) {
			quantizedPrescaler = 8;
			this->prescaler = LPTIM_PRESCALER_DIV8;
		} else if (fullPrescaler > 2) {
			quantizedPrescaler = 4;
			this->prescaler = LPTIM_PRESCALER_DIV4;
		} else if (fullPrescaler > 1) {
			quantizedPrescaler = 2;
			this->prescaler = LPTIM_PRESCALER_DIV2;
		} else {
			quantizedPrescaler = 1;
			this->prescaler = LPTIM_PRESCALER_DIV1;
		}
		factor2 = ticks / quantizedPrescaler;
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
	this->period = (factor2 > 0) ? uint32_t(factor2 - 1) : 0;
	const uint32_t closestTickCount = fullPrescaler * factor2;
	this->error = (closestTickCount > ticks) ? uint32_t(closestTickCount - ticks) : uint32_t(ticks - closestTickCount);
}


extern "C" {


/**
 * Overwrites the STM32 HAL API handler for TIM base MSP initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef * hTim) {
	switch (reinterpret_cast<uintptr_t>(hTim->Instance)) {
#ifdef TIM1
	case TIM1_BASE:
		__HAL_RCC_TIM1_CLK_ENABLE();
		break;
#endif /* TIM1 */
#ifdef TIM2
	case TIM2_BASE:
		__HAL_RCC_TIM2_CLK_ENABLE();
		break;
#endif /* TIM2 */
#ifdef TIM3
	case TIM3_BASE:
		__HAL_RCC_TIM3_CLK_ENABLE();
		break;
#endif /* TIM3 */
#ifdef TIM4
	case TIM4_BASE:
		__HAL_RCC_TIM4_CLK_ENABLE();
		break;
#endif /* TIM4 */
#ifdef TIM5
	case TIM5_BASE:
		__HAL_RCC_TIM5_CLK_ENABLE();
		break;
#endif /* TIM5 */
#ifdef TIM6
	case TIM6_BASE:
		__HAL_RCC_TIM6_CLK_ENABLE();
		break;
#endif /* TIM6 */
#ifdef TIM7
	case TIM7_BASE:
		__HAL_RCC_TIM7_CLK_ENABLE();
		break;
#endif /* TIM7 */
#ifdef TIM8
	case TIM8_BASE:
		__HAL_RCC_TIM8_CLK_ENABLE();
		break;
#endif /* TIM8 */
#ifdef TIM9
	case TIM9_BASE:
		__HAL_RCC_TIM9_CLK_ENABLE();
		break;
#endif /* TIM9 */
#ifdef TIM10
	case TIM10_BASE:
		__HAL_RCC_TIM10_CLK_ENABLE();
		break;
#endif /* TIM10 */
#ifdef TIM11
	case TIM11_BASE:
		__HAL_RCC_TIM11_CLK_ENABLE();
		break;
#endif /* TIM11 */
#ifdef TIM12
	case TIM12_BASE:
		__HAL_RCC_TIM12_CLK_ENABLE();
		break;
#endif /* TIM12 */
#ifdef TIM13
	case TIM13_BASE:
		__HAL_RCC_TIM13_CLK_ENABLE();
		break;
#endif /* TIM13 */
#ifdef TIM14
	case TIM14_BASE:
		__HAL_RCC_TIM14_CLK_ENABLE();
		break;
#endif /* TIM14 */
#ifdef TIM15
	case TIM15_BASE:
		__HAL_RCC_TIM15_CLK_ENABLE();
		break;
#endif /* TIM15 */
#ifdef TIM16
	case TIM16_BASE:
		__HAL_RCC_TIM16_CLK_ENABLE();
		break;
#endif /* TIM16 */
#ifdef TIM17
	case TIM17_BASE:
		__HAL_RCC_TIM17_CLK_ENABLE();
		break;
#endif /* TIM17 */
#ifdef TIM18
	case TIM18_BASE:
		__HAL_RCC_TIM18_CLK_ENABLE();
		break;
#endif /* TIM18 */
#ifdef TIM19
	case TIM19_BASE:
		__HAL_RCC_TIM19_CLK_ENABLE();
		break;
#endif /* TIM19 */
#ifdef TIM20
	case TIM20_BASE:
		__HAL_RCC_TIM20_CLK_ENABLE();
		break;
#endif /* TIM20 */
#ifdef TIM21
	case TIM21_BASE:
		__HAL_RCC_TIM21_CLK_ENABLE();
		break;
#endif /* TIM21 */
#ifdef TIM22
	case TIM22_BASE:
		__HAL_RCC_TIM22_CLK_ENABLE();
		break;
#endif /* TIM22 */
#ifdef TIM23
	case TIM23_BASE:
		__HAL_RCC_TIM23_CLK_ENABLE();
		break;
#endif /* TIM23 */
#ifdef TIM24
	case TIM24_BASE:
		__HAL_RCC_TIM24_CLK_ENABLE();
		break;
#endif /* TIM24 */
	default: break;
	}
}


/**
 * Overwrites the STM32 HAL API handler for TIM base MSP de-initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_Base_DeMspInit(TIM_HandleTypeDef * hTim) {
	switch (reinterpret_cast<uintptr_t>(hTim->Instance)) {
#ifdef TIM1
	case TIM1_BASE:
		__HAL_RCC_TIM1_CLK_DISABLE();
		break;
#endif /* TIM1 */
#ifdef TIM2
	case TIM2_BASE:
		__HAL_RCC_TIM2_CLK_DISABLE();
		break;
#endif /* TIM2 */
#ifdef TIM3
	case TIM3_BASE:
		__HAL_RCC_TIM3_CLK_DISABLE();
		break;
#endif /* TIM3 */
#ifdef TIM4
	case TIM4_BASE:
		__HAL_RCC_TIM4_CLK_DISABLE();
		break;
#endif /* TIM4 */
#ifdef TIM5
	case TIM5_BASE:
		__HAL_RCC_TIM5_CLK_DISABLE();
		break;
#endif /* TIM5 */
#ifdef TIM6
	case TIM6_BASE:
		__HAL_RCC_TIM6_CLK_DISABLE();
		break;
#endif /* TIM6 */
#ifdef TIM7
	case TIM7_BASE:
		__HAL_RCC_TIM7_CLK_DISABLE();
		break;
#endif /* TIM7 */
#ifdef TIM8
	case TIM8_BASE:
		__HAL_RCC_TIM8_CLK_DISABLE();
		break;
#endif /* TIM8 */
#ifdef TIM9
	case TIM9_BASE:
		__HAL_RCC_TIM9_CLK_DISABLE();
		break;
#endif /* TIM9 */
#ifdef TIM10
	case TIM10_BASE:
		__HAL_RCC_TIM10_CLK_DISABLE();
		break;
#endif /* TIM10 */
#ifdef TIM11
	case TIM11_BASE:
		__HAL_RCC_TIM11_CLK_DISABLE();
		break;
#endif /* TIM11 */
#ifdef TIM12
	case TIM12_BASE:
		__HAL_RCC_TIM12_CLK_DISABLE();
		break;
#endif /* TIM12 */
#ifdef TIM13
	case TIM13_BASE:
		__HAL_RCC_TIM13_CLK_DISABLE();
		break;
#endif /* TIM13 */
#ifdef TIM14
	case TIM14_BASE:
		__HAL_RCC_TIM14_CLK_DISABLE();
		break;
#endif /* TIM14 */
#ifdef TIM15
	case TIM15_BASE:
		__HAL_RCC_TIM15_CLK_DISABLE();
		break;
#endif /* TIM15 */
#ifdef TIM16
	case TIM16_BASE:
		__HAL_RCC_TIM16_CLK_DISABLE();
		break;
#endif /* TIM16 */
#ifdef TIM17
	case TIM17_BASE:
		__HAL_RCC_TIM17_CLK_DISABLE();
		break;
#endif /* TIM17 */
#ifdef TIM18
	case TIM18_BASE:
		__HAL_RCC_TIM18_CLK_DISABLE();
		break;
#endif /* TIM18 */
#ifdef TIM19
	case TIM19_BASE:
		__HAL_RCC_TIM19_CLK_DISABLE();
		break;
#endif /* TIM19 */
#ifdef TIM20
	case TIM20_BASE:
		__HAL_RCC_TIM20_CLK_DISABLE();
		break;
#endif /* TIM20 */
#ifdef TIM21
	case TIM21_BASE:
		__HAL_RCC_TIM21_CLK_DISABLE();
		break;
#endif /* TIM21 */
#ifdef TIM22
	case TIM22_BASE:
		__HAL_RCC_TIM22_CLK_DISABLE();
		break;
#endif /* TIM22 */
#ifdef TIM23
	case TIM23_BASE:
		__HAL_RCC_TIM23_CLK_DISABLE();
		break;
#endif /* TIM23 */
#ifdef TIM24
	case TIM24_BASE:
		__HAL_RCC_TIM24_CLK_DISABLE();
		break;
#endif /* TIM24 */
	default: break;
	}
}


/**
 * Overwrites the STM32 HAL API handler for TIM output compare MSP initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_MspInit(hTim);
}


/**
 * Overwrites the STM32 HAL API handler for TIM PWM MSP initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_MspInit(hTim);
}


/**
 * Overwrites the STM32 HAL API handler for TIM input compare MSP initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_MspInit(hTim);
}


/**
 * Overwrites the STM32 HAL API handler for TIM output compare MSP de-initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_OC_DeMspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_DeMspInit(hTim);
}


/**
 * Overwrites the STM32 HAL API handler for TIM PWM MSP de-initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_PWM_DeMspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_DeMspInit(hTim);
}


/**
 * Overwrites the STM32 HAL API handler for TIM input compare MSP de-initialization request.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 */
void HAL_TIM_IC_DeMspInit(TIM_HandleTypeDef * hTim) {
	HAL_TIM_Base_DeMspInit(hTim);
}


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
/**
 * Overwrites the STM32 HAL API handler for LPTIM MSP initialization request.
 * 
 * @param[in,out] hLpTim - pointer to LPTIM handle
 */
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef * hLpTim) {
	switch (reinterpret_cast<uintptr_t>(hLpTim->Instance)) {
#ifdef LPTIM1
	case LPTIM1_BASE:
		__HAL_RCC_LPTIM1_CLK_ENABLE();
		break;
#endif /* LPTIM1 */
#ifdef LPTIM2
	case LPTIM2_BASE:
		__HAL_RCC_LPTIM2_CLK_ENABLE();
		break;
#endif /* LPTIM2 */
#ifdef LPTIM3
	case LPTIM3_BASE:
		__HAL_RCC_LPTIM3_CLK_ENABLE();
		break;
#endif /* LPTIM3 */
#ifdef LPTIM4
	case LPTIM4_BASE:
		__HAL_RCC_LPTIM4_CLK_ENABLE();
		break;
#endif /* LPTIM4 */
#ifdef LPTIM5
	case LPTIM5_BASE:
		__HAL_RCC_LPTIM5_CLK_ENABLE();
		break;
#endif /* LPTIM5 */
	default:
		break;
	}
}


/**
 * Overwrites the STM32 HAL API handler for LPTIM MSP de-initialization request.
 * 
 * @param[in,out] hLpTim - pointer to LPTIM handle
 */
void HAL_LPTIM_DeMspInit(LPTIM_HandleTypeDef * hLpTim) {
	switch (reinterpret_cast<uintptr_t>(hLpTim->Instance)) {
#ifdef LPTIM1
	case LPTIM1_BASE:
		__HAL_RCC_LPTIM1_CLK_DISABLE();
		break;
#endif /* LPTIM1 */
#ifdef LPTIM2
	case LPTIM2_BASE:
		__HAL_RCC_LPTIM2_CLK_DISABLE();
		break;
#endif /* LPTIM2 */
#ifdef LPTIM3
	case LPTIM3_BASE:
		__HAL_RCC_LPTIM3_CLK_DISABLE();
		break;
#endif /* LPTIM3 */
#ifdef LPTIM4
	case LPTIM4_BASE:
		__HAL_RCC_LPTIM4_CLK_DISABLE();
		break;
#endif /* LPTIM4 */
#ifdef LPTIM5
	case LPTIM5_BASE:
		__HAL_RCC_LPTIM5_CLK_DISABLE();
		break;
#endif /* LPTIM5 */
	default:
		break;
	}
}
#endif /* LPTIM_PRESCALER_DIV1 */


#if !defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE) /* STM32 HAL I2C header was included */
/**
 * Returns the peripheral clock frequency for the given I2C HAL instance
 * or 0 if the frequency could not be determined.
 * 
 * @param[in] i2c - I2C HAL instance
 * @return peripheral clock frequency
 */
uint32_t getI2cClockFrequency(const I2C_TypeDef * i2c) {
	uint32_t res = 0;
	switch (reinterpret_cast<uintptr_t>(i2c)) {
#ifdef I2C1
	case I2C1_BASE:	
#if defined(RCC_PERIPHCLK_I2C1) || defined(RCC_PERIPHCLK_I2C12)
#ifdef RCC_PERIPHCLK_I2C1
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C1);
#else /* not RCC_PERIPHCLK_I2C1 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C12);
#endif /* not RCC_PERIPHCLK_I2C1 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_I2C1 or RCC_PERIPHCLK_I2C12 */
		{
#ifdef __HAL_RCC_GET_I2C1_SOURCE
			_TimerPinMap::ClockSource clkSrc = _TimerPinMap::ClockSource_Unknown;
			switch (__HAL_RCC_GET_I2C1_SOURCE()) {
#ifdef RCC_I2C1CLKSOURCE_SYSCLK
			case RCC_I2C1CLKSOURCE_SYSCLK:  clkSrc = _TimerPinMap::ClockSource_SYSCLK; break;
#endif /* RCC_I2C1CLKSOURCE_SYSCLK */
#ifdef RCC_I2C1CLKSOURCE_CSI
			case RCC_I2C1CLKSOURCE_CSI:     clkSrc = _TimerPinMap::ClockSource_CSI; break;
#endif /* RCC_I2C1CLKSOURCE_CSI */
#ifdef RCC_I2C1CLKSOURCE_HSI
			case RCC_I2C1CLKSOURCE_HSI:     clkSrc = _TimerPinMap::ClockSource_HSI; break;
#endif /* RCC_I2C1CLKSOURCE_HSI */
#ifdef RCC_I2C1CLKSOURCE_PCLK1
			case RCC_I2C1CLKSOURCE_PCLK1:   clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C1CLKSOURCE_PCLK1 */
#ifdef RCC_I2C1CLKSOURCE_D2PCLK1
			case RCC_I2C1CLKSOURCE_D2PCLK1: clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C1CLKSOURCE_D2PCLK1 */
#ifdef RCC_I2C1CLKSOURCE_PLL3
			case RCC_I2C1CLKSOURCE_PLL3:    clkSrc = _TimerPinMap::ClockSource_PLL3; break;
#endif /* RCC_I2C1CLKSOURCE_PLL3 */
			default: break;
			}
			res = _TimerPinMap::getClockFrequency(clkSrc);
#endif /* __HAL_RCC_GET_I2C1_SOURCE */
		}
		break;
#endif /* I2C1 */
#ifdef I2C2
	case I2C2_BASE:	
#if defined(RCC_PERIPHCLK_I2C2) || defined(RCC_PERIPHCLK_I2C12)
#ifdef RCC_PERIPHCLK_I2C2
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C2);
#else /* not RCC_PERIPHCLK_I2C2 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C12);
#endif /* not RCC_PERIPHCLK_I2C2 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_I2C2 or RCC_PERIPHCLK_I2C12 */
		{
#ifdef __HAL_RCC_GET_I2C2_SOURCE
			_TimerPinMap::ClockSource clkSrc = _TimerPinMap::ClockSource_Unknown;
			switch (__HAL_RCC_GET_I2C2_SOURCE()) {
#ifdef RCC_I2C2CLKSOURCE_SYSCLK
			case RCC_I2C2CLKSOURCE_SYSCLK:  clkSrc = _TimerPinMap::ClockSource_SYSCLK; break;
#endif /* RCC_I2C2CLKSOURCE_SYSCLK */
#ifdef RCC_I2C2CLKSOURCE_CSI
			case RCC_I2C2CLKSOURCE_CSI:     clkSrc = _TimerPinMap::ClockSource_CSI; break;
#endif /* RCC_I2C2CLKSOURCE_CSI */
#ifdef RCC_I2C2CLKSOURCE_HSI
			case RCC_I2C2CLKSOURCE_HSI:     clkSrc = _TimerPinMap::ClockSource_HSI; break;
#endif /* RCC_I2C2CLKSOURCE_HSI */
#ifdef RCC_I2C2CLKSOURCE_PCLK1
			case RCC_I2C2CLKSOURCE_PCLK1:   clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C2CLKSOURCE_PCLK1 */
#ifdef RCC_I2C2CLKSOURCE_D2PCLK1
			case RCC_I2C2CLKSOURCE_D2PCLK1: clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C2CLKSOURCE_D2PCLK1 */
#ifdef RCC_I2C2CLKSOURCE_PLL3
			case RCC_I2C2CLKSOURCE_PLL3:    clkSrc = _TimerPinMap::ClockSource_PLL3; break;
#endif /* RCC_I2C2CLKSOURCE_PLL3 */
			default: break;
			}
			res = _TimerPinMap::getClockFrequency(clkSrc);
#endif /* __HAL_RCC_GET_I2C2_SOURCE */
		}
		break;
#endif /* I2C2 */
#ifdef I2C3
	case I2C3_BASE:	
#if defined(RCC_PERIPHCLK_I2C3) || defined(RCC_PERIPHCLK_I2C35)
#ifdef RCC_PERIPHCLK_I2C3
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C3);
#else /* not RCC_PERIPHCLK_I2C3 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C35);
#endif /* not RCC_PERIPHCLK_I2C3 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_I2C3 or RCC_PERIPHCLK_I2C35 */
		{
#ifdef __HAL_RCC_GET_I2C3_SOURCE
			_TimerPinMap::ClockSource clkSrc = _TimerPinMap::ClockSource_Unknown;
			switch (__HAL_RCC_GET_I2C3_SOURCE()) {
#ifdef RCC_I2C3CLKSOURCE_SYSCLK
			case RCC_I2C3CLKSOURCE_SYSCLK:  clkSrc = _TimerPinMap::ClockSource_SYSCLK; break;
#endif /* RCC_I2C3CLKSOURCE_SYSCLK */
#ifdef RCC_I2C3CLKSOURCE_CSI
			case RCC_I2C3CLKSOURCE_CSI:     clkSrc = _TimerPinMap::ClockSource_CSI; break;
#endif /* RCC_I2C3CLKSOURCE_CSI */
#ifdef RCC_I2C3CLKSOURCE_HSI
			case RCC_I2C3CLKSOURCE_HSI:     clkSrc = _TimerPinMap::ClockSource_HSI; break;
#endif /* RCC_I2C3CLKSOURCE_HSI */
#ifdef RCC_I2C3CLKSOURCE_PCLK1
			case RCC_I2C3CLKSOURCE_PCLK1:   clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C3CLKSOURCE_PCLK1 */
#ifdef RCC_I2C3CLKSOURCE_D2PCLK1
			case RCC_I2C3CLKSOURCE_D2PCLK1: clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C3CLKSOURCE_D2PCLK1 */
#ifdef RCC_I2C3CLKSOURCE_PLL3
			case RCC_I2C3CLKSOURCE_PLL3:    clkSrc = _TimerPinMap::ClockSource_PLL3; break;
#endif /* RCC_I2C3CLKSOURCE_PLL3 */
			default: break;
			}
			res = _TimerPinMap::getClockFrequency(clkSrc);
#endif /* __HAL_RCC_GET_I2C3_SOURCE */
		}
		break;
#endif /* I2C3 */
#ifdef I2C4
	case I2C4_BASE:	
#if defined(RCC_PERIPHCLK_I2C4) || defined(RCC_PERIPHCLK_I2C46)
#ifdef RCC_PERIPHCLK_I2C4
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C4);
#else /* not RCC_PERIPHCLK_I2C4 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C46);
#endif /* not RCC_PERIPHCLK_I2C4 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_I2C4 or RCC_PERIPHCLK_I2C46 */
		{
#ifdef __HAL_RCC_GET_I2C4_SOURCE
			_TimerPinMap::ClockSource clkSrc = _TimerPinMap::ClockSource_Unknown;
			switch (__HAL_RCC_GET_I2C4_SOURCE()) {
#ifdef RCC_I2C4CLKSOURCE_SYSCLK
			case RCC_I2C4CLKSOURCE_SYSCLK:  clkSrc = _TimerPinMap::ClockSource_SYSCLK; break;
#endif /* RCC_I2C4CLKSOURCE_SYSCLK */
#ifdef RCC_I2C4CLKSOURCE_CSI
			case RCC_I2C4CLKSOURCE_CSI:     clkSrc = _TimerPinMap::ClockSource_CSI; break;
#endif /* RCC_I2C4CLKSOURCE_CSI */
#ifdef RCC_I2C4CLKSOURCE_HSI
			case RCC_I2C4CLKSOURCE_HSI:     clkSrc = _TimerPinMap::ClockSource_HSI; break;
#endif /* RCC_I2C4CLKSOURCE_HSI */
#ifdef RCC_I2C4CLKSOURCE_PCLK1
			case RCC_I2C4CLKSOURCE_PCLK1:   clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C4CLKSOURCE_PCLK1 */
#ifdef RCC_I2C4CLKSOURCE_D2PCLK1
			case RCC_I2C4CLKSOURCE_D2PCLK1: clkSrc = _TimerPinMap::ClockSource_PCLK1; break;
#endif /* RCC_I2C4CLKSOURCE_D2PCLK1 */
#ifdef RCC_I2C4CLKSOURCE_PLL3
			case RCC_I2C4CLKSOURCE_PLL3:    clkSrc = _TimerPinMap::ClockSource_PLL3; break;
#endif /* RCC_I2C4CLKSOURCE_PLL3 */
			default: break;
			}
			res = _TimerPinMap::getClockFrequency(clkSrc);
#endif /* __HAL_RCC_GET_I2C4_SOURCE */
		}
		break;
#endif /* I2C4 */
#ifdef I2C5
	case I2C5_BASE:
#ifdef RCC_PERIPHCLK_I2C5
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C5);
#else /* not RCC_PERIPHCLK_I2C5 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C35);
#endif /* not RCC_PERIPHCLK_I2C5 */
		break;
#endif /* I2C5 */
#ifdef I2C6
	case I2C6_BASE:
#ifdef RCC_PERIPHCLK_I2C6
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C6);
#else /* not RCC_PERIPHCLK_I2C6 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_I2C46);
#endif /* not RCC_PERIPHCLK_I2C6 */
		break;
#endif /* I2C6 */
	default:
		break;
	}
	return res;
}
#endif /* STM32 HAL I2C header was included */


#if !defined(STM32CUBEDUINO_DISABLE_SPI) && defined(IS_SPI_MODE) /* STM32 HAL SPI header was included */
/**
 * Returns the peripheral clock frequency for the given SPI HAL instance
 * or 0 if the frequency could not be determined.
 * 
 * @param[in] spi - SPI HAL instance
 * @return peripheral clock frequency
 */
uint32_t getSpiClockFrequency(const SPI_TypeDef * spi) {
	uint32_t res = 0;
	switch (reinterpret_cast<uintptr_t>(spi)) {
#ifdef SPI1
	case SPI1_BASE:	
#if defined(RCC_PERIPHCLK_SPI1) || defined(RCC_PERIPHCLK_SPI123)
#ifdef RCC_PERIPHCLK_SPI1
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI1);
#else /* not RCC_PERIPHCLK_SPI1 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI123);
#endif /* not RCC_PERIPHCLK_SPI1 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_SPI1 or RCC_PERIPHCLK_SPI123 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK2);
		}
		break;
#endif /* SPI1 */
#ifdef SPI2
	case SPI2_BASE:	
#if defined(RCC_PERIPHCLK_SPI2) || defined(RCC_PERIPHCLK_SPI123) || defined(RCC_PERIPHCLK_SPI23)
#ifdef RCC_PERIPHCLK_SPI2
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI2);
#elif defined(RCC_PERIPHCLK_SPI123) /* RCC_PERIPHCLK_SPI123 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI123);
#else /* not RCC_PERIPHCLK_SPI2 or RCC_PERIPHCLK_SP123 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI23);
#endif /* not RCC_PERIPHCLK_SPI2 or RCC_PERIPHCLK_SP123 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_SPI2 or RCC_PERIPHCLK_SPI123 or RCC_PERIPHCLK_SPI23 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK1);
		}
		break;
#endif /* SPI2 */
#ifdef SPI3
	case SPI3_BASE:	
#if defined(RCC_PERIPHCLK_SPI3) || defined(RCC_PERIPHCLK_SPI123) || defined(RCC_PERIPHCLK_SPI23)
#ifdef RCC_PERIPHCLK_SPI3
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI3);
#elif defined(RCC_PERIPHCLK_SPI123) /* RCC_PERIPHCLK_SPI123 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI123);
#else /* not RCC_PERIPHCLK_SPI3 or RCC_PERIPHCLK_SP123 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI23);
#endif /* not RCC_PERIPHCLK_SPI3 or RCC_PERIPHCLK_SP123 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_SPI3 or RCC_PERIPHCLK_SPI123 or RCC_PERIPHCLK_SPI23 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK1);
		}
		break;
#endif /* SPI3 */
#ifdef SPI4
	case SPI4_BASE:	
#if defined(RCC_PERIPHCLK_SPI4) || defined(RCC_PERIPHCLK_SPI45)
#ifdef RCC_PERIPHCLK_SPI4
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI4);
#else /* not RCC_PERIPHCLK_SPI4 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI45);
#endif /* not RCC_PERIPHCLK_SPI4 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_SPI4 or RCC_PERIPHCLK_SPI45 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK2);
		}
		break;
#endif /* SPI4 */
#ifdef SPI5
	case SPI5_BASE:	
#if defined(RCC_PERIPHCLK_SPI5) || defined(RCC_PERIPHCLK_SPI45)
#ifdef RCC_PERIPHCLK_SPI5
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI5);
#else /* not RCC_PERIPHCLK_SPI5 */
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI45);
#endif /* not RCC_PERIPHCLK_SPI5 */
		if (res == 0)
#endif /* not RCC_PERIPHCLK_SPI5 or RCC_PERIPHCLK_SPI45 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK2);
		}
		break;
#endif /* SPI5 */
#ifdef SPI6
	case SPI6_BASE:
#ifdef RCC_PERIPHCLK_SPI6
		res = HAL_RCCEx_GetPeriphCLKFreq(RCC_PERIPHCLK_SPI6);
		if (res == 0)
#endif /* RCC_PERIPHCLK_SPI6 */
		{
			res = _TimerPinMap::getClockFrequency(_TimerPinMap::ClockSource_PCLK2);
		}
		break;
#endif /* SPI6 */
	default:
		break;
	}
	return res;
}
#endif /* STM32 HAL SPI header was included */


} /* extern "C" */
