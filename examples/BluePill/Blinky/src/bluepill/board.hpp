/**
 * @file board.hpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-17
 * @version 2022-03-17
 */
#ifndef __BLUEPILL_HPP__
#define __BLUEPILL_HPP__

#include <stdint.h>
#include <stm32f1xx.h>
#include <stm32f1xx_hal.h>
#include <stm32f1xx_ll_cortex.h>
#include <stm32f1xx_ll_exti.h>
#include <stm32f1xx_ll_gpio.h>
#include <stm32f1xx_ll_system.h>
#include <stm32f1xx_ll_tim.h>


#ifndef __STM32F103xB_H
#error Missing include of stm32f103xb.h. Please define STM32F103xB.
#endif


/* force USB device re-enumeration by pulling down D+ for 5ms */
#define ACTIVATE_USB_PORT() do { \
		pinMode(PA_12, OUTPUT); \
		digitalWrite(PA_12, LOW); \
		delay(5); \
	} while (false)


#define USB_IRQ_PRIO 0
#define USB_IRQ_SUBPRIO 0

#define UART_IRQ_PRIO 1
#define UART_IRQ_SUBPRIO 0

#define EXTI_IRQ_PRIO 3
#define EXTI_IRQ_SUBPRIO 0

#define TIMER_IRQ_PRIO 4
#define TIMER_IRQ_SUBPRIO 0

#define I2C_IRQ_PRIO 5
#define I2C_IRQ_SUBPRIO 0


/* pin aliases */
#define LED_BUILTIN PC_13


#endif /* __BLUEPILL_HPP__ */
