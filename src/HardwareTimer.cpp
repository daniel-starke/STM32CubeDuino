/**
 * @file HardwareTimer.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-11-04
 * @version 2022-03-21
 */
#include "Arduino.h"
#include "HardwareTimer.h"
#include "wiring_private.h"
#include "wiring_irq.h"
#include "scdinternal/macro.h"


#if !defined(STM32CUBEDUINO_DISABLE_TIMER) && (defined(TIM_CLOCKPRESCALER_DIV1) || defined(LPTIM_PRESCALER_DIV1)) /* STM32 HAL TIM or LPTIM header was included */


#ifndef TIMER_IRQ_PRIO
#error Please define TIMER_IRQ_PRIO in board.hpp.
#endif
#ifndef TIMER_IRQ_SUBPRIO
#error Please define TIMER_IRQ_SUBPRIO in board.hpp.
#endif


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
#ifdef TIM1_BASE
extern _TimerPinMap::TimHandle tim1Handle;
#endif /* TIM1_BASE */
#ifdef TIM2_BASE
extern _TimerPinMap::TimHandle tim2Handle;
#endif /* TIM2_BASE */
#ifdef TIM3_BASE
extern _TimerPinMap::TimHandle tim3Handle;
#endif /* TIM3_BASE */
#ifdef TIM4_BASE
extern _TimerPinMap::TimHandle tim4Handle;
#endif /* TIM4_BASE */
#ifdef TIM5_BASE
extern _TimerPinMap::TimHandle tim5Handle;
#endif /* TIM5_BASE */
#ifdef TIM6_BASE
extern _TimerPinMap::TimHandle tim6Handle;
#endif /* TIM6_BASE */
#ifdef TIM7_BASE
extern _TimerPinMap::TimHandle tim7Handle;
#endif /* TIM7_BASE */
#ifdef TIM8_BASE
extern _TimerPinMap::TimHandle tim8Handle;
#endif /* TIM8_BASE */
#ifdef TIM9_BASE
extern _TimerPinMap::TimHandle tim9Handle;
#endif /* TIM9_BASE */
#ifdef TIM10_BASE
extern _TimerPinMap::TimHandle tim10Handle;
#endif /* TIM10_BASE */
#ifdef TIM11_BASE
extern _TimerPinMap::TimHandle tim11Handle;
#endif /* TIM11_BASE */
#ifdef TIM12_BASE
extern _TimerPinMap::TimHandle tim12Handle;
#endif /* TIM12_BASE */
#ifdef TIM13_BASE
extern _TimerPinMap::TimHandle tim13Handle;
#endif /* TIM13_BASE */
#ifdef TIM14_BASE
extern _TimerPinMap::TimHandle tim14Handle;
#endif /* TIM14_BASE */
#ifdef TIM15_BASE
extern _TimerPinMap::TimHandle tim15Handle;
#endif /* TIM15_BASE */
#ifdef TIM16_BASE
extern _TimerPinMap::TimHandle tim16Handle;
#endif /* TIM16_BASE */
#ifdef TIM17_BASE
extern _TimerPinMap::TimHandle tim17Handle;
#endif /* TIM17_BASE */
#ifdef TIM18_BASE
extern _TimerPinMap::TimHandle tim18Handle;
#endif /* TIM18_BASE */
#ifdef TIM19_BASE
extern _TimerPinMap::TimHandle tim19Handle;
#endif /* TIM19_BASE */
#ifdef TIM20_BASE
extern _TimerPinMap::TimHandle tim20Handle;
#endif /* TIM20_BASE */
#ifdef TIM21_BASE
extern _TimerPinMap::TimHandle tim21Handle;
#endif /* TIM21_BASE */
#ifdef TIM22_BASE
extern _TimerPinMap::TimHandle tim22Handle;
#endif /* TIM22_BASE */
#ifdef TIM23_BASE
extern _TimerPinMap::TimHandle tim23Handle;
#endif /* TIM23_BASE */
#ifdef TIM24_BASE
extern _TimerPinMap::TimHandle tim24Handle;
#endif /* TIM24_BASE */
#endif /* TIM_CLOCKPRESCALER_DIV1 */

#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
#ifdef LPTIM1_BASE
extern _TimerPinMap::LpTimHandle lpTim1Handle;
#endif /* LPTIM1_BASE */
#ifdef LPTIM2_BASE
extern _TimerPinMap::LpTimHandle lpTim2Handle;
#endif /* LPTIM2_BASE */
#ifdef LPTIM3_BASE
extern _TimerPinMap::LpTimHandle lpTim3Handle;
#endif /* LPTIM3_BASE */
#ifdef LPTIM4_BASE
extern _TimerPinMap::LpTimHandle lpTim4Handle;
#endif /* LPTIM4_BASE */
#ifdef LPTIM5_BASE
extern _TimerPinMap::LpTimHandle lpTim5Handle;
#endif /* LPTIM5_BASE */
#endif /* LPTIM_PRESCALER_DIV1 */


namespace {


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
/* C++ SFINAE for handling the absence of LL_TIM_SetPrescaler() */
template <typename T>
inline auto LL_TIM_SetPrescaler_Wrapper(const int, T hTim, const uint32_t prescaler) -> decltype(LL_TIM_SetPrescaler(hTim, prescaler)) {
	LL_TIM_SetPrescaler(hTim, prescaler);
}

template <typename T>
inline void LL_TIM_SetPrescaler_Wrapper(const float, T hTim, const uint32_t prescaler) {
	/* this re-implements LL_TIM_SetPrescaler() */
	WRITE_REG(hTim->PSC, prescaler);
}

inline void LL_TIM_SetPrescaler_Wrapper(TIM_TypeDef * hTim, const uint32_t prescaler) {
	LL_TIM_SetPrescaler_Wrapper(0 /* prefer int here */, hTim, prescaler);
}

/* C++ SFINAE for handling the absence of LL_TIM_DisableCounter() */
template <typename T>
inline auto LL_TIM_DisableCounter_Wrapper(const int, T hTim) -> decltype(LL_TIM_DisableCounter(hTim)) {
	LL_TIM_DisableCounter(hTim);
}

template <typename T>
inline void LL_TIM_DisableCounter_Wrapper(const float, T hTim) {
	/* this re-implements LL_TIM_DisableCounter() */
	CLEAR_BIT(hTim->CR1, TIM_CR1_CEN);
}

inline void LL_TIM_DisableCounter_Wrapper(TIM_TypeDef * hTim) {
	LL_TIM_DisableCounter_Wrapper(0 /* prefer int here */, hTim);
}
#endif /* TIM_CLOCKPRESCALER_DIV1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
/* C++ SFINAE for handling the absence of LL_LPTIM_SetPrescaler() */
template <typename T>
inline auto LL_LPTIM_SetPrescaler_Wrapper(const int, T hLpTim, const uint32_t prescaler) -> decltype(LL_LPTIM_SetPrescaler(hLpTim, prescaler)) {
	LL_LPTIM_SetPrescaler(hLpTim, prescaler);
}

template <typename T>
inline void LL_LPTIM_SetPrescaler_Wrapper(const float, T hLpTim, const uint32_t prescaler) {
	/* this re-implements LL_LPTIM_SetPrescaler() */
	MODIFY_REG(hLpTim->CFGR, LPTIM_CFGR_PRESC, prescaler);
}

inline void LL_LPTIM_SetPrescaler_Wrapper(LPTIM_TypeDef * hLpTim, const uint32_t prescaler) {
	LL_LPTIM_SetPrescaler_Wrapper(0 /* prefer int here */, hLpTim, prescaler);
}

/**
 * Helper function to properly return the low power timer counter
 * even if running on asynchronous clock.
 * 
 * @param[in] hLpTim - pointer to LPTIM handle
 * @return current counter
 */
uint16_t getLpTimCounter(LPTIM_HandleTypeDef * hLpTim) {
	uint16_t last;
	uint16_t now = uint16_t(HAL_LPTIM_ReadCounter(hLpTim));
	do {
		last = now;
		now = uint16_t(HAL_LPTIM_ReadCounter(hLpTim));
	} while (last != now);
	return now;
}
#endif /* LPTIM_PRESCALER_DIV1 */


} /* anonymous namespace */


#if defined(TIM_CLOCKPRESCALER_DIV1) /* STM32 HAL TIM header was included */
#if defined(TIM1_BASE)
HardwareTimer __attribute__((weak)) Timer0(TIM1, getIrqNumFor(TIM1));
#elif defined(TIM2_BASE)
HardwareTimer __attribute__((weak)) Timer0(TIM2, getIrqNumFor(TIM2));
#elif defined(TIM3_BASE)
HardwareTimer __attribute__((weak)) Timer0(TIM3, getIrqNumFor(TIM3));
#elif defined(TIM6_BASE)
HardwareTimer __attribute__((weak)) Timer0(TIM6, getIrqNumFor(TIM6));
#endif
#elif defined(LPTIM_PRESCALER_DIV1) && defined(LPTIM1_BASE) /* STM32 HAL LPTIM header was included */
HardwareTimer __attribute__((weak)) Timer0(LPTIM1, getIrqNumFor(LPTIM1));
#endif /* LPTIM_PRESCALER_DIV1 */


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
/**
 * Creates a timer from the given TIM instance and matching IRQ number.
 * 
 * @param[in] inst - TIM instance
 * @param[in] irqNum - IRQ number
 */
HardwareTimer::HardwareTimer(TIM_TypeDef * inst, const IRQn_Type irqNum) {
	_TimerPinMap::TimHandle * hTim = _TimerPinMap::getInternalTimHandleFromId(inst);
	if (hTim == NULL) systemErrorHandler();
	this->handle.tim = &(hTim->tim);
	this->irq = irqNum;
	this->callback = NULL;
	this->instNum = uint8_t(_TimerPinMap::getInstanceNumFromId(inst));
	hTim->tim.Instance = inst;
	hTim->tim.Init.Prescaler = 0;
	hTim->tim.Init.Period = 1;
	hTim->tim.Init.CounterMode = TIM_COUNTERMODE_UP;
	hTim->tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTim->tim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	SET_FOR_EXISTING_MEMBER(hTim->tim.Init, RepetitionCounter, 0);
	this->setFrequency(1);
}
#endif /* TIM_CLOCKPRESCALER_DIV1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
/**
 * Creates a timer from the given LPTIM instance and matching IRQ number.
 * 
 * @param[in] inst - LPTIM instance
 * @param[in] irqNum - IRQ number
 */
HardwareTimer::HardwareTimer(LPTIM_TypeDef * inst, const IRQn_Type irqNum) {
	_TimerPinMap::LpTimHandle * hLpTim = _TimerPinMap::getInternalLpTimHandleFromId(inst);
	if (hLpTim == NULL) systemErrorHandler();
	this->handle.lptim = &(hLpTim->lptim);
	this->irq = irqNum;
	this->callback = NULL;
	this->instNum = uint8_t(_TimerPinMap::getInstanceNumFromId(inst));
	hLpTim->lptim.Instance = inst;
	hLpTim->lptim.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
	hLpTim->lptim.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
	hLpTim->lptim.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
	hLpTim->lptim.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_LOW;
	hLpTim->lptim.Init.UpdateMode = LPTIM_UPDATE_ENDOFPERIOD;
	hLpTim->lptim.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
	SET_FOR_EXISTING_MEMBER(hLpTim->lptim.Init, RepetitionCounter, 0);
	this->setFrequency(1);
}
#endif /* LPTIM_PRESCALER_DIV1 */


/**
 * Destructor.
 */
HardwareTimer::~HardwareTimer() {
	this->deinitialize();
}


/**
 * Initializes the timer. This needs to be called before starting a timer for the first time.
 * The timer remains stopped until start() is called.
 */
void HardwareTimer::initialize() {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		if (HAL_TIM_Base_Init(this->handle.tim) != HAL_OK) return;
		TIM_ClockConfigTypeDef clkSrc = {0};
		clkSrc.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		if (HAL_TIM_ConfigClockSource(this->handle.tim, &clkSrc) != HAL_OK) return;
		/* link this HardwareTimer instance to the IRQ relevant handler */
		reinterpret_cast<_TimerPinMap::TimHandle *>(this->handle.tim)->obj = this;
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		if (HAL_LPTIM_Init(this->handle.lptim) != HAL_OK) return;
		/* link this HardwareTimer instance to the IRQ relevant handler */
		reinterpret_cast<_TimerPinMap::LpTimHandle *>(this->handle.lptim)->obj = this;
	}
#endif /* LPTIM_PRESCALER_DIV1 */
#ifdef TIMER_IRQ_PRIO
	this->setInterruptPriority(TIMER_IRQ_PRIO, TIMER_IRQ_SUBPRIO);
#endif /* TIMER_IRQ_PRIO */
	HAL_NVIC_EnableIRQ(this->irq);
}


/**
 * De-initializes the timer.
 */
void HardwareTimer::deinitialize() {
	this->stop();
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		if (this->handle.tim != NULL) {
			/* unlink this HardwareTimer instance from the IRQ relevant handler */
			reinterpret_cast<_TimerPinMap::TimHandle *>(this->handle.tim)->obj = NULL;
			HAL_TIM_Base_DeInit(this->handle.tim);
		}
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		if (this->handle.lptim != NULL) {
			/* unlink this HardwareTimer instance from the IRQ relevant handler */
			reinterpret_cast<_TimerPinMap::LpTimHandle *>(this->handle.lptim)->obj = NULL;
			HAL_LPTIM_DeInit(this->handle.lptim);
		}
	}
#endif /* LPTIM_PRESCALER_DIV1 */
	HAL_NVIC_DisableIRQ(this->irq);
}


/**
 * Initializes the timer. This needs to be called before starting a timer.
 * The timer remains stopped until start() is called.
 * 
 * @param[in] microseconds - timer interval in microseconds (initial default: 1s)
 */
void HardwareTimer::initialize(const unsigned long microseconds) {
	this->setPeriod(microseconds);
	this->initialize();
}


/**
 * Sets the timer interval from the given absolute value.
 * 
 * @param[in] microseconds - timer interval in microseconds
 */
void HardwareTimer::setPeriod(const unsigned long microseconds) {
	const uint32_t clockFrequency = _TimerPinMap::getClockFrequency(_TimerPinMap::getClockSource(this->instNum));
	const uint32_t closestTickCount = uint32_t((uint64_t(clockFrequency) * microseconds) / 1000000UL);
	if ( _TimerPinMap::has32BitCounter(this->instNum) ) {
		/* There are no low power timers with 32-bit counter, hence, no special handling of their prescaler is needed. */
		this->setConfiguration(0, (closestTickCount > 0) ? uint32_t(closestTickCount - 1) : 0);
		return;
	}
	const _TimerConfig cfg1(closestTickCount, 10000, this->isLpTimer());
	const _TimerConfig cfg2(closestTickCount, 0x10000UL, this->isLpTimer());
	const _TimerConfig & cfg = (cfg1.error < cfg2.error) ? cfg1 : cfg2;
	this->setConfiguration(cfg.prescaler, cfg.period);
}


/**
 * Sets the timer interval from the given frequency value.
 * 
 * @param[in] hertz - timer frequency in Hertz
 */
void HardwareTimer::setFrequency(const unsigned long hertz) {
	const uint32_t clockFrequency = _TimerPinMap::getClockFrequency(_TimerPinMap::getClockSource(this->instNum));
	const uint32_t closestTickCount = (clockFrequency + (hertz / 2)) / hertz;
	if ( _TimerPinMap::has32BitCounter(this->instNum) ) {
		/* There are no low power timers with 32-bit counter, hence, no special handling of their prescaler is needed. */
		this->setConfiguration(0, (closestTickCount > 0) ? uint32_t(closestTickCount - 1) : 0);
		return;
	}
	const _TimerConfig cfg1(closestTickCount, 10000, this->isLpTimer());
	const _TimerConfig cfg2(closestTickCount, 0x10000UL, this->isLpTimer());
	const _TimerConfig & cfg = (cfg1.error < cfg2.error) ? cfg1 : cfg2;
	this->setConfiguration(cfg.prescaler, cfg.period);
}


/**
 * Performs a fresh start of the timer.
 */
void HardwareTimer::start() {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		__HAL_TIM_CLEAR_FLAG(this->handle.tim, TIM_FLAG_UPDATE);
		HAL_TIM_Base_Start_IT(this->handle.tim);
		__HAL_TIM_SET_COUNTER(this->handle.tim, 0);
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		__HAL_LPTIM_CLEAR_FLAG(this->handle.lptim, LPTIM_FLAG_CMPM);
		HAL_LPTIM_Counter_Start_IT(this->handle.lptim, this->period);
		__HAL_LPTIM_COMPARE_SET(this->handle.lptim, this->period);
		__HAL_LPTIM_ENABLE_IT(this->handle.lptim, LPTIM_IT_CMPM);
	}
#endif /* LPTIM_PRESCALER_DIV1 */
}


/**
 * Stops the timer.
 */
void HardwareTimer::stop() {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		this->lastCounter = CounterType(__HAL_TIM_GET_COUNTER(this->handle.tim));
		HAL_TIM_Base_Stop_IT(this->handle.tim);
		LL_TIM_DisableCounter_Wrapper(this->handle.tim->Instance);
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		this->lastCounter = getLpTimCounter(this->handle.lptim);
		HAL_LPTIM_Counter_Stop_IT(this->handle.lptim);
	}
#endif /* LPTIM_PRESCALER_DIV1 */
}


/**
 * Resumes a previously stopped timer.
 */
void HardwareTimer::resume() {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		if (this->callback != NULL) {
			__HAL_TIM_CLEAR_FLAG(this->handle.tim, TIM_FLAG_UPDATE);
			__HAL_TIM_SET_COUNTER(this->handle.tim, this->lastCounter);
			HAL_TIM_Base_Start_IT(this->handle.tim);
		}
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		if (this->callback != NULL) {
			__HAL_LPTIM_CLEAR_FLAG(this->handle.lptim, LPTIM_FLAG_CMPM);
			HAL_LPTIM_Counter_Start_IT(this->handle.lptim, this->period);
			__HAL_LPTIM_COMPARE_SET(this->handle.lptim, this->lastCounter);
			__HAL_LPTIM_ENABLE_IT(this->handle.lptim, LPTIM_IT_CMPM);
		}
	}
#endif /* LPTIM_PRESCALER_DIV1 */
}


/**
 * Attaches a callback function to this timer.
 * 
 * @param[in] cb - new callback
 */
void HardwareTimer::attachInterrupt(void (*cb)(void)) {
	this->callback = cb;
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		if (cb != NULL) {
			__HAL_TIM_CLEAR_FLAG(this->handle.tim, TIM_FLAG_UPDATE);
			__HAL_TIM_ENABLE_IT(this->handle.tim, TIM_IT_UPDATE);
		}
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		if (cb != NULL) {
			__HAL_LPTIM_CLEAR_FLAG(this->handle.lptim, LPTIM_FLAG_CMPM);
			__HAL_LPTIM_ENABLE_IT(this->handle.lptim, LPTIM_IT_CMPM);
		}
	}
#endif /* LPTIM_PRESCALER_DIV1 */
}


/**
 * Attaches a callback function to this timer and set the interval
 * to the given number of microseconds.
 * 
 * @param[in] cb - new callback
 * @param[in] microseconds - timer interval in microseconds
 */
void HardwareTimer::attachInterrupt(void (*cb)(void), const unsigned long microseconds) {
	this->setPeriod(microseconds);
	this->attachInterrupt(cb);
}


/**
 * Removes a possibly attached callback function from this timer.
 */
void HardwareTimer::detachInterrupt() {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		__HAL_TIM_DISABLE_IT(this->handle.tim, TIM_IT_UPDATE);
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		__HAL_LPTIM_DISABLE_IT(this->handle.lptim, LPTIM_IT_CMPM);
	}
#endif /* LPTIM_PRESCALER_DIV1 */
	this->callback = NULL;
}


/**
 * Sets the interrupt main and sub priority. This defaults to TIMER_IRQ_PRIO
 * and TIMER_IRQ_SUBPRIO if defined. This needs to be called after each call
 * to HardwareTimer::initialize() if a different priority is required.
 * 
 * @param[in] mainPrio - main priority
 * @param[in] subPrio - sub priority
 */
void HardwareTimer::setInterruptPriority(const uint32_t mainPrio, const uint32_t subPrio) {
	HAL_NVIC_SetPriority(this->irq, mainPrio, subPrio);
}


/**
 * Returns whether the used timer instance is a low power timer.
 * 
 * @return true if low power timer, else false
 */
inline bool HardwareTimer::isLpTimer() const {
	return _TimerPinMap::isLowPowerInstance(this->instNum);
}


/**
 * Sets the timer configuration from the given prescaler and period value. These
 * values are expected to be correctly calculated to be passed as it to the HAL API.
 * This includes proper constants for aPrescaler in case of LPTIM.
 * 
 * @param[in] aPrescaler - prescaler value
 * @param[in] aPeriod - period value
 */
void HardwareTimer::setConfiguration(const uint32_t aPrescaler, const uint32_t aPeriod) {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	if ( ! this->isLpTimer() ) {
		this->handle.tim->Init.Prescaler = aPrescaler;
		this->handle.tim->Init.Period = aPeriod;
		if (this->handle.tim->State != HAL_TIM_STATE_RESET) {
			LL_TIM_SetPrescaler_Wrapper(this->handle.tim->Instance, aPrescaler);
			__HAL_TIM_SET_AUTORELOAD(this->handle.tim, aPeriod);
		}
	}
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	if ( this->isLpTimer() ) {
		this->handle.lptim->Init.Clock.Prescaler = aPrescaler;
		this->period = static_cast<decltype(this->period)>(aPeriod);
		if (this->handle.lptim->State != HAL_LPTIM_STATE_RESET) {
			LL_LPTIM_SetPrescaler_Wrapper(this->handle.lptim->Instance, aPrescaler);
			__HAL_LPTIM_AUTORELOAD_SET(this->handle.lptim, aPeriod);
		}
	}
#endif /* LPTIM_PRESCALER_DIV1 */
}


extern "C" {


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
/**
 * Overwrites the STM32 HAL API handler for TIM period elapsed events.
 * This maps the TIM instance to a callback of the associated HardwareTimer instance.
 * 
 * @param[in,out] hTim - pointer to TIM handle
 * @see HardwareTimer::callback
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * hTim) {
	HardwareTimer * obj = reinterpret_cast<_TimerPinMap::TimHandle *>(hTim)->obj;
	if (obj != NULL && obj->callback != NULL) obj->callback();
}
#endif /* TIM_CLOCKPRESCALER_DIV1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
/**
 * Overwrites the STM32 HAL API handler for LPTIM compare match events.
 * This maps the LPTIM instance to a callback of the associated HardwareTimer instance.
 * 
 * @param[in,out] hLpTim - pointer to LPTIM handle
 * @see HardwareTimer::callback
 */
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef * hLpTim) {
	HardwareTimer * obj = reinterpret_cast<_TimerPinMap::LpTimHandle *>(hLpTim)->obj;
	if (obj != NULL && obj->callback != NULL) obj->callback();
}
#endif /* LPTIM_PRESCALER_DIV1 */


#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for TIMx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForTIM##x(void) { \
		if (tim##x##Handle.obj != NULL) HAL_TIM_IRQHandler(&(tim##x##Handle.tim)); \
	}
#ifdef TIM1_BASE
DEF_IRQ_HANDLER(1)
#endif /* TIM1_BASE */

#ifdef TIM2_BASE
DEF_IRQ_HANDLER(2)
#endif /* TIM2_BASE */

#ifdef TIM3_BASE
DEF_IRQ_HANDLER(3)
#endif /* TIM3_BASE */

#ifdef TIM4_BASE
DEF_IRQ_HANDLER(4)
#endif /* TIM4_BASE */

#ifdef TIM5_BASE
DEF_IRQ_HANDLER(5)
#endif /* TIM5_BASE */

#ifdef TIM6_BASE
DEF_IRQ_HANDLER(6)
#endif /* TIM6_BASE */

#ifdef TIM7_BASE
DEF_IRQ_HANDLER(7)
#endif /* TIM7_BASE */

#ifdef TIM8_BASE
DEF_IRQ_HANDLER(8)
#endif /* TIM8_BASE */

#ifdef TIM9_BASE
DEF_IRQ_HANDLER(9)
#endif /* TIM9_BASE */

#ifdef TIM10_BASE
DEF_IRQ_HANDLER(10)
#endif /* TIM10_BASE */

#ifdef TIM11_BASE
DEF_IRQ_HANDLER(11)
#endif /* TIM11_BASE */

#ifdef TIM12_BASE
DEF_IRQ_HANDLER(12)
#endif /* TIM12_BASE */

#ifdef TIM13_BASE
DEF_IRQ_HANDLER(13)
#endif /* TIM13_BASE */

#ifdef TIM14_BASE
DEF_IRQ_HANDLER(14)
#endif /* TIM14_BASE */

#ifdef TIM15_BASE
DEF_IRQ_HANDLER(15)
#endif /* TIM15_BASE */

#ifdef TIM16_BASE
DEF_IRQ_HANDLER(16)
#endif /* TIM16_BASE */

#ifdef TIM17_BASE
DEF_IRQ_HANDLER(17)
#endif /* TIM17_BASE */

#ifdef TIM18_BASE
DEF_IRQ_HANDLER(18)
#endif /* TIM18_BASE */

#ifdef TIM19_BASE
DEF_IRQ_HANDLER(19)
#endif /* TIM19_BASE */

#ifdef TIM20_BASE
DEF_IRQ_HANDLER(20)
#endif /* TIM20_BASE */

#ifdef TIM21_BASE
DEF_IRQ_HANDLER(21)
#endif /* TIM21_BASE */

#ifdef TIM22_BASE
DEF_IRQ_HANDLER(22)
#endif /* TIM22_BASE */

#ifdef TIM23_BASE
DEF_IRQ_HANDLER(23)
#endif /* TIM23_BASE */

#ifdef TIM24_BASE
DEF_IRQ_HANDLER(24)
#endif /* TIM24_BASE */
#undef DEF_IRQ_HANDLER
#endif /* TIM_CLOCKPRESCALER_DIV1 */


#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for LPTIMx interrupt. */ \
	void STM32CubeDuinoIrqHandlerForLPTIM##x(void) { \
		if (lpTim##x##Handle.obj != NULL) HAL_LPTIM_IRQHandler(&(lpTim##x##Handle.lptim)); \
	}
#ifdef LPTIM1_BASE
DEF_IRQ_HANDLER(1)
#endif /* LPTIM1_BASE */

#ifdef LPTIM2_BASE
DEF_IRQ_HANDLER(2)
#endif /* LPTIM2_BASE */

#ifdef LPTIM3_BASE
DEF_IRQ_HANDLER(3)
#endif /* LPTIM3_BASE */

#ifdef LPTIM4_BASE
DEF_IRQ_HANDLER(4)
#endif /* LPTIM4_BASE */

#ifdef LPTIM5_BASE
DEF_IRQ_HANDLER(5)
#endif /* LPTIM5_BASE */
#undef DEF_IRQ_HANDLER
#endif /* LPTIM_PRESCALER_DIV1 */
} /* extern "C" */


#endif /* TIM_CLOCKPRESCALER_DIV1 or LPTIM_PRESCALER_DIV1 */
