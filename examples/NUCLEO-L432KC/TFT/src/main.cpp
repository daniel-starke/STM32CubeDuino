/**
 * @file main.cpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-20
 * @version 2022-03-20
 * 
 * Reads the sensor data from a BME280 module and outputs it
 * on the NUCLEO-L432KC serial debug interface and on the
 * connected 240x320 SPI TFT module with ST7789V driver.
 * The wiring can be found in the comments and defines below.
 * 
 * Note: Adafruit_ST7789::setRotation() was modified to correct mirroring.
 */
#include <Arduino.h>
#include <SPI.h>
#include "Bme280.hpp"
#include "Adafruit_ST7789.h"
#include <stdio.h>


/* BME280 VIN to 3.3V
 * BME280 GND to GND
 */
#define BME280_SCL D1
#define BME280_SDA D0



/* TFT VCC to 3.3V
 * TFT GND to GND
 * TFT LED to 3.3V
 */
#define TFT_CS A2
#define TFT_RESET A1
#define TFT_DC A0
#define TFT_MOSI A6
#define TFT_SCLK A4
#define TFT_MISO A5


/** Converts 24-bit RGB to inverted 16-bit BGR (565 encoding). */
#define COLOR(r, g, b) uint16_t(((((r) >> 3) & 0x1F) | ((((g) >> 2) & 0x3F) << 5) | ((((b) >> 3) & 0x1F) << 11)) ^ 0xFFFF)
#define COLOR_BLACK COLOR(0, 0, 0)
#define COLOR_RED COLOR(255, 0, 0)
#define COLOR_GREEN COLOR(0, 255, 0)
#define COLOR_BLUE COLOR(0, 0, 255)
#define COLOR_YELLOW COLOR(255, 255, 0)
#define COLOR_WHITE COLOR(255, 255, 255)

#define COLOR_FOREGROUND COLOR_BLACK
#define COLOR_BACKGROUND COLOR_WHITE


Bme280 sensor;
Adafruit_ST7789 display(TFT_CS, TFT_DC, TFT_RESET);


void setup() {
	Serial2.begin(115200);
	delay(100);
	Serial2.println("begin");
	sensor.begin();
	display.init(240, 320);
	display.setRotation(2);
	display.fillScreen(COLOR_BACKGROUND);
	display.setTextColor(COLOR_FOREGROUND);
	display.setTextSize(2);
	display.setCursor(0, 0);
	display.print("Temperature [C]:");
	display.setCursor(0, 80);
	display.print("Pressure [kPa]:");
	display.setCursor(0, 160);
	display.print("Humidity [%]:");
	display.setTextSize(5);
}


void loop() {
	static char strBuf[32] = {0};
	const int32_t fineTemp = sensor.readFineTemperature();
	const float temp = sensor.readTemperature(fineTemp);
	const float press = sensor.readPressure(fineTemp);
	const float humid = sensor.readHumidity(fineTemp);
	Serial2.print(temp);
	Serial2.print(" °C, ");
	Serial2.print(press);
	Serial2.print(" Pa, ");
	Serial2.print(humid);
	Serial2.println(" %");
	/* temperature */
	snprintf(strBuf, sizeof(strBuf), "%.1f", temp);
	display.setCursor(0, 20);
	display.print(strBuf);
	/* pressure */
	snprintf(strBuf, sizeof(strBuf), "%.2f", press / 1000.0f);
	display.setCursor(0, 100);
	display.print(strBuf);
	/* humidity */
	snprintf(strBuf, sizeof(strBuf), "%.1f", humid);
	display.setCursor(0, 180);
	display.print(strBuf);
	delay(1000);
	/* next redraw is opaque */
	display.setTextColor(COLOR_FOREGROUND, COLOR_BACKGROUND);
}
