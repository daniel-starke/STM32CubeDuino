/**
 * @file Tone.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-11-07
 * @version 2022-03-11
 */
#include "Arduino.h"
#include "HardwareTimer.h"


#if !defined(STM32CUBEDUINO_DISABLE_TIMER) && (defined(TIM_CLOCKPRESCALER_DIV1) || defined(LPTIM_PRESCALER_DIV1)) /* STM32 HAL TIM or LPTIM header was included */
static uint32_t toggleCount;
static uint32_t outputPin = uint32_t(NC);


/**
 * Called by the HardwareTimer Timer0 every time a rising or falling output edge
 * of the square wave needs to be generated.
 */
static void toneCallback() {
	if (outputPin == uint32_t(NC)) return;
	
	digitalToggle(outputPin);
	
	/* handle timeout */
	if (toggleCount > 0) {
		toggleCount = uint32_t(toggleCount - 1);
		if (toggleCount == 0) noTone(outputPin);
	}
}


/**
 * Generates a tone at the given frequency as square wave with a 50% duty cycle.
 * 
 * @param[in] pin - output pin
 * @param[in] frequency - tone frequency in Hertz
 * @param[in] duration - optional duration in milliseconds
 */
void tone(const uint32_t pin, unsigned int frequency, unsigned long duration) {
	if (pin == uint32_t(NC)) return;
	if (outputPin != uint32_t(NC)) noTone(outputPin);
	const uint32_t doubleFreq = frequency * 2;
	toggleCount = (doubleFreq * duration) / 1000;
	outputPin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(outputPin, HIGH);
	Timer0.attachInterrupt(toneCallback);
	Timer0.setFrequency(doubleFreq);
	Timer0.initialize();
	Timer0.start();
}


/**
 * Stops a previously started tone output.
 * 
 * @param[in] pin - output pin
 */
void noTone(const uint32_t pin) {
	if (outputPin != pin) return;
	Timer0.deinitialize();
	digitalWrite(outputPin, LOW);
	outputPin = uint32_t(NC);
}
#endif /* TIM_CLOCKPRESCALER_DIV1 or LPTIM_PRESCALER_DIV1 */
