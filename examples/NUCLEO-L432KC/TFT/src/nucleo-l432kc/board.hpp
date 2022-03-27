/**
 * @file board.hpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-10-01
 * @version 2022-03-17
 */
#ifndef __NUCLEO_L432KC_HPP__
#define __NUCLEO_L432KC_HPP__

#include <stm32l4xx.h>
#include <stm32l4xx_hal.h>
#include <stm32l4xx_ll_cortex.h>
#include <stm32l4xx_ll_exti.h>
#include <stm32l4xx_ll_gpio.h>
#include <stm32l4xx_ll_system.h>
#include <stm32l4xx_ll_tim.h>


#ifndef __STM32L432xx_H
#error Missing include of stm32l432xx.h. Please define STM32L432xx.
#endif


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


/* pin aliases (these are the same for all NUCLEO-32 boards)
 * @see https://www.st.com/resource/en/user_manual/dm00231744-stm32-nucleo32-boards-mb1180-stmicroelectronics.pdf#page=29
 */
#define D0 PA_10 /* SCL for Wire */
#define D1 PA_9 /* SDA for Wire */
#define D2 PA_12 /* USB DP */
#define D3 PB_0
#define D4 PB_7
#define D5 PB_6
#define D6 PB_1
#define D7 PC_14 /* shared with OSC32_IN */
#define D8 PC_15 /* shared with OSC32_OUT */
#define D9 PA_8
#define D10 PA_11 /* USB DM */
#define D11 PB_5 /* no PWM */
#define D12 PB_4
#define D13 PB_3
#define A0 PA_0
#define A1 PA_1
#define A2 PA_3
#define A3 PA_4
#define A4 PA_5 /* only floating input (for ADC) unless solder bridges SB16 and SB18 were removed */
#define A5 PA_6 /* only floating input (for ADC) unless solder bridges SB16 and SB18 were removed */
#define A6 PA_7
#define A7 PA_2 /* exclusive with VCP_TX */
#define LED_BUILTIN PB_3
/* button B1 is connected to NRST */


#endif /* __NUCLEO_L432KC_HPP__ */
