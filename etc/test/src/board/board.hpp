/**
 * @file board.hpp
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-20
 * @version 2022-03-25
 * 
 * Test template.
 */
#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <stdint.h>
#include <stm32??xx.h>
#include <stm32??xx_hal.h>
#include <stm32??xx_ll_cortex.h>
#include <stm32??xx_ll_exti.h>
#include <stm32??xx_ll_gpio.h>
#include <stm32??xx_ll_system.h>
#include <stm32??xx_ll_tim.h>


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


#endif /* __BOARD_HPP__ */
