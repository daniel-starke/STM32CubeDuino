/**
 * @file mcu.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-28
 * @version 2022-03-28
 */
#include <Arduino.h>

#define SIZE_IN_MB 1
//#define HAS_AVAILABLEFORWRITE

static char buf[256];

void setup() {
	Serial.begin(9600);
	while ( ! Serial );
	memset(buf, '\r', sizeof(buf));
	
	long tT = millis();
	while (Serial.read() < 0) tT = millis();
	for (size_t received = 1; received < (SIZE_IN_MB * 1048576);) {
		if (Serial.read() >= 0) received++;
	}
	tT = millis() - tT;
	
	long rT = millis();
#ifdef HAS_AVAILABLEFORWRITE
	for (size_t written = 0; written < (SIZE_IN_MB * 1048576);) {
		const size_t left = (SIZE_IN_MB * 1048576) - written;
		const size_t writable = min(min(sizeof(buf), left), size_t(Serial.availableForWrite()));
		written += Serial.write(reinterpret_cast<const uint8_t *>(buf), writable);
	}
#else
	for (size_t i = 0; i < (SIZE_IN_MB * 1048576 / sizeof(buf)); i++) {
		Serial.write(reinterpret_cast<const uint8_t *>(buf), sizeof(buf));
	}
#endif
	Serial.flush();
	rT = millis() - rT;
	
	Serial.print("\r\nPC -> MCU: ");
	Serial.print(tT);
	Serial.print(" ms for ");
	Serial.print(SIZE_IN_MB);
	Serial.print(" MiB, ");
	Serial.print(float(SIZE_IN_MB * 1024) / (float(tT) / 1000.0f), 1);
	Serial.print(" KiB/s\r\n");
	
	Serial.print("MCU -> PC: ");
	Serial.print(rT);
	Serial.print(" ms for ");
	Serial.print(SIZE_IN_MB);
	Serial.print(" MiB, ");
	Serial.print(float(SIZE_IN_MB * 1024) / (float(rT) / 1000.0f), 1);
	Serial.print(" KiB/s\r\n");
	Serial.print("done\r\n");
}

void loop() {
}
