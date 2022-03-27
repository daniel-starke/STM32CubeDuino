/**
 * @file main.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-20
 * @version 2022-03-20
 * 
 * Simple blinky example with serial output.
 */
#include <Arduino.h>


void setup() {
	Serial1.begin(115200);
	delay(100);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial1.println("begin");
}


void loop() {
	static bool value = false;
	Serial1.println(value ? "LOW" : "HIGH");
	digitalWrite(LED_BUILTIN, uint32_t(value));
	value = !value;
	delay(500);
}
