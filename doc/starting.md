# Getting Started

This introduction describes how to create a new custom board and simply blinky using the
[BluePill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html).

## Hardware

The board consists of:
* a user LED (PC13)
* a reset button
* a pin header to select the boot mode
* a pin header for debugging via [ST-LINK/V2](https://www.st.com/en/development-tools/st-link-v2.html)
* a USB micro connector
* 32 user available GPIOs
* the [STM32F103C8T6](https://www.st.com/en/microcontrollers-microprocessors/stm32f103c8.html) Cortex-M3 MCU with 20KiB RAM, 64KiB flash and up to 72MHz speed

The board is power via USB and can also be programmed via USB using the system boot loader if `BOOT0` is set 1.
`BOOT1` shall be 0 for both programming and user application. Alternatively, keep `BOOT0` set to 0 for user
application and program it with ST-LINK.

## 1/3) PlatformIO Setup

Create a new folder for the PlatformIO project with the following folders within it:
* boards
* src
* src/bluepill

Use the provided `BuildScript.py` to run:
``` shell
python BuildScript.py cortex-m3 stm32f103c8t6 >boards/bluepill.json
```

Set the board name to `bluepill` in `boards/bluepill.json`.

Create `platformio.ini` with the following content:
``` ini
[platformio]
workspace_dir = bin
src_dir = src

[common]
build_flags = -Wall -Wextra -Wformat -pedantic -Wshadow -Wconversion -Wparentheses -Wunused -Wno-missing-field-initializers

[env:bluepill]
platform = ststm32
platform_packages = toolchain-gccarmnoneeabi@1.90201.191206
framework = stm32cube
lib_deps = https://github.com/daniel-starke/STM32CubeDuino/archive/refs/heads/main.zip
board = bluepill
build_flags = -fno-strict-aliasing -I${PROJECTSRC_DIR}/bluepill -DNO_GPL
src_build_flags = ${common.build_flags}
debug_tool = stlink
```

Note the board `bluepill` here, which was just added before.
Also note that the additional include direction via `build_flags` is needed to load the board specific header correctly from within the STM32CubeDuino library.

## 2/3) Board Code

A `board.cpp` and `board.hpp` is needed within `src/bluepill` in order to initialize the STM32CubeDuino for this specific board correctly.  
`board.cpp` contains the periphery instantiations for this board and the clock source setup, which is defined `initVariant()`.  
`board.hpp` on the other hand needs to include all relevant STM32 HAL header from the STM32Cube framework, interrupt handler priority settings and optional pin name aliases.

### `board.cpp`

``` c++
#include <Arduino.h>
#include <wiring_irq.h>


/* exported variables */
HardwareSerial Serial1(USART1, getIrqNumFor(USART1), PA_10, PA_9, 1, 1);


/**
 * Initializes this board by configuring the system clock base.
 */
void initVariant() {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
	
	/* Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure. */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		systemErrorHandler();
	}
	/* Initializes the CPU, AHB and APB buses clocks. */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		systemErrorHandler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		systemErrorHandler();
	}
}
```

This is a minimal example only with a hardware serial interface and 72MHz clock from external 8MHz quartz crystal resonator.
The content of `initVariant()` was generated with [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) by performing the following steps:
* create a new STM32 project for STM32F103C8 for C++
* in "System Core/RCC" set "High Speed Clock" to "Crystal/Ceramic resonator"
* in "Connectivity/USART1" set "Mode" to "Asynchronous"
* in "Connectivity/USB" enable "Device (FS)"
* switch to the tab "Clock Configuration"
* set "System Clock Mux" to "PLLCLK" and "HCLK" to 72MHz
* save the changes and generate the code
* `Core/Src/main.c` contains the function `SystemClock_Config()`
* copy the content of `SystemClock_Config()` to `initVariant()` in `board.cpp` and replace `Error_Handler()` with `systemErrorHandler()`
* prettify the code (optional)

Usually, the alternate function numbers needed for the USART pins can be obtained from the [datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf).
However, the STM32F1 series implements a complex remapping system. Look up the function `setAltFunction()` within `Arduino.cpp` to obtain the correct number.
Or implement a custom `setAltFunction()` to change the meaning of the alternate function numbers. Use the following C++ function signature:
``` c++
void setAltFunction(GPIO_TypeDef * gpioPort, const uint32_t llPin, const uint32_t altFn)
```
Note, however, that a custom implementation may break code of other STM32CubeDuino functions.

### `board.hpp`

``` c++
#ifndef __BLUEPILL_HPP__
#define __BLUEPILL_HPP__

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
```

This includes the STM32 HAL and LL driver headers. It also implements a custom logic to pull D+ low
during USB activation to force a USB enumeration even if a wrong pull-up resistor was used on the
D+ pin of the BluePill. Furthermore, priorities for the USB, UART, external GPIO, timer and I2C
interrupts are defined. A macro is included to set an alias for the built-in LED pin.

Note that the shown header includes in this example represent a bare minimum for STM32CubeDuino.

## 3/3) Blinky

Finally, add a `src/main.cpp` file for the blinky example:
``` c++
#include <Arduino.h>


void setup() {
	Serial1.begin(115200);
	delay(100);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial1.println("begin");
}


void loop() {
	static bool value = false;
	Serial1.println(value ? "LOW" : "HIGH");
	digitalWrite(LED_BUILTIN, uint32_t(value));
	value = !value;
	delay(500);
}
```

Compile and flash everything with:
``` shell
pio run -t upload
```

Note that the serial output would be written to USB if `Serial1` was changed to `Serial`.
