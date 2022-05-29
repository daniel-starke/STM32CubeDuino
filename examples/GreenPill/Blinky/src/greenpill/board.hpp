/**
 * @file board.hpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-05-29
 * @version 2022-05-29
 */
#ifndef __GREENPILL_HPP__
#define __GREENPILL_HPP__

#include <stdint.h>
#include <stm32l0xx.h>
#include <stm32l0xx_hal.h>
#include <stm32l0xx_ll_cortex.h>
#include <stm32l0xx_ll_exti.h>
#include <stm32l0xx_ll_gpio.h>
#include <stm32l0xx_ll_system.h>
#include <stm32l0xx_ll_tim.h>


#ifndef __STM32L053xx_H
#error Missing include of stm32l053xx.h. Please define STM32L053xx.
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


/* pin aliases */
/* may be PA_0 for some variants */
#define LED_BUILTIN PA_1


#endif /* __GREENPILL_HPP__ */
