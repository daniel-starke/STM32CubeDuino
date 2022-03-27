/**
 * @file main.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-20
 * @version 2022-03-20
 * 
 * Simple blinky example.
 */
#include <Arduino.h>


void setup() {
	pinMode(PA_1, OUTPUT);
}


void loop() {
	static bool value = false;
	digitalWrite(PA_1, uint32_t(value));
	value = !value;
	delay(500);
}
