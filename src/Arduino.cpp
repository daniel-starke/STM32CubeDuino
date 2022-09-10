/**
 * @file Arduino.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-12
 * @version 2022-09-01
 */
#include "Arduino.h"


/* forward declarations */
GPIO_TypeDef * enableGpioClock(const PortName port);
#ifdef DWT_BASE
bool dwtInit();
#endif /* DWT_BASE */
extern "C" {
extern const char _wiring_irq;
/**
 * PlatformIO puts all object files in a single archive before entering the link stage.
 * However, wiring_irq.c only contains interrupt handlers and wrappers so no external
 * reference exists to that file which is why we need to add one in order to force the
 * linker to include that object file. Finally, the linker removes this variable as no
 * function references it.
 */
volatile char _wiring_irq_forced_linkage __attribute__((unused)) = _wiring_irq;
} /* extern "C" */


/**
 * Called before main and before any static object creation.
 * This is needed to ensure that the system is running before
 * any API access happens.
 * 
 * @remarks 101 is the static construction priority that happens first.
 * Priority values up to and including 100 are reserved for internal use.
 * @see init_priority() in https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Attributes.html
 */
extern "C" __attribute__((constructor(101)))
void preMain(void) {
	init();
	initVariant();
	/* USBDevice has not been created yet at this point. Hence, initialization is in main(). */
}


/**
 * Main entry point.
 */
int main() {
#ifdef USBCON
	USBDevice.init();
	USBDevice.attach();
#endif /* USBCON */
	setup(); /* call user defined setup function */
	for (;;) {
		loop(); /* call user defined loop function */
		serialEventRun();
	}
	__builtin_unreachable();
}


/** Dummy function for undefined hooks. */
extern "C" {
static void __empty(void) {}
void yield(void) __attribute__((weak, alias("__empty")));
void setupUSB(void) __attribute__((weak, alias("__empty")));
void serialEventRun(void) __attribute__((weak, alias("__empty")));
int atexit(void (* /* func */)()) { return 0; }
} /* extern "C" */


/* Arduino specific functions */
/**
 * System initialization.
 */
void init(void) {
#ifdef NVIC_PRIORITYGROUP_4
	/* needed for FreeRTOS: http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#endif
#if (__CORTEX_M == 0x07U)
	noInterrupts();
#ifndef I_CACHE_DISABLED
	SCB_EnableICache();
#endif
#ifndef D_CACHE_DISABLED
	SCB_EnableDCache();
#endif
	interrupts();
#endif
#ifdef DWT_BASE
	dwtInit();
#endif /* DWT_BASE */
	HAL_Init();
}


/**
 * Returns the number of microseconds passed since system start.
 * 
 * @return microseconds counter
 */
uint32_t micros(void) {
	/* Ensure COUNTFLAG is reset by reading SysTick control and status register */
	LL_SYSTICK_IsActiveCounterFlag();
	uint32_t ms = HAL_GetTick();
	const uint32_t tms = SysTick->LOAD + 1;
	uint32_t us = tms - SysTick->VAL;
	if ( LL_SYSTICK_IsActiveCounterFlag() ) {
		ms = HAL_GetTick();
		us = tms - SysTick->VAL;
	}
	return ((ms * 1000) + (us * 1000) / tms);
}


/**
 * Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 * 
 * @param[in] pin - named pin (input)
 * @param[in] state - LOW or HIGH
 * @param[in] timeout - timeout in microseconds
 * @return pulse length in microseconds or 0 on timeout
 * @warning This function relies on `micros()` and cannot be used in noInterrupt() context.
 */
uint32_t pulseInLong(const uint32_t pin, const uint32_t state, const uint32_t timeout) {
#if LL_GPIO_PIN_0 == 0x00000001 && LL_GPIO_PIN_1 == 0x00000002 && LL_GPIO_PIN_2 == 0x0000004 && LL_GPIO_PIN_3 == 0x0000008 \
	&& LL_GPIO_PIN_4 == 0x0000010 && LL_GPIO_PIN_5 == 0x0000020 && LL_GPIO_PIN_6 == 0x0000040 && LL_GPIO_PIN_7 == 0x0000080 \
	&& LL_GPIO_PIN_8 == 0x0000100 && LL_GPIO_PIN_9 == 0x0000200 && LL_GPIO_PIN_10 == 0x0000400 && LL_GPIO_PIN_11 == 0x0000800 \
	&& LL_GPIO_PIN_12 == 0x0001000 && LL_GPIO_PIN_13 == 0x0002000 && LL_GPIO_PIN_14 == 0x0004000 && LL_GPIO_PIN_15 == 0x0008000
	const uint32_t bit = getInternalPin(pin);
	volatile uint32_t & port = getInternalPort(digitalPinToPort(pin))->IDR;
	const uint32_t stateMask = (state ? bit : 0);
	const uint32_t startMicros = micros();
	/* wait for any previous pulse to end */
	while ((port & bit) == stateMask) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	/* wait for the pulse to start */
	while ((port & bit) != stateMask) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	const uint32_t start = micros();
	/* wait for the pulse to stop */
	while ((port & bit) == stateMask) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	return micros() - start;
#else /* non-trivial match */
	const uint32_t llPin = getInternalPin(pin);
	GPIO_TypeDef * port = getInternalPort(digitalPinToPort(pin));
	bool (* const check)(const uint32_t) = (state == LOW)
		? [](const uint32_t val) { return val == 0; }
		: [](const uint32_t val) { return val != 0; };
	const uint32_t startMicros = micros();
	/* wait for any previous pulse to end */
	while ( check(LL_GPIO_IsInputPinSet(port, llPin)) ) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	/* wait for the pulse to start */
	while ( ! check(LL_GPIO_IsInputPinSet(port, llPin)) ) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	const uint32_t start = micros();
	/* wait for the pulse to stop */
	while ( check(LL_GPIO_IsInputPinSet(port, llPin)) ) {
		if ((micros() - startMicros) > timeout) return 0;
	}
	return micros() - start;
#endif
}


/**
 * Shifts out the given octet one bit at a time. Starts from either the most or least significant bit.
 * Each bit is written to the data pin at the falling edge of the clock pin. To interface devices that
 * are clocked by rising edges the clock pin needs to be set low (e.g. `digitalWrite(clockPin, LOW)`)
 * before calling this function.
 * 
 * @param[in] dataPin - named data pin (output)
 * @param[in] clockPin - named clock pin (output)
 * @param[in] bitOrder - `MSBFIRST` (most significant bit first) or `LSBFIRST` (least significant bit first)
 * @param[in] value - data to shift out
 */
void shiftOut(const uint32_t dataPin, const uint32_t clockPin, const uint32_t bitOrder, const uint8_t value) {
	uint32_t valReg = value;
	for (uint32_t i = 0; i < 8; i++) {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, valReg & 0x01);
			valReg >>= 1;
		} else {
			digitalWrite(dataPin, valReg & 0x80);
			valReg <<= 1;
		}
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}


/**
 * Shifts in an octet on bit at a time. Starts from either the most or the least significant bit.
 * The clock pin is set high for each bit taken from the data pin. To interface devices that are
 * clocked by rising edges the clock pin needs to be set low (e.g. `digitalWrite(clockPin, LOW)`)
 * before calling this function.
 * 
 * @param[in] dataPin - named data pin (output)
 * @param[in] clockPin - named clock pin (output)
 * @param[in] bitOrder - `MSBFIRST` (most significant bit first) or `LSBFIRST` (least significant bit first)
 * @return value shifted in
 */
uint8_t shiftIn(const uint32_t dataPin, const uint32_t clockPin, const uint32_t bitOrder) {
	uint32_t value = 0;
	for (uint32_t i = 0; i < 8; i++)  {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST) {
			value = (value << 1) | digitalRead(dataPin);
		} else {	
			value = (value >> 1) | (digitalRead(dataPin) << 7);
		}
		digitalWrite(clockPin, LOW);
	}
	return uint8_t(value);
}


/**
 * Sets the mode for the given pin. Possible modes are:
 * - `INPUT`
 * - `INPUT_PULLUP`
 * - `INPUT_FLOATING`
 * - `INPUT_PULLDOWN`
 * - `INPUT_ANALOG`
 * - `OUTPUT`
 * - `OUTPUT_OPEN_DRAIN`
 * - `OUTPUT_ANALOG`
 * - `ALTERNATE_FUNCTION`
 * - `ALTERNATE_FUNCTION_OPEN_DRAIN`
 * 
 * @param[in] pin - pin to set
 * @param[in] mode - new pin mode
 * @param[in] altFn - alternate pin function (STM32 specific)
 * @remarks The function is not part of the legacy Arduino API.
 * @see `pinMode()`
 */
void pinModeEx(const uint32_t pin, const uint32_t mode, const uint32_t altFn) {
	if (pin == uint32_t(NC)) systemErrorHandler();
	if (((pin >> 4) & 0x0F) == PortIntern) return;
	const uint32_t llPin = getInternalPin(pin);
#if defined(LL_SYSCFG_PIN_RMP_PA11) && defined(LL_SYSCFG_PIN_RMP_PA12)
	/* handle pin remap */
	if (pin >= PA_9 && pin <= PA_12) {
		__HAL_RCC_SYSCFG_CLK_ENABLE();
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
	GPIO_TypeDef * const gpioPort = enableGpioClock(digitalPinToPort(pin));
	if (gpioPort == NULL) return;
	/* set pin speed to high if applicable */
#ifdef STM32F1 /* STMF1 supports this only for output mode */
	if (mode == OUTPUT || mode == ALTERNATE_FUNCTION) {
#endif
#ifdef LL_GPIO_SPEED_FREQ_VERY_HIGH
		LL_GPIO_SetPinSpeed(gpioPort, llPin, LL_GPIO_SPEED_FREQ_VERY_HIGH);
#else
		LL_GPIO_SetPinSpeed(gpioPort, llPin, LL_GPIO_SPEED_FREQ_HIGH);
#endif
#ifdef STM32F1
	}
#endif
	switch (mode) {
	case INPUT: /* same as INPUT_FLOATING */
	case INPUT_PULLUP:
	case INPUT_PULLDOWN:
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_INPUT);
#ifdef GPIO_MODE_AF_INPUT
		/* STM32F1 sets input mode via an alternate function.
		 * GPIO_MODE_AF_INPUT can be defined by the user when overwriting setAltFunction() to set the mode appropriate to that. */
		setAltFunction(gpioPort, llPin, GPIO_MODE_AF_INPUT);
#endif /* GPIO_MODE_AF_INPUT */
		break;
	case INPUT_ANALOG: /* also matches OUTPUT_ANALOG */
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_ANALOG);
		break;
	case OUTPUT:
		LL_GPIO_SetPinOutputType(gpioPort, llPin, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_OUTPUT);
		break;
	case OUTPUT_OPEN_DRAIN:
		LL_GPIO_SetPinOutputType(gpioPort, llPin, LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_OUTPUT);
		break;
	case ALTERNATE_FUNCTION:
		LL_GPIO_SetPinOutputType(gpioPort, llPin, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_ALTERNATE);
		setAltFunction(gpioPort, llPin, altFn);
		break;
	case ALTERNATE_FUNCTION_OPEN_DRAIN:
		LL_GPIO_SetPinOutputType(gpioPort, llPin, LL_GPIO_OUTPUT_OPENDRAIN);
		LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_ALTERNATE);
		setAltFunction(gpioPort, llPin, altFn);
		break;
	default:
		systemErrorHandler();
		break;
	}
#ifdef STM32F1
	const uint32_t llMode = LL_GPIO_GetPinMode(gpioPort, llPin);
#endif
	switch (mode) {
	case INPUT_PULLUP:
#ifdef STM32F1
		if (llMode == LL_GPIO_MODE_FLOATING) {
			LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_INPUT);
		}
#endif
		LL_GPIO_SetPinPull(gpioPort, llPin, LL_GPIO_PULL_UP);
		break;
	case INPUT_PULLDOWN:
#ifdef STM32F1
		if (llMode == LL_GPIO_MODE_FLOATING) {
			LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_INPUT);
		}
#endif
		LL_GPIO_SetPinPull(gpioPort, llPin, LL_GPIO_PULL_DOWN);
		break;
	default:
#ifdef STM32F1
		/*  Input+NoPull = Floating for F1 family */
		if (llMode == LL_GPIO_MODE_INPUT) {
			LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_FLOATING);
		}
#else
		LL_GPIO_SetPinPull(gpioPort, llPin, LL_GPIO_PULL_NO);
#endif
		break;
	}
#ifdef GPIO_ASCR_ASC0
	setAsc0Register(gpioPort, llPin);
#endif /* GPIO_ASCR_ASC0 */
}


/* STM32 specific functions */
/**
 * System error handler.
 */
__attribute__((weak,noreturn))
void systemErrorHandler() {
	for (;;);
	__builtin_unreachable();
}


/**
 * The user may install a custom alternate function method which gets called instead
 * of the default function setting in `pinMode()`.
 * 
 * @param[in,out] gpioPort - GPIO base address
 * @param[in] llPin - low level pin number
 * @param[in] altFn - requested alternate function
 */
__attribute__((weak))
void setAltFunction(GPIO_TypeDef * gpioPort, const uint32_t llPin, const uint32_t altFn) {
#ifdef STM32F1
	UNUSED(gpioPort);
	UNUSED(llPin);
	__HAL_RCC_AFIO_CLK_ENABLE();
	switch (altFn) {
	case 1: __HAL_AFIO_REMAP_SPI1_ENABLE(); break;
	case 2: __HAL_AFIO_REMAP_I2C1_ENABLE(); break;
	case 3: __HAL_AFIO_REMAP_USART1_ENABLE(); break;
	case 4: __HAL_AFIO_REMAP_USART2_ENABLE(); break;
#ifdef AFIO_MAPR_USART3_REMAP
	case 5: __HAL_AFIO_REMAP_USART3_ENABLE(); break;
	case 6: __HAL_AFIO_REMAP_USART3_PARTIAL(); break;
#endif
	case 7: __HAL_AFIO_REMAP_TIM1_PARTIAL(); break;
	case 8: __HAL_AFIO_REMAP_TIM3_PARTIAL(); break;
	case 9: __HAL_AFIO_REMAP_TIM2_ENABLE(); break;
	case 10: __HAL_AFIO_REMAP_TIM3_ENABLE(); break;
#ifdef AFIO_MAPR_CAN_REMAP1
	case 11: __HAL_AFIO_REMAP_CAN1_1(); break;
	case 12: __HAL_AFIO_REMAP_CAN1_2(); break;
	case 13: __HAL_AFIO_REMAP_CAN1_3(); break;
#endif
#ifdef AFIO_MAPR_TIM2ITR1_IREMAP
	case 14: __HAL_AFIO_TIM2ITR1_TO_USB(); break;
	case 15: __HAL_AFIO_TIM2ITR1_TO_ETH(); break;
#endif
	/* more values are not possible */
	default: break;
	}
#else
	if (llPin < LL_GPIO_PIN_8) {
		LL_GPIO_SetAFPin_0_7(gpioPort, llPin, altFn);
	} else {
		LL_GPIO_SetAFPin_8_15(gpioPort, llPin, altFn);
	}
#endif
}


/**
 * Enable GPIO clock and return GPIO base address.
 * 
 * @param[in] port - port to enable
 * @return GPIO base address
 */
GPIO_TypeDef * enableGpioClock(const PortName port) {
	GPIO_TypeDef * gpioPort;
	switch (port) {
	case PortA:
		gpioPort = GPIOA;
		__HAL_RCC_GPIOA_CLK_ENABLE();
		break;
	case PortB:
		gpioPort = GPIOB;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		break;
#ifdef GPIOC_BASE
	case PortC:
		gpioPort = GPIOC;
		__HAL_RCC_GPIOC_CLK_ENABLE();
		break;
#endif
#ifdef GPIOD_BASE
	case PortD:
		gpioPort = GPIOD;
		__HAL_RCC_GPIOD_CLK_ENABLE();
		break;
#endif
#ifdef GPIOE_BASE
	case PortE:
		gpioPort = GPIOE;
		__HAL_RCC_GPIOE_CLK_ENABLE();
		break;
#endif
#ifdef GPIOF_BASE
	case PortF:
		gpioPort = GPIOF;
		__HAL_RCC_GPIOF_CLK_ENABLE();
		break;
#endif
#ifdef GPIOG_BASE
	case PortG:
#if defined(STM32L4) && defined(PWR_CR2_IOSV)
		/* enable VDDIO2 supply for 14 I/Os (Port G[15:2]) */
		HAL_PWREx_EnableVddIO2();
#endif
		gpioPort = GPIOG;
		__HAL_RCC_GPIOG_CLK_ENABLE();
		break;
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	case PortH:
		gpioPort = GPIOH;
		__HAL_RCC_GPIOH_CLK_ENABLE();
		break;
#endif
#ifdef GPIOI_BASE
	case PortI:
		gpioPort = GPIOI;
		__HAL_RCC_GPIOI_CLK_ENABLE();
		break;
#endif
#ifdef GPIOJ_BASE
	case PortJ:
		gpioPort = GPIOJ;
		__HAL_RCC_GPIOJ_CLK_ENABLE();
		break;
#endif
#ifdef GPIOK_BASE
	case PortK:
		gpioPort = GPIOK;
		__HAL_RCC_GPIOK_CLK_ENABLE();
		break;
#endif
#ifdef GPIOZ_BASE
	case PortZ:
		gpioPort = GPIOZ;
		__HAL_RCC_GPIOZ_CLK_ENABLE();
		break;
#endif
	default:
		gpioPort = NULL;
		break;
	}
	return gpioPort;
}


#ifdef DWT_BASE
/**
 * Change Data Watchdog and Trace Unit access.
 * 
 * @param[in] enable - true to enable access, else false
 */
void dwtAccess(bool enable) {
#if (__CORTEX_M == 0x07U)
/*
 * Define DWT LSR mask which is (currently) not defined by the CMSIS.
 * Same as ITM LSR one.
 */
#ifndef DWT_LSR_Present_Msk
#define DWT_LSR_Present_Msk ITM_LSR_Present_Msk
#endif
#ifndef DWT_LSR_Access_Msk
#define DWT_LSR_Access_Msk ITM_LSR_Access_Msk
#endif
	uint32_t lsr = DWT->LSR;
	if ((lsr & DWT_LSR_Present_Msk) != 0) {
		if ( enable ) {
			if ((lsr & DWT_LSR_Access_Msk) != 0) {
				DWT->LAR = 0xC5ACCE55; /* locked */
			}
		} else {
			if ((lsr & DWT_LSR_Access_Msk) == 0) {
				DWT->LAR = 0; /* unlocked */
			}
		}
	}
#else /* __CORTEX_M */
	UNUSED(enable);
#endif /* __CORTEX_M */
}


/**
 * Initializes the Data Watchdog and Trace Unit for usage.
 * 
 * @return true on success, else false
 */
bool dwtInit() {
	/* Enable use of DWT. */
	if ( ! (CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) ) {
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	}
	/* Unlock. */
	dwtAccess(true);
	/* Reset the clock cycle counter value. */
	DWT->CYCCNT = 0;
	/* Enable clock cycle counter. */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	/* 3 NO OPERATION instructions. */
	_NOP();
	_NOP();
	_NOP();
	/* Check if clock cycle counter has started. */
	return DWT->CYCCNT == 0;
}
#endif /* DWT_BASE */


extern "C" {
/**
 * Additional HAL initialization handler.
 */
void HAL_MspInit() {
	static volatile uint8_t cmp[] = {0, 0x67, 0xF8, 0xE1, 0xC6, 0x99, 0x19, 0x67, 0x09, 0x0E, 0xE8, 0x6D, 0x17, 0x3E, 0xB3, 0xF7, 0xA5};
	if (SYSTICK_IRQ_PRIO != TICK_INT_PRIORITY || SYSTICK_IRQ_SUBPRIO != cmp[0]) {
		HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIO, SYSTICK_IRQ_SUBPRIO);
	}
	__HAL_RCC_SYSCFG_CLK_ENABLE();
#ifdef __HAL_RCC_PWR_CLK_ENABLE
	__HAL_RCC_PWR_CLK_ENABLE();
#endif /* __HAL_RCC_PWR_CLK_ENABLE */
}


/**
 * SysTick event handler.
 */
void SysTick_Handler() {
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
} /* extern "C" */
