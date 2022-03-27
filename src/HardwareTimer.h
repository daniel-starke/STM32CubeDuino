/**
 * @file HardwareTimer.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-11-04
 * @version 2022-03-11
 */
#ifndef __HARDWARETIMER_H__
#define __HARDWARETIMER_H__


#if !defined(STM32CUBEDUINO_DISABLE_TIMER) && (defined(TIM_CLOCKPRESCALER_DIV1) || defined(LPTIM_PRESCALER_DIV1)) /* STM32 HAL TIM or LPTIM header was included */


/**
 * This hardware timer class is implemented with compatibility to Arduino's TimerOne
 * library in mind. However, the constructor is different due to the STM32 platform
 * and the additional methods setFrequency() and setInterruptPriority() are provided.
 */
class HardwareTimer {
private:
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *);
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	friend void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *);
#endif /* LPTIM_PRESCALER_DIV1 */
protected:
#if defined(TIM_CLOCKPRESCALER_DIV1) && defined(IS_TIM_32B_COUNTER_INSTANCE)
	typedef uint32_t CounterType;
#else /* has no 32-bit timer */
	typedef uint16_t CounterType;
#endif /* has no 32-bit timer */
	union {
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
		TIM_HandleTypeDef * tim;
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
		LPTIM_HandleTypeDef * lptim;
#endif /* LPTIM_PRESCALER_DIV1 */
	} handle;
	IRQn_Type irq;
	void (* callback)(void);
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
#if defined(LPTIM_CFGR_PRESC_Msk) && LPTIM_CFGR_PRESC_Msk < 0x10000
	uint16_t period; /**< needed for LPTIM */
#else /* not LPTIM_CFGR_PRESC_Msk or >= 0x1000 */
	uint32_t period; /**< needed for LPTIM */
#endif /* not LPTIM_CFGR_PRESC_Msk or >= 0x1000 */
#endif /* LPTIM_PRESCALER_DIV1 */
	CounterType lastCounter;
	uint8_t instNum;
public:
#ifdef TIM_CLOCKPRESCALER_DIV1 /* STM32 HAL TIM header was included */
	HardwareTimer(TIM_TypeDef * inst, const IRQn_Type irqNum);
#endif /* TIM_CLOCKPRESCALER_DIV1 */
#ifdef LPTIM_PRESCALER_DIV1 /* STM32 HAL LPTIM header was included */
	HardwareTimer(LPTIM_TypeDef * inst, const IRQn_Type irqNum);
#endif /* LPTIM_PRESCALER_DIV1 */
	~HardwareTimer();
	
	void initialize();
	void initialize(const unsigned long microseconds);
	void deinitialize();
	void setPeriod(const unsigned long microseconds);
	void setFrequency(const unsigned long hertz);
	
	void start();
	void stop();
	/** Restarts the timer. */
	inline void restart() {
		this->stop();
		this->start();
	}
	void resume();
	
	void attachInterrupt(void (*cb)(void));
	void attachInterrupt(void (*cb)(void), const unsigned long microseconds);
	void detachInterrupt();
	
	void setInterruptPriority(const uint32_t mainPrio, const uint32_t subPrio);
protected:
	inline bool isLpTimer() const;
	void setConfiguration(const uint32_t aPrescaler, const uint32_t aPeriod);
};


extern HardwareTimer Timer0; /**< For internal use in tone()/noTone(). May be defined by the user or in `board.cpp` to overwrite the default (TIM1 or LPTIM1). */
extern HardwareTimer Timer1; /**< Undefined by default. Can be defined by the user or in `board.cpp`. */
extern HardwareTimer Timer2; /**< Undefined by default. Can be defined by the user or in `board.cpp`. */
extern HardwareTimer Timer3; /**< Undefined by default. Can be defined by the user or in `board.cpp`. */
extern HardwareTimer Timer4; /**< Undefined by default. Can be defined by the user or in `board.cpp`. */
extern HardwareTimer Timer5; /**< Undefined by default. Can be defined by the user or in `board.cpp`. */


#endif /* TIM_CLOCKPRESCALER_DIV1 or LPTIM_PRESCALER_DIV1 */
#endif /* __HARDWARESERIAL_H__ */
