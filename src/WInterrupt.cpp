/**
 * @file WInterrupt.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-17
 * @version 2022-04-03
 */
#include "Arduino.h"
#include "scdinternal/fifo.h"
#include "wiring_irq.h"


#if !defined(STM32CUBEDUINO_DISABLE_EXTI)


#define PP_CAT(x, ...) PP_CAT_HELPER(x, __VA_ARGS__)
#define PP_CAT_HELPER(x, ...) x ## __VA_ARGS__
#define EXTI_PORT(x) PP_CAT(PORT_PREFIX, x)
#define EXTI_LINE(x) PP_CAT(LINE_PREFIX, x)


#ifdef AFIO
#ifndef LL_GPIO_AF_EXTI_PORTA
#error Please include the STM32 HAL LL GPIO header in board.hpp. E.g. stm32f1xx_ll_gpio.h
#endif
#define PORT_PREFIX LL_GPIO_AF_EXTI_PORT
#define LINE_PREFIX LL_GPIO_AF_EXTI_LINE
#define SetEXTISource LL_GPIO_AF_SetEXTISource
#elif defined(LL_EXTI_EXTI_PORTA) /* not AFIO */
#define PORT_PREFIX LL_EXTI_EXTI_PORT
#define LINE_PREFIX LL_EXTI_EXTI_LINE
#define SetEXTISource LL_EXTI_SetEXTISource
#elif defined(LL_EXTI_CONFIG_PORTA) /* not (AFIO or EXTI) */
#define PORT_PREFIX LL_EXTI_CONFIG_PORT
#define LINE_PREFIX LL_EXTI_CONFIG_LINE
#define SetEXTISource LL_EXTI_SetEXTISource
#else /* not (AFIO or EXTI or CONFIG) */
#ifndef LL_SYSCFG_EXTI_PORTA
#error Please include the STM32 HAL LL SYSTEM header in board.hpp. E.g. stm32l4xx_ll_system.h
#endif
#define PORT_PREFIX LL_SYSCFG_EXTI_PORT
#define LINE_PREFIX LL_SYSCFG_EXTI_LINE
#define SetEXTISource LL_SYSCFG_SetEXTISource
#endif /* not AFIO */


#ifndef SetEXTISource
#error Please include the STM32 HAL LL EXTI header in board.hpp. E.g. stm32f1xx_ll_exti.h
#endif


/** Number of GPIO interrupt lines. */
#define NUM_INTERRUPTS 16


#ifndef EXTI_IRQ_PRIO
#error Please define EXTI_IRQ_PRIO in board.hpp.
#endif
#ifndef EXTI_IRQ_SUBPRIO
#error Please define EXTI_IRQ_SUBPRIO in board.hpp.
#endif


/* exported functions */
extern GPIO_TypeDef * enableGpioClock(const PortName port);


namespace {


/**
 * This table maps the HAL GPIO pin to the corresponding interrupt number.
 */
const IRQn_Type irqTable[NUM_INTERRUPTS] = {
	getIrqNumFor(EXTI0),
	getIrqNumFor(EXTI1),
	getIrqNumFor(EXTI2),
	getIrqNumFor(EXTI3),
	getIrqNumFor(EXTI4),
	getIrqNumFor(EXTI5),
	getIrqNumFor(EXTI6),
	getIrqNumFor(EXTI7),
	getIrqNumFor(EXTI8),
	getIrqNumFor(EXTI9),
	getIrqNumFor(EXTI10),
	getIrqNumFor(EXTI11),
	getIrqNumFor(EXTI12),
	getIrqNumFor(EXTI13),
	getIrqNumFor(EXTI14),
	getIrqNumFor(EXTI15)
};


/**
 * This table keeps association between callback function and interrupt.
 */
void (*cbTable[NUM_INTERRUPTS])(void) = { NULL };


/**
 * Helper function to determine the maximum value in `portTable`.
 * 
 * @return maximum `portTable` value
 */
constexpr uint32_t maxPortTableValue() {
	const uint32_t portTableValues[] = {
		EXTI_PORT(A),
		EXTI_PORT(B),
#ifdef GPIOC_BASE
		EXTI_PORT(C),
#endif
#ifdef GPIOD_BASE
		EXTI_PORT(D),
#endif
#ifdef GPIOE_BASE
		EXTI_PORT(E),
#endif
#ifdef GPIOF_BASE
		EXTI_PORT(F),
#endif
#ifdef GPIOG_BASE
		EXTI_PORT(G),
#endif
#ifdef GPIOH_BASE
		EXTI_PORT(H),
#endif
#ifdef GPIOI_BASE
		EXTI_PORT(I),
#endif
#ifdef GPIOJ_BASE
		EXTI_PORT(J),
#endif
#ifdef GPIOK_BASE
		EXTI_PORT(K),
#endif
#ifdef GPIOZ_BASE
		EXTI_PORT(Z)
#endif
	};
	uint32_t res = 0;
	for (size_t i = 0; i < (sizeof(portTableValues) / sizeof(*portTableValues)); i++) {
		if (portTableValues[i] > res) res = portTableValues[i];
	}
	return res;
}


/**
 * Defines the smallest unsigned integer type that can hold the any value of `portTable`.
 */
typedef typename _FifoIndexTypeFor<maxPortTableValue()>::type PortTableValue;


/**
 * This table maps the pin number to the corresponding SYSCFG EXTI port.
 */
const PortTableValue portTable[] = {
	EXTI_PORT(A),
	EXTI_PORT(B),
#ifdef GPIOC_BASE
	EXTI_PORT(C),
#endif
#ifdef GPIOD_BASE
	EXTI_PORT(D),
#endif
#ifdef GPIOE_BASE
	EXTI_PORT(E),
#endif
#ifdef GPIOF_BASE
	EXTI_PORT(F),
#endif
#ifdef GPIOG_BASE
	EXTI_PORT(G),
#endif
#ifdef GPIOH_BASE
	EXTI_PORT(H),
#endif
#ifdef GPIOI_BASE
	EXTI_PORT(I),
#endif
#ifdef GPIOJ_BASE
	EXTI_PORT(J),
#endif
#ifdef GPIOK_BASE
	EXTI_PORT(K),
#endif
#ifdef GPIOZ_BASE
	EXTI_PORT(Z)
#endif
};


/**
 * This table maps the pin number to the corresponding SYSCFG EXTI line.
 */
constexpr uint32_t __attribute__((unused)) lineTable[] = {
	EXTI_LINE(0),
	EXTI_LINE(1),
	EXTI_LINE(2),
	EXTI_LINE(3),
	EXTI_LINE(4),
	EXTI_LINE(5),
	EXTI_LINE(6),
	EXTI_LINE(7),
	EXTI_LINE(8),
	EXTI_LINE(9),
	EXTI_LINE(10),
	EXTI_LINE(11),
	EXTI_LINE(12),
	EXTI_LINE(13),
	EXTI_LINE(14),
	EXTI_LINE(15)
};


/**
 * Template function to select the smallest known `getLine()` function.
 * 
 * @param[in] pinNum - pin number
 * @return corresponding SYSCFG EXTI line value
 */
template <size_t>
constexpr inline uint32_t getLine(const uint32_t);


/**
 * Returns the SYSCFG line value by mapping table.
 * 
 * @param[in] pinNum - pin number
 * @return corresponding SYSCFG EXTI line value
 */
template <>
constexpr inline uint32_t getLine<0>(const uint32_t pinNum) {
	return lineTable[pinNum];
}


/**
 * Returns the SYSCFG line value from a known function.
 * 
 * @param[in] pinNum - pin number
 * @return corresponding SYSCFG EXTI line value
 */
template <>
constexpr inline uint32_t getLine<1>(const uint32_t pinNum) {
	return (0xF0000U << (4 * (pinNum & 3))) | (pinNum >> 2);
}


/**
 * Returns the SYSCFG line value from a known function.
 * 
 * @param[in] pinNum - pin number
 * @return corresponding SYSCFG EXTI line value
 */
template <>
constexpr inline uint32_t getLine<2>(const uint32_t pinNum) {
	const uint32_t x = 4 * (pinNum & 3);
	return (x << (16 + x)) | (pinNum >> 2);
}


/**
 * Returns the smallest known implementation to map a pin number
 * to a SYSCFG line value.
 * 
 * @return template index used for `getLine<n>()`
 */
constexpr size_t smallestGetLineFn() {
	bool algorithm1 = true;
	bool algorithm2 = true;
	for (uint32_t i = 0; i < (sizeof(lineTable) / sizeof(*lineTable)); i++) {
		if (lineTable[i] != getLine<1>(i)) algorithm1 = false;
		if (lineTable[i] != getLine<2>(i)) algorithm2 = false;
	}
	if ( algorithm1 ) return 1;
	if ( algorithm2 ) return 2;
	return 0;
}


/**
 * Helper function to map the pin to the external interrupt line.
 * 
 * @param[in] pin - pin to map
 * @return corresponding external interrupt line
 */
static uint32_t getLineFromPin(const uint32_t pin) {
	if (LL_EXTI_LINE_0 == 0x00000001 && LL_EXTI_LINE_1 == 0x00000002 && LL_EXTI_LINE_2 == 0x0000004 && LL_EXTI_LINE_3 == 0x0000008
		&& LL_EXTI_LINE_4 == 0x0000010 && LL_EXTI_LINE_5 == 0x0000020 && LL_EXTI_LINE_6 == 0x0000040 && LL_EXTI_LINE_7 == 0x0000080
		&& LL_EXTI_LINE_8 == 0x0000100 && LL_EXTI_LINE_9 == 0x0000200 && LL_EXTI_LINE_10 == 0x0000400 && LL_EXTI_LINE_11 == 0x0000800
		&& LL_EXTI_LINE_12 == 0x0001000 && LL_EXTI_LINE_13 == 0x0002000 && LL_EXTI_LINE_14 == 0x0004000 && LL_EXTI_LINE_15 == 0x0008000) {
		return 1 << (pin & 0xF);
	}
	/* non-trivial mapping function (optimized out if previous condition is true) */
	uint32_t res = 0;
	switch (pin & 0xF) {
	case 0: res = LL_EXTI_LINE_0; break;
	case 1: res = LL_EXTI_LINE_1; break;
	case 2: res = LL_EXTI_LINE_2; break;
	case 3: res = LL_EXTI_LINE_3; break;
	case 4: res = LL_EXTI_LINE_4; break;
	case 5: res = LL_EXTI_LINE_5; break;
	case 6: res = LL_EXTI_LINE_6; break;
	case 7: res = LL_EXTI_LINE_7; break;
	case 8: res = LL_EXTI_LINE_8; break;
	case 9: res = LL_EXTI_LINE_9; break;
	case 10: res = LL_EXTI_LINE_10; break;
	case 11: res = LL_EXTI_LINE_11; break;
	case 12: res = LL_EXTI_LINE_12; break;
	case 13: res = LL_EXTI_LINE_13; break;
	case 14: res = LL_EXTI_LINE_14; break;
	case 15: res = LL_EXTI_LINE_15; break;
	default: break;
	}
	return res;
}


/**
 * The HAL GPIO pin defines (e.g. `GPIO_PIN_0`) have their n'th bit set.
 * This function reverses this by getting the position of the MSB set.
 * 
 * @param[in] val - HAL GPIO pin value
 * @return pin number (e.g. `gpioPinToNum(GPIO_PIN_3)` returns 3)
 */
inline size_t gpioPinToNum(uint32_t val) {
	if (GPIO_PIN_0 == 0x00000001 && GPIO_PIN_1 == 0x00000002 && GPIO_PIN_2 == 0x0000004 && GPIO_PIN_3 == 0x0000008
		&& GPIO_PIN_4 == 0x0000010 && GPIO_PIN_5 == 0x0000020 && GPIO_PIN_6 == 0x0000040 && GPIO_PIN_7 == 0x0000080
		&& GPIO_PIN_8 == 0x0000100 && GPIO_PIN_9 == 0x0000200 && GPIO_PIN_10 == 0x0000400 && GPIO_PIN_11 == 0x0000800
		&& GPIO_PIN_12 == 0x0001000 && GPIO_PIN_13 == 0x0002000 && GPIO_PIN_14 == 0x0004000 && GPIO_PIN_15 == 0x0008000) {
		if (val == 0) return 0;
		return size_t(__builtin_ffs(val) - 1);
	}
	uint8_t res = 0;
	switch (val) {
	case GPIO_PIN_0: res = 0; break;
	case GPIO_PIN_1: res = 1; break;
	case GPIO_PIN_2: res = 2; break;
	case GPIO_PIN_3: res = 3; break;
	case GPIO_PIN_4: res = 4; break;
	case GPIO_PIN_5: res = 5; break;
	case GPIO_PIN_6: res = 6; break;
	case GPIO_PIN_7: res = 7; break;
	case GPIO_PIN_8: res = 8; break;
	case GPIO_PIN_9: res = 9; break;
	case GPIO_PIN_10: res = 10; break;
	case GPIO_PIN_11: res = 11; break;
	case GPIO_PIN_12: res = 12; break;
	case GPIO_PIN_13: res = 13; break;
	case GPIO_PIN_14: res = 14; break;
	case GPIO_PIN_15: res = 15; break;
	default: break;
	}
	return size_t(res);
}


} /* namespace anonymous */


/**
 * Attaches a callback function to the given pin based on the passed interrupt mode.
 * This also activates interrupts on the pin. On of the following modes is possible:
 * - `CHANGE`
 * - `FALLING`
 * - `LOW`
 * - `RISING`
 * - `HIGH`
 * This defaults to `RISING` on invalid mode.
 * 
 * @param[in] pin - interrupt pin
 * @param[in] userFunc - callback function to call upon interrupt
 * @param[in] mode - interrupt detection mode
 * @remarks There can be only one callback function per pin, regardless of the port.
 * @remarks The pull-up/pull-down/no pull configuration remains unchanged.
 */
void attachInterrupt(const uint32_t pin, void (* userFunc)(void), const int mode) {
	if (pin == uint32_t(NC)) systemErrorHandler();
	GPIO_TypeDef * gpioPort = enableGpioClock(digitalPinToPort(pin));
	if (gpioPort == NULL) return;
	const uint32_t llPin = getInternalPin(pin);
	const uint32_t line = getLineFromPin(pin);
	const uint32_t pinNum = pin & 0x0F;
	const uint32_t portNum = (pin >> 4) & 0x0F;
	cbTable[pinNum] = NULL;
	SetEXTISource(uint32_t(portTable[portNum]), getLine<smallestGetLineFn()>(pinNum));
	/* Set pin mode. */
	LL_GPIO_SetPinMode(gpioPort, llPin, LL_GPIO_MODE_INPUT);
#ifdef GPIO_MODE_AF_INPUT
	/* STM32F1 sets input mode via an alternate function.
	 * GPIO_MODE_AF_INPUT can be defined by the user when overwriting setAltFunction() to set the mode appropriate to that. */
	setAltFunction(gpioPort, llPin, GPIO_MODE_AF_INPUT);
#endif /* GPIO_MODE_AF_INPUT */
	/* Configure external interrupt mode. */
	LL_EXTI_DisableEvent_0_31(line);
	LL_EXTI_EnableIT_0_31(line);
	switch (mode) {
	case FALLING:
	case LOW:
		LL_EXTI_DisableRisingTrig_0_31(line);
		LL_EXTI_EnableFallingTrig_0_31(line);
		break;
	case CHANGE:
		LL_EXTI_EnableFallingTrig_0_31(line);
		LL_EXTI_EnableRisingTrig_0_31(line);
		break;
	case RISING:
	case HIGH:
	default:
		LL_EXTI_DisableFallingTrig_0_31(line);
		LL_EXTI_EnableRisingTrig_0_31(line);
		break;
	}
	/* Enable and set EXTI interrupt. */
	HAL_NVIC_SetPriority(irqTable[pinNum], EXTI_IRQ_PRIO, EXTI_IRQ_SUBPRIO);
	HAL_NVIC_EnableIRQ(irqTable[pinNum]);
	delay(1); /* Avoid sporadic triggers after setup. */
	cbTable[pinNum] = userFunc;
}


/**
 * Detaches the associated interrupt callback function from the given pin.
 * 
 * @param[in] pin - interrupt pin
 */
void detachInterrupt(const uint32_t pin) {
	if (pin == uint32_t(NC)) systemErrorHandler();
	GPIO_TypeDef * gpioPort = enableGpioClock(digitalPinToPort(pin));
	if (gpioPort == NULL) return;
	const uint32_t line = getLineFromPin(pin);
	const uint32_t pinNum = pin & 0x0F;
	cbTable[pinNum] = NULL;
	LL_EXTI_DisableIT_0_31(line);
	LL_EXTI_DisableFallingTrig_0_31(line);
	LL_EXTI_DisableRisingTrig_0_31(line);
	/* Multiple interrupt lines may share a common IRQ number. Early out if another
	 * interrupt callback is still registered to the same interrupt number. */
	for (size_t i = 0; i < NUM_INTERRUPTS; i++) {
		if (irqTable[pinNum] == irqTable[i] && cbTable[i] != NULL) return;
	}
	/* Disable EXTI interrupt. */
	HAL_NVIC_DisableIRQ(irqTable[pinNum]);
}


/* interrupt IRQ handlers */
extern "C" {
/**
 * Overwrites the STM32 HAL API handler for GPIO interrupt events.
 * @param[in] llPin - interrupt pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t llPin) {
	const size_t id = gpioPinToNum(llPin);
	if (id < NUM_INTERRUPTS && cbTable[id] != NULL) {
		/* Only handle interrupts from external pins. */
		cbTable[id]();
	}
}


/**
 * Overwrites the STM32 HAL API handler for rising GPIO interrupt events.
 * @param[in] llPin - interrupt pin
 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t llPin) {
	HAL_GPIO_EXTI_Callback(llPin);
}


/**
 * Overwrites the STM32 HAL API handler for falling GPIO interrupt events.
 * @param[in] llPin - interrupt pin
 */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t llPin) {
	HAL_GPIO_EXTI_Callback(llPin);
}


#define DEF_IRQ_HANDLER(x) \
	/** IRQ handler for external interrupt on GPIO line x. */ \
	void STM32CubeDuinoIrqHandlerForEXTI##x(void) { \
		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_##x); \
	}

DEF_IRQ_HANDLER(0)
DEF_IRQ_HANDLER(1)
DEF_IRQ_HANDLER(2)
DEF_IRQ_HANDLER(3)
DEF_IRQ_HANDLER(4)
DEF_IRQ_HANDLER(5)
DEF_IRQ_HANDLER(6)
DEF_IRQ_HANDLER(7)
DEF_IRQ_HANDLER(8)
DEF_IRQ_HANDLER(9)
DEF_IRQ_HANDLER(10)
DEF_IRQ_HANDLER(11)
DEF_IRQ_HANDLER(12)
DEF_IRQ_HANDLER(13)
DEF_IRQ_HANDLER(14)
DEF_IRQ_HANDLER(15)
} /* extern "C" */


#endif /* not STM32CUBEDUINO_DISABLE_EXTI */
