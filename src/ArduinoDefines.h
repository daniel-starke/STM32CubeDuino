/**
 * @file ArduinoDefines.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-16
 * @version 2022-03-20
 */
#ifndef __ARDUINO_DEFINES_H__
#define __ARDUINO_DEFINES_H__

#include "Version.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <board.hpp>
#include "binary.h"


/* Arduino specific defines. */
#ifndef F_CPU
#define F_CPU SystemCoreClock
#endif /* F_CPU */

#ifndef SYSTICK_IRQ_PRIO
#define SYSTICK_IRQ_PRIO TICK_INT_PRIORITY
#endif
#ifndef SYSTICK_IRQ_SUBPRIO
#define SYSTICK_IRQ_SUBPRIO 0
#endif

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define INPUT_FLOATING INPUT
#define INPUT_PULLDOWN 3
#define INPUT_ANALOG 4
#define OUTPUT_OPEN_DRAIN 5
#define OUTPUT_ANALOG INPUT_ANALOG /* STM32 specific */
#define ALTERNATE_FUNCTION 6 /* STM32 specific */
#define ALTERNATE_FUNCTION_OPEN_DRAIN 7 /* STM32 specific */

#define LOW 0
#define HIGH 1
#define CHANGE 2
#define FALLING 3
#define RISING 4

#define DEFAULT 1
#define EXTERNAL 0

#define SERIAL 0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

#define PSTR(...) __VA_ARGS__

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#undef abs
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define abs(x) (((x) > 0) ? (x) : -(x))
#define constrain(x, low, high) (((x) < (low)) ? (low) : (((x) > (high)) ? (high) : (x)))
#define round(x) (((x) >= 0) ? long((x) + 0.5) : long((x) - 0.5))
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define sq(x) ((x) * (x))
#undef RANDOM_MAX
#define RANDOM_MAX 0x7FFFFFFF

#define clockCyclesPerMicrosecond() (SystemCoreClock / 1000000L)
#define clockCyclesToMicroseconds(x) (((x) * 1000L) / (SystemCoreClock / 1000L))
#define microsecondsToClockCycles(x) ((x) * (SystemCoreClock / 1000000L))

#define lowByte(x) ((uint8_t)((x) & 0xFF))
#define highByte(x) ((uint8_t)((x) >> 8))

#define bit(x) (1UL << (x))
#define bitRead(x, bit) (((x) >> (bit)) & 0x01)
#define bitSet(x, bit) ((x) = (__typeof__(x))((x) | (1UL << (bit))))
#define bitClear(x, bit) ((x) = (__typeof__(x))((x) & (~(1UL << (bit)))))
#define bitWrite(x, bit, val) ((val) ? bitSet((x), (bit)) : bitClear((x), (bit)))

#ifndef _NOP
#define _NOP() __asm__ volatile ("nop")
#endif /* _NOP*/

typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

enum PortName {
	FirstPort = 0x00,
	PortA = FirstPort,
	PortB,
#ifdef GPIOC_BASE
	PortC,
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	PortD,
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	PortE,
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	PortF,
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	PortG,
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	PortH,
#endif /* GPIOH_BASE */
#ifdef GPIOI_BASE
	PortI,
#endif /* GPIOI_BASE */
#ifdef GPIOJ_BASE
	PortJ,
#endif /* GPIOJ_BASE */
#ifdef GPIOK_BASE
	PortK,
#endif /* GPIOK_BASE */
#ifdef GPIOZ_BASE
	PortZ,
#endif /* GPIOZ_BASE */
	PortIntern,
	PortCount
};

enum PinName {
	NC = -1,
	/* A */
	PA_0  = (PortA << 4) + 0x00,
	PA_1  = (PortA << 4) + 0x01,
	PA_2  = (PortA << 4) + 0x02,
	PA_3  = (PortA << 4) + 0x03,
	PA_4  = (PortA << 4) + 0x04,
	PA_5  = (PortA << 4) + 0x05,
	PA_6  = (PortA << 4) + 0x06,
	PA_7  = (PortA << 4) + 0x07,
	PA_8  = (PortA << 4) + 0x08,
	PA_9  = (PortA << 4) + 0x09,
	PA_10 = (PortA << 4) + 0x0A,
	PA_11 = (PortA << 4) + 0x0B,
	PA_12 = (PortA << 4) + 0x0C,
	PA_13 = (PortA << 4) + 0x0D,
	PA_14 = (PortA << 4) + 0x0E,
	PA_15 = (PortA << 4) + 0x0F,
	/* B */
	PB_0  = (PortB << 4) + 0x00,
	PB_1  = (PortB << 4) + 0x01,
	PB_2  = (PortB << 4) + 0x02,
	PB_3  = (PortB << 4) + 0x03,
	PB_4  = (PortB << 4) + 0x04,
	PB_5  = (PortB << 4) + 0x05,
	PB_6  = (PortB << 4) + 0x06,
	PB_7  = (PortB << 4) + 0x07,
	PB_8  = (PortB << 4) + 0x08,
	PB_9  = (PortB << 4) + 0x09,
	PB_10 = (PortB << 4) + 0x0A,
	PB_11 = (PortB << 4) + 0x0B,
	PB_12 = (PortB << 4) + 0x0C,
	PB_13 = (PortB << 4) + 0x0D,
	PB_14 = (PortB << 4) + 0x0E,
	PB_15 = (PortB << 4) + 0x0F,
#ifdef GPIOC_BASE
	PC_0  = (PortC << 4) + 0x00,
	PC_1  = (PortC << 4) + 0x01,
	PC_2  = (PortC << 4) + 0x02,
	PC_3  = (PortC << 4) + 0x03,
	PC_4  = (PortC << 4) + 0x04,
	PC_5  = (PortC << 4) + 0x05,
	PC_6  = (PortC << 4) + 0x06,
	PC_7  = (PortC << 4) + 0x07,
	PC_8  = (PortC << 4) + 0x08,
	PC_9  = (PortC << 4) + 0x09,
	PC_10 = (PortC << 4) + 0x0A,
	PC_11 = (PortC << 4) + 0x0B,
	PC_12 = (PortC << 4) + 0x0C,
	PC_13 = (PortC << 4) + 0x0D,
	PC_14 = (PortC << 4) + 0x0E,
	PC_15 = (PortC << 4) + 0x0F,
#endif /* GPIOC_BASE */
#ifdef GPIOD_BASE
	PD_0  = (PortD << 4) + 0x00,
	PD_1  = (PortD << 4) + 0x01,
	PD_2  = (PortD << 4) + 0x02,
	PD_3  = (PortD << 4) + 0x03,
	PD_4  = (PortD << 4) + 0x04,
	PD_5  = (PortD << 4) + 0x05,
	PD_6  = (PortD << 4) + 0x06,
	PD_7  = (PortD << 4) + 0x07,
	PD_8  = (PortD << 4) + 0x08,
	PD_9  = (PortD << 4) + 0x09,
	PD_10 = (PortD << 4) + 0x0A,
	PD_11 = (PortD << 4) + 0x0B,
	PD_12 = (PortD << 4) + 0x0C,
	PD_13 = (PortD << 4) + 0x0D,
	PD_14 = (PortD << 4) + 0x0E,
	PD_15 = (PortD << 4) + 0x0F,
#endif /* GPIOD_BASE */
#ifdef GPIOE_BASE
	PE_0  = (PortE << 4) + 0x00,
	PE_1  = (PortE << 4) + 0x01,
	PE_2  = (PortE << 4) + 0x02,
	PE_3  = (PortE << 4) + 0x03,
	PE_4  = (PortE << 4) + 0x04,
	PE_5  = (PortE << 4) + 0x05,
	PE_6  = (PortE << 4) + 0x06,
	PE_7  = (PortE << 4) + 0x07,
	PE_8  = (PortE << 4) + 0x08,
	PE_9  = (PortE << 4) + 0x09,
	PE_10 = (PortE << 4) + 0x0A,
	PE_11 = (PortE << 4) + 0x0B,
	PE_12 = (PortE << 4) + 0x0C,
	PE_13 = (PortE << 4) + 0x0D,
	PE_14 = (PortE << 4) + 0x0E,
	PE_15 = (PortE << 4) + 0x0F,
#endif /* GPIOE_BASE */
#ifdef GPIOF_BASE
	PF_0  = (PortF << 4) + 0x00,
	PF_1  = (PortF << 4) + 0x01,
	PF_2  = (PortF << 4) + 0x02,
	PF_3  = (PortF << 4) + 0x03,
	PF_4  = (PortF << 4) + 0x04,
	PF_5  = (PortF << 4) + 0x05,
	PF_6  = (PortF << 4) + 0x06,
	PF_7  = (PortF << 4) + 0x07,
	PF_8  = (PortF << 4) + 0x08,
	PF_9  = (PortF << 4) + 0x09,
	PF_10 = (PortF << 4) + 0x0A,
	PF_11 = (PortF << 4) + 0x0B,
	PF_12 = (PortF << 4) + 0x0C,
	PF_13 = (PortF << 4) + 0x0D,
	PF_14 = (PortF << 4) + 0x0E,
	PF_15 = (PortF << 4) + 0x0F,
#endif /* GPIOF_BASE */
#ifdef GPIOG_BASE
	PG_0  = (PortG << 4) + 0x00,
	PG_1  = (PortG << 4) + 0x01,
	PG_2  = (PortG << 4) + 0x02,
	PG_3  = (PortG << 4) + 0x03,
	PG_4  = (PortG << 4) + 0x04,
	PG_5  = (PortG << 4) + 0x05,
	PG_6  = (PortG << 4) + 0x06,
	PG_7  = (PortG << 4) + 0x07,
	PG_8  = (PortG << 4) + 0x08,
	PG_9  = (PortG << 4) + 0x09,
	PG_10 = (PortG << 4) + 0x0A,
	PG_11 = (PortG << 4) + 0x0B,
	PG_12 = (PortG << 4) + 0x0C,
	PG_13 = (PortG << 4) + 0x0D,
	PG_14 = (PortG << 4) + 0x0E,
	PG_15 = (PortG << 4) + 0x0F,
#endif /* GPIOG_BASE */
#ifdef GPIOH_BASE
	PH_0  = (PortH << 4) + 0x00,
	PH_1  = (PortH << 4) + 0x01,
	PH_2  = (PortH << 4) + 0x02,
	PH_3  = (PortH << 4) + 0x03,
	PH_4  = (PortH << 4) + 0x04,
	PH_5  = (PortH << 4) + 0x05,
	PH_6  = (PortH << 4) + 0x06,
	PH_7  = (PortH << 4) + 0x07,
	PH_8  = (PortH << 4) + 0x08,
	PH_9  = (PortH << 4) + 0x09,
	PH_10 = (PortH << 4) + 0x0A,
	PH_11 = (PortH << 4) + 0x0B,
	PH_12 = (PortH << 4) + 0x0C,
	PH_13 = (PortH << 4) + 0x0D,
	PH_14 = (PortH << 4) + 0x0E,
	PH_15 = (PortH << 4) + 0x0F,
#endif /* GPIOH_BASE */
#ifdef GPIOI_BASE
	PI_0  = (PortI << 4) + 0x00,
	PI_1  = (PortI << 4) + 0x01,
	PI_2  = (PortI << 4) + 0x02,
	PI_3  = (PortI << 4) + 0x03,
	PI_4  = (PortI << 4) + 0x04,
	PI_5  = (PortI << 4) + 0x05,
	PI_6  = (PortI << 4) + 0x06,
	PI_7  = (PortI << 4) + 0x07,
	PI_8  = (PortI << 4) + 0x08,
	PI_9  = (PortI << 4) + 0x09,
	PI_10 = (PortI << 4) + 0x0A,
	PI_11 = (PortI << 4) + 0x0B,
	PI_12 = (PortI << 4) + 0x0C,
	PI_13 = (PortI << 4) + 0x0D,
	PI_14 = (PortI << 4) + 0x0E,
	PI_15 = (PortI << 4) + 0x0F,
#endif /* GPIOI_BASE */
#ifdef GPIOJ_BASE
	PJ_0  = (PortJ << 4) + 0x00,
	PJ_1  = (PortJ << 4) + 0x01,
	PJ_2  = (PortJ << 4) + 0x02,
	PJ_3  = (PortJ << 4) + 0x03,
	PJ_4  = (PortJ << 4) + 0x04,
	PJ_5  = (PortJ << 4) + 0x05,
	PJ_6  = (PortJ << 4) + 0x06,
	PJ_7  = (PortJ << 4) + 0x07,
	PJ_8  = (PortJ << 4) + 0x08,
	PJ_9  = (PortJ << 4) + 0x09,
	PJ_10 = (PortJ << 4) + 0x0A,
	PJ_11 = (PortJ << 4) + 0x0B,
	PJ_12 = (PortJ << 4) + 0x0C,
	PJ_13 = (PortJ << 4) + 0x0D,
	PJ_14 = (PortJ << 4) + 0x0E,
	PJ_15 = (PortJ << 4) + 0x0F,
#endif /* GPIOJ_BASE */
#ifdef GPIOK_BASE
	PK_0  = (PortK << 4) + 0x00,
	PK_1  = (PortK << 4) + 0x01,
	PK_2  = (PortK << 4) + 0x02,
	PK_3  = (PortK << 4) + 0x03,
	PK_4  = (PortK << 4) + 0x04,
	PK_5  = (PortK << 4) + 0x05,
	PK_6  = (PortK << 4) + 0x06,
	PK_7  = (PortK << 4) + 0x07,
	PK_8  = (PortK << 4) + 0x08,
	PK_9  = (PortK << 4) + 0x09,
	PK_10 = (PortK << 4) + 0x0A,
	PK_11 = (PortK << 4) + 0x0B,
	PK_12 = (PortK << 4) + 0x0C,
	PK_13 = (PortK << 4) + 0x0D,
	PK_14 = (PortK << 4) + 0x0E,
	PK_15 = (PortK << 4) + 0x0F,
#endif /* GPIOK_BASE */
#ifdef GPIOZ_BASE
	PZ_0  = (PortZ << 4) + 0x00,
	PZ_1  = (PortZ << 4) + 0x01,
	PZ_2  = (PortZ << 4) + 0x02,
	PZ_3  = (PortZ << 4) + 0x03,
	PZ_4  = (PortZ << 4) + 0x04,
	PZ_5  = (PortZ << 4) + 0x05,
	PZ_6  = (PortZ << 4) + 0x06,
	PZ_7  = (PortZ << 4) + 0x07,
	PZ_8  = (PortZ << 4) + 0x08,
	PZ_9  = (PortZ << 4) + 0x09,
	PZ_10 = (PortZ << 4) + 0x0A,
	PZ_11 = (PortZ << 4) + 0x0B,
	PZ_12 = (PortZ << 4) + 0x0C,
	PZ_13 = (PortZ << 4) + 0x0D,
	PZ_14 = (PortZ << 4) + 0x0E,
	PZ_15 = (PortZ << 4) + 0x0F,
#endif /* GPIOZ_BASE */
#ifdef ADC_CHANNEL_TEMPSENSOR
	PINTERN_TEMPSENSOR = (PortIntern << 4) + 0x00,
#endif /* ADC_CHANNEL_TEMPSENSOR */
#ifdef ADC_CHANNEL_TEMPSENSOR_ADC1
	PINTERN_TEMPSENSOR_ADC1 = (PortIntern << 4) + 0x01,
#endif /* ADC_CHANNEL_TEMPSENSOR_ADC1 */
#ifdef ADC_CHANNEL_TEMPSENSOR_ADC5
	PINTERN_TEMPSENSOR_ADC5 = (PortIntern << 4) + 0x02,
#endif /* ADC_CHANNEL_TEMPSENSOR_ADC5 */
#ifdef ADC_CHANNEL_VBAT
	PINTERN_VBAT = (PortIntern << 4) + 0x03,
#endif /* ADC_CHANNEL_VBAT */
#ifdef ADC_CHANNEL_VCOMP
	PINTERN_VCOMP = (PortIntern << 4) + 0x04,
#endif /* ADC_CHANNEL_VCOMP */
#ifdef ADC_CHANNEL_VCORE
	PINTERN_VCORE = (PortIntern << 4) + 0x05,
#endif /* ADC_CHANNEL_VCORE */
#ifdef ADC_CHANNEL_VLCD
	PINTERN_VLCD = (PortIntern << 4) + 0x06,
#endif /* ADC_CHANNEL_VLCD */
#ifdef ADC_CHANNEL_VREFINT
	PINTERN_VREFINT = (PortIntern << 4) + 0x07,
#endif /* ADC_CHANNEL_VREFINT */
	PinCount
};


/**
 * STM32 products have only one reference for ADC.
 * This is kept only for compatibility with existing AVR based API.
 */
enum AnalogReference {
	AR_DEFAULT
};


/* https://stackoverflow.com/a/50189151/2525536 */
#undef UNUSED
#define UNUSED(x) ((void)((uint32_t)(x)))


#ifdef __has_include
#if __has_include(<pins_arduino.h>)
#include <pins_arduino.h>
#endif /* __has_include(<pins_arduino.h>) */
#endif /* __has_include */


#endif /* __ARDUINO_DEFINES_H__ */
