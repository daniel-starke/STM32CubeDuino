/**
 * @file main.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-13
 * @version 2022-03-13
 * 
 * Simple blinky example with serial output.
 */
#include <Arduino.h>


void setup() {
	Serial2.begin(115200);
	delay(100);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial2.println("begin");
}


void loop() {
	static bool value = false;
	Serial2.println(value ? "LOW" : "HIGH");
	digitalWrite(LED_BUILTIN, uint32_t(value));
	value = !value;
	delay(500);
}
