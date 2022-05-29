/**
 * @file main.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-05-29
 * @version 2022-05-29
 * 
 * Simple blinky and EEPROM example with serial output.
 */
#include <Arduino.h>
#include <EEPROM.h>


void setup() {
	Serial1.begin(115200);
	delay(100);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial1.println("begin");
	/* EEPROM */
	Serial1.print("EEPROM size: ");
	Serial1.println(EEPROM.length());
	/* boot count */
	uint32_t bootCount;
	EEPROM.get(0, bootCount);
	Serial1.print("boot count: ");
	Serial1.println(bootCount);
	bootCount++;
	EEPROM.put(0, bootCount);
}


void loop() {
	static bool value = false;
	Serial1.println(value ? "LOW" : "HIGH");
	digitalWrite(LED_BUILTIN, uint32_t(value));
	value = !value;
	delay(500);
}
