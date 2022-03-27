/**
 * @file Arduino.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-10
 * @version 2022-03-19
 */
#ifndef __ARDUINO_H__
#define __ARDUINO_H__

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "ArduinoDefines.h"


/* Arduino specific functions */
/**
 * Returns the GPIO base address of the given port.
 * 
 * @param[in] port - port by name
 * @return associated GPIO base address
 */
constexpr static uintptr_t _getInternalPort(const uint32_t port) {
	uintptr_t res = 0;
	switch (port) {
	case PortA: res = GPIOA_BASE; break;
	case PortB: res = GPIOB_BASE; break;
#ifdef GPIOC_BASE
	case PortC: res = GPIOC_BASE; break;
#endif
#ifdef GPIOD_BASE
	case PortD: res = GPIOD_BASE; break;
#endif
#ifdef GPIOE_BASE
	case PortE: res = GPIOE_BASE; break;
#endif
#ifdef GPIOF_BASE
	case PortF: res = GPIOF_BASE; break;
#endif
#ifdef GPIOG_BASE
	case PortG: res = GPIOG_BASE; break;
#endif
#ifdef GPIOH_BASE
	case PortH: res = GPIOH_BASE; break;
#endif
#ifdef GPIOI_BASE
	case PortI: res = GPIOI_BASE; break;
#endif
#ifdef GPIOJ_BASE
	case PortJ: res = GPIOJ_BASE; break;
#endif
#ifdef GPIOK_BASE
	case PortK: res = GPIOK_BASE; break;
#endif
#ifdef GPIOZ_BASE
	case PortZ: res = GPIOZ_BASE; break;
#endif
	default: break;
	}
	return res;
}
#define getInternalPort(x) reinterpret_cast<GPIO_TypeDef *>(_getInternalPort((x)))

/**
 * Returns the low level pin mask from the given pin.
 * 
 * @param[in] pin - pin by name
 * @return associated low level pin mask
 */
constexpr static uint32_t getInternalPin(const uint32_t pin) {
	if (LL_GPIO_PIN_0 == 0x00000001 && LL_GPIO_PIN_1 == 0x00000002 && LL_GPIO_PIN_2 == 0x0000004 && LL_GPIO_PIN_3 == 0x0000008
		&& LL_GPIO_PIN_4 == 0x0000010 && LL_GPIO_PIN_5 == 0x0000020 && LL_GPIO_PIN_6 == 0x0000040 && LL_GPIO_PIN_7 == 0x0000080
		&& LL_GPIO_PIN_8 == 0x0000100 && LL_GPIO_PIN_9 == 0x0000200 && LL_GPIO_PIN_10 == 0x0000400 && LL_GPIO_PIN_11 == 0x0000800
		&& LL_GPIO_PIN_12 == 0x0001000 && LL_GPIO_PIN_13 == 0x0002000 && LL_GPIO_PIN_14 == 0x0004000 && LL_GPIO_PIN_15 == 0x0008000) {
		return 1 << (pin & 0xF);
	}
	/* non-trivial mapping function (optimized out if previous condition is true) */
	uint32_t res = 0;
	switch (pin & 0xF) {
	case 0: res = LL_GPIO_PIN_0; break;
	case 1: res = LL_GPIO_PIN_1; break;
	case 2: res = LL_GPIO_PIN_2; break;
	case 3: res = LL_GPIO_PIN_3; break;
	case 4: res = LL_GPIO_PIN_4; break;
	case 5: res = LL_GPIO_PIN_5; break;
	case 6: res = LL_GPIO_PIN_6; break;
	case 7: res = LL_GPIO_PIN_7; break;
	case 8: res = LL_GPIO_PIN_8; break;
	case 9: res = LL_GPIO_PIN_9; break;
	case 10: res = LL_GPIO_PIN_10; break;
	case 11: res = LL_GPIO_PIN_11; break;
	case 12: res = LL_GPIO_PIN_12; break;
	case 13: res = LL_GPIO_PIN_13; break;
	case 14: res = LL_GPIO_PIN_14; break;
	case 15: res = LL_GPIO_PIN_15; break;
	default: break;
	}
	return res;
}

#ifdef __cplusplus
extern "C" {
#endif

void init(void);
void initVariant(void);

void setup(void);
void loop(void);

void yield(void) __attribute__((weak));
void setupUSB(void) __attribute__((weak));
void serialEventRun(void) __attribute__((weak));
int atexit(void (* func)()) __attribute__((weak));

uint32_t micros(void);

static inline uint32_t millis(void) __attribute__((always_inline, unused));
static inline uint32_t millis(void) {
	return HAL_GetTick();
}

static inline void delay(const uint32_t ms) __attribute__((always_inline, unused));
static inline void delay(const uint32_t ms) {
	if (ms == 0) return;
	const uint32_t start = millis();
	do {
		yield();
	} while ((millis() - start) < ms);
}

static inline void delayMicroseconds(const uint32_t) __attribute__((always_inline, unused));
static inline void delayMicroseconds(const uint32_t us) {
#ifdef DWT_BASE
	/* target has a data watchdog and trace unit */
	const int32_t start = int32_t(DWT->CYCCNT);
	const int32_t cycles = us * (SystemCoreClock / 1000000);
	while ((int32_t(DWT->CYCCNT) - start) < cycles);
#else /* no DWT available */
	/* SysTick CTRL, LOAD and VAL are declared volatile and are always read */
	uint32_t currentTick = SysTick->VAL;
	const uint32_t ticksPerMs = SysTick->LOAD + 1;
	const uint32_t deadlineTicks = ((us - ((us > 0) ? 1 : 0)) * ticksPerMs) / 1000;
	/* accumulation is needed because VAL only counts up to one millisecond */
	uint32_t elapsedTicks = 0;
	uint32_t lastTick = currentTick;
	do {
		currentTick = SysTick->VAL;
		elapsedTicks += ((lastTick < currentTick) ? ticksPerMs : 0) + lastTick - currentTick;
		lastTick = currentTick;
	} while (elapsedTicks < deadlineTicks);
#endif
}


/**
 * Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * ATTENTION:
 * this function relies on micros() so cannot be used in noInterrupts() context
 * 
 * @param[in] pin - named pin (input)
 * @param[in] state - LOW or HIGH
 * @param[in] timeout - timeout in microseconds
 * @return pulse length in microseconds or 0 on timeout
 */
static inline uint32_t pulseIn(const uint32_t pin, const uint32_t state, const uint32_t timeout) __attribute__((always_inline, unused));
uint32_t pulseInLong(const uint32_t pin, const uint32_t state, const uint32_t timeout);
static inline uint32_t pulseIn(const uint32_t pin, const uint32_t state, const uint32_t timeout) {
	return pulseInLong(pin, state, timeout);
}


void shiftOut(const uint32_t dataPin, const uint32_t clockPin, const uint32_t bitOrder, const uint8_t value);
uint8_t shiftIn(const uint32_t dataPin, const uint32_t clockPin, const uint32_t bitOrder);


void attachInterrupt(const uint32_t pin, void (* userFunc)(void), const int mode);
void detachInterrupt(const uint32_t pin);


#define digitalPinToPort(x) PortName((x) >> 4)
#define digitalReadFast digitalRead
#define digitalWriteFast digitalWrite
#define digitalToggleFast digitalToggle

void pinModeEx(const uint32_t pin, const uint32_t mode, const uint32_t altFn); /* STM32 specific */

static inline void pinMode(const uint32_t pin, const uint32_t mode) __attribute__((always_inline, unused));
static inline void pinMode(const uint32_t pin, const uint32_t mode) {
	pinModeEx(pin, mode, 0);
}

/**
 * Reads a LOW or HIGH from the given pin. The pin needs to be in input mode.
 * 
 * @param[in] pin - named pin
 * @see `pinMode()`
 */
inline int digitalRead(const uint32_t pin) __attribute__((always_inline, unused));
inline int digitalRead(const uint32_t pin) {
	return LL_GPIO_IsInputPinSet(getInternalPort(digitalPinToPort(pin)), getInternalPin(pin));
}

/**
 * Writes a LOW or HIGH to the given pin. The pin needs to be in output mode.
 * 
 * @param[in] pin - named pin
 * @param[in] val - LOW or HIGH
 * @see `pinMode()`
 */
inline void digitalWrite(const uint32_t pin, const uint32_t val) __attribute__((always_inline, unused));
inline void digitalWrite(const uint32_t pin, const uint32_t val) {
	if (val != 0) {
		LL_GPIO_SetOutputPin(getInternalPort(digitalPinToPort(pin)), getInternalPin(pin));
	} else {
		LL_GPIO_ResetOutputPin(getInternalPort(digitalPinToPort(pin)), getInternalPin(pin));
	}
}

/**
 * Toggles the output value of the given pin. The pin needs to be in output mode.
 * 
 * @param[in] pin - named pin
 * @see `pinMode()`
 */
inline void digitalToggle(const uint32_t pin) __attribute__((always_inline, unused));
inline void digitalToggle(const uint32_t pin) {
	LL_GPIO_TogglePin(getInternalPort(digitalPinToPort(pin)), getInternalPin(pin));
}

/**
 * Sets the analog voltage reference.
 * 
 * @param[in] mode - new reference
 * @remarks This function is only included for compatibility reasons and has no effect.
 * STM32 use only the provided external reference voltage.
 * @see https://www.st.com/resource/en/application_note/cd00211314-how-to-get-the-best-adc-accuracy-in-stm32-microcontrollers-stmicroelectronics.pdf
 */
inline void analogReference(const AnalogReference mode) __attribute__((always_inline, unused));
inline void analogReference(const AnalogReference /* mode */) {}

extern void analogReadResolution(const int res);
extern uint32_t analogRead(const uint32_t pin);

extern void analogWriteResolution(int res);
extern void analogWrite(const uint32_t pin, const uint32_t value);
#ifndef STM32CUBEDUINO_LEGACY_API
extern void analogWriteFrequency(const uint32_t freq);
#endif /* STM32CUBEDUINO_LEGACY_API */


#ifdef __cplusplus
} /* extern "C" */
#endif


/* STM32 specific functions */
extern void systemErrorHandler() __attribute__((weak, noreturn));
extern void setAltFunction(GPIO_TypeDef * gpioPort, const uint32_t llPin, const uint32_t altFn) __attribute__((weak));
#ifdef STM32CUBEDUINO_INTERNALS
extern GPIO_TypeDef * enableGpioClock(const PortName port);
#endif /* STM32CUBEDUINO_INTERNALS */
#ifdef GPIO_ASCR_ASC0
extern void setAsc0Register(GPIO_TypeDef * gpioPort, const uint32_t llPin) __attribute__((weak));
#endif /* GPIO_ASCR_ASC0 */


/* Arduino specific functions */
#ifdef __cplusplus
long random(long _max);
long random(long _min, long _max);
void randomSeed(unsigned long seed);
long map(long val, long fromLow, long fromHigh, long toLow, long toHigh);

uint16_t makeWord(uint16_t w);
uint16_t makeWord(uint8_t h, uint8_t l);
#define word(...) makeWord(__VA_ARGS__)

void tone(const uint32_t pin, unsigned int frequency, unsigned long duration = 0);
void noTone(const uint32_t pin);
#endif /* __cplusplus */

#include "new.h"
#include "WCharacter.h"
#include "WString.h"
#include "Stream.h"
#include "HardwareSerial.h"
#include "USBAPI.h"
#ifdef HAVE_CDCSERIAL
#define Serial SerialUSB
#elif defined(HAVE_HWSERIAL0)
#define Serial Serial1
#endif /* else: no Serial available */


#endif /* __ARDUINO_H__ */
