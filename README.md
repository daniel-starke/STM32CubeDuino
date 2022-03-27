STM32CubeDuino
==============

[![STM32Cube Framework Compatibility Tests](https://github.com/daniel-starke/STM32CubeDuino/actions/workflows/compatibility-tests.yml/badge.svg?branch=main)](https://github.com/daniel-starke/STM32CubeDuino/actions/workflows/compatibility-tests.yml)

Port of the Arduino API based on the STM32 Cube framework.

This library aims to provide high compatibility to the original Arduino AVR framework, or at least  
Arduino SAMD framework, while maintaining a simple, stable and lightweight implementation.  
Issues of the Arduino API (not its implementation) are adopted were applicable for highest compatibility.  
To ensure easy adaption between existing and custom STM32 based boards, STM32 Cube framework  
is used as backend API. Middleware layers are avoided to reduce function overhead. Only the Cube  
Low Layer and Hardware Abstraction Layer is used together with the CMSIS API.

Please note that this only provides the Arduino API, not its whole environment. Hence, there is  
no special pre-processor pass to handle .ino files. You need to write C/C++ compliant code.

Also note that this is not a modified version of STM32duino or Mbed. The used implementation approach
is fundamentally different, to provide better support for custom boards.

This project is not affiliated with Arduino AG, STMicroelectronics International NV, or any of its affiliates. 

Getting Started
===============

A getting started guide can be found [here](doc/starting.md).  
Some small examples are also included [here](examples).

Done
====

- [x] Custom board.ini Creation Script
- [x] Basic Arduino API
- [x] Time
- [x] Digital I/O
- [x] WInterrupt
- [x] WMath
- [x] WCharacter
- [x] WString
- [x] Printable
- [x] Print
- [x] Stream
- [x] HardwareSerial
- [x] binary.h
- [x] serialEventRun()
- [ ] ADC (still missing support for SDADC)
- [x] DAC (analogWrite())
- [x] PWM (analogWrite())
- [x] HardwareTimer
- [x] tone()/noTone()
- [x] pulseIn()/pulseInLong()
- [x] shiftOut()/shiftIn()
- [x] SPI (untested)
- [x] I2C
- [ ] [EEPROM](https://www.st.com/resource/en/application_note/dm00311483-eeprom-emulation-techniques-and-software-for-stm32-microcontrollers-stmicroelectronics.pdf)
- [x] Pluggable USB
- [x] USB CDC
- [ ] USB HID
- [ ] CAN
- [x] Version.h
- [ ] boot to system bootloader via USB CDC 1200 Baud touch
- [ ] SoftwareSerial
- [ ] LiquidCrystal
- [ ] USBHost
- [x] Documentation
- [x] License
- [x] keywords.txt
- [x] Example Board
- [x] disabling macros
- [ ] add support for STM32G0 (new pull request to make `RCC_GetSystemClockFreq()` public)
- [ ] add support for STM32MP1 (needs [PlatformIO support](https://github.com/platformio/platform-ststm32/issues/439))
- [ ] add support for STM32U5 (needs [PlatformIO support](https://github.com/platformio/platform-ststm32/issues/439))
- [ ] add support for STM32WL (needs [PlatformIO support](https://github.com/platformio/platform-ststm32/issues/439))
- [ ] add support for STM32WB (needs [PlatformIO support](https://github.com/platformio/platform-ststm32/issues/439))
- [x] GitHub Release

Special Macros
==============

|Name                                 |Meaning
|-------------------------------------|---------------------------------------------------------------
|`ARDUINO`                            |Defines the compatible Arduino version.
|`ARDUINO_API_VERSION`                |Defines the compatible Arduino API version.
|`ARDUINO_ARCH_STM32`                 |Defines the Arduino target architecture.
|`STM32CUBEDUINO`                     |Defines the version of the STM32CubeDuino library as string.
|`STM32CUBEDUINO`                     |Defines the version of the STM32CubeDuino library as string.
|`STM32CUBEDUINO_MAJOR`               |Defines the major version number of the STM32CubeDuino library.
|`STM32CUBEDUINO_MINOR`               |Defines the minor version number of the STM32CubeDuino library.
|`STM32CUBEDUINO_PATCH`               |Defines the patch version number of the STM32CubeDuino library.
|`STM32CUBEDUINO_VERSION(x, y, z)`    |Transforms the given major, minor and patch version number into a single number for comparison.
|`STM32CUBEDUINO_VERSION_CODE`        |Defines the version of the STM32CubeDuino library as single number for comparison.
|`STM32CUBEDUINO_SMALL_FLASH`         |May be defined by the user to trade off speed for small flash usage.
|`STM32CUBEDUINO_FLOAT_STRING`        |May be defined by the user to implement only single precision float to string conversion functions.
|`STM32CUBEDUINO_LEGACY_API`          |May be defined by the user to remove non-Arduino functions for better compatibility.
|`STM32CUBEDUINO_INTERNALS`           |May be defined by the user to get access to some internal functions. These are not automatically exported. The user needs to declare a function prototype to access these.
|`STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS` |May be defined by the user to define custom STM32 HAL IRQ handlers. All STM32CubeDuino specific handlers will be disabled.
|`STM32CUBEDUINO_MAP_ALL_IRQS`        |May be defined by the user to unconditionally map all STM32 HAL IRQ handlers to STM32CubeDino specific ones.
|`STM32CUBEDUINO_DISABLE_ADC`         |May be defined by the user to disable ADC related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_CDC`         |May be defined by the user to disable USB serial (CDC) related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_DAC`         |May be defined by the user to disable DAC related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_EXTI`        |May be defined by the user to disable external interrupt related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_I2C`         |May be defined by the user to disable I2C related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_PWM`         |May be defined by the user to disable PWM related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_SERIAL`      |May be defined by the user to disable hardware serial related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_SPI`         |May be defined by the user to disable SPI related STM32CubeDino functions.
|`STM32CUBEDUINO_DISABLE_TIMER`       |May be defined by the user to disable hardware timer related STM32CubeDino functions (not PWM).
|`STM32CUBEDUINO_DISABLE_USB`         |May be defined by the user to disable USB related STM32CubeDino functions.
|`NO_GPL`                             |May be defined by the user to exclude GPL licensed code. This affects only support functions included for better Arduino AVR compatibility.
|`SERIAL_RX_BUFFER_SIZE`              |May be defined by the user to change the serial reception buffer size. Defaults to 64 bytes.
|`SERIAL_TX_BUFFER_SIZE`              |May be defined by the user to change the serial transmission buffer size. Defaults to 64 bytes.
|`TWOWIRE_RX_BUFFER_SIZE`             |May be defined by the user to change the I2C reception buffer size. Defaults to 32 bytes.
|`TWOWIRE_TX_BUFFER_SIZE`             |May be defined by the user to change the I2C transmission buffer size. Defaults to 32 bytes.
|`SPI_TRANSFER_TIMEOUT`               |May be defined by the user to change the SPI timeout in milliseconds. Set to HAL_MAX_DELAY for no timeout. Defaults to 1000ms.
|`ACTIVATE_USB_PORT`                  |May be defined by the user with custom logic to force a USB enumeration, e.g. by pulling down D+.
|`USB_EP_SIZE`                        |May be defined by the user to change the USB single endpoint size. Defaults to 32 or 64 bytes depending on the target platform.
|`USB_PRODUCT`                        |May be defined by the user to change the USB product name. Defaults to `"USB IO Board"`.
|`USB_MANUFACTURER`                   |May be defined by the user to change the USB manufacturer name. Defaults to `"STMicroelectronics"` depending on `USB_VID`.
|`I_CACHE_DISABLED`                   |May be defined by the user to disable instruction cache.
|`D_CACHE_DISABLED`                   |May be defined by the user to disable data cache.
|`HAVE_HWSERIAL0`                     |Defined if the hardware serial API is available.
|`HAVE_CDCSERIAL`                     |Defined if the CDC (serial USB) API is available.
|`USBCON`                             |Defined if the USB API is available.
|`USB_ENDPOINTS`                      |Defined with the number of available USB endpoints.
|`EXTI_IRQ_PRIO`                      |Needs to be defined in `board.hpp` to set the priority for external interrupts.
|`EXTI_IRQ_SUBPRIO`                   |Needs to be defined in `board.hpp` to set the sub-priority for external interrupts.
|`I2C_IRQ_PRIO`                       |Needs to be defined in `board.hpp` to set the priority for I2C interrupts.
|`I2C_IRQ_SUBPRIO`                    |Needs to be defined in `board.hpp` to set the sub-priority for I2C interrupts.
|`SYSTICK_IRQ_PRIO`                   |Needs to be defined in `board.hpp` to set the priority for SYSTICK interrupts.
|`SYSTICK_IRQ_SUBPRIO`                |Needs to be defined in `board.hpp` to set the sub-priority for SYSTICK interrupts.
|`TIMER_IRQ_PRIO`                     |Needs to be defined in `board.hpp` to set the priority for TIMER interrupts.
|`TIMER_IRQ_SUBPRIO`                  |Needs to be defined in `board.hpp` to set the sub-priority for TIMER interrupts.
|`UART_IRQ_PRIO`                      |Needs to be defined in `board.hpp` to set the priority for UART/USART interrupts.
|`UART_IRQ_SUBPRIO`                   |Needs to be defined in `board.hpp` to set the sub-priority for UART/USART interrupts.
|`USB_IRQ_PRIO`                       |Needs to be defined in `board.hpp` to set the priority for USB interrupts.
|`USB_IRQ_SUBPRIO`                    |Needs to be defined in `board.hpp` to set the sub-priority for USB interrupts.
|`USB_VID`                            |Needs to be defined in `board.hpp` to set the USB vendor ID. E.g. `0x2341` for Arduino LLC.
|`USB_PID`                            |Needs to be defined in `board.hpp` to set the USB product ID. E.g. `0x8036` for Leonardo (CDC ACM, HID).

Implementation Notes
====================

* All code is written for C++14 and C18.
* The used DAC/ADC/Timer (PWM) instance for each pin is the one with the lowest instance number available for that pin by default.
* Only USB FS and no USB HS or ULPI is supported for chips with USB peripheral
* Proper registration of the USB VID and PID is need to publish a USB product. See [here](https://www.usb.org/getting-vendor-id), [here](https://pid.codes/howto/) and [here](https://community.st.com/s/question/0D50X00009XkgcCSAR/has-anyone-managed-to-sublicence-a-usb-pid-from-st).
* `setAltFunction()` may be overwritten by the user to allow a different alternate function number to remap mapping for STM32F1.
* `setAdcFromPin()` may be overwritten by the user to define a different pin to ADC mapping.
* `_TimerPinMap::_TimerPinMap(const uint32_t pin)` may be overwritten by the user to define a different pin to timer mapping.
* Timer0 may be overwritten by the user to use a different timer for internal functions. The other timers (e.g. Timer1) are not defined by default.
* DAC pin mapping cannot be changed as it is the same for all targets and requires no changes.
* STM32CubeDuinoIrqHandlerForXXX() functions may be overwritten by the user to handle interrupt of the corresponding STM32 HAL instance XXX.
* getIrqNumFor(XXX) returns the IRQ number of the given STM32 HAL instance XXX, whereas XXX matches only pre-defined names like TIM1.
* clearNvicIrqFor(XXX) clears the pending interrupt bit for the given STM32 HAL instance XXX, whereas XXX matches only pre-defined names like TIM1.
* There can be only one interrupt callback function attached per pin, regardless of the port.
* Additional `int` overloads for functions and methods have been omitted.
* `__HAL_RCC_SYSCFG_CLK_ENABLE()` and `__HAL_RCC_PWR_CLK_ENABLE()` are called before `main()`. USB, `HardwareSerial`, `_TimerPinMap::f1PinModeTimer()` and `enableGpioClock()` require that these clocks are enabled. Keep that in mind when during these clocks off.

Hardware Design Hints
=====================

* Try to configure your clocks and pinout in STM32CubeIDE before starting the hardware design, to find design flews due to operation restrictions in an early development stage.
* Check the chip specific datasheet carefully for the recommended power setup. Missing backup/filtering/decoupling capacities can have a strong influence on the operation stability.
* All STM32 chips have a correct USB line termination resistor for the correct impedance included (see AN4879 Q&A section).
* Some STM32 chips require an external 1.5kΩ pull-up for USB D+ to operate correctly.
* One 5.1kΩ pull-down on CC1 and one on CC2 can be applied to use a USB-C connector in USB2.0 mode.
* The needed values for the passive components for an external crystal can be calculated via [this calculator](https://helios.wh2.tu-dresden.de/~benni_koch/stmcalculator/) in accordance to AN2867.
  * The capacitors are mainly needed to stimulate the crystal and may work well with different values as well.
  * The resistor value can be less then given but should not be higher.
* See AN2606 for details regarding the possible boot configurations and programming modes (e.g. not every chip can be programmed via USB).
* The edge detection period for external interrupts may be shorter than a clock period. Make sure to add sufficient electrical filtering.

FAQ
====

**Q:** Can I use STM32CubeDuino with the Arduino IDE?  
**A:** No. This would require some code changes in the Arduino IDE. Therefore, support is currently only given for PlatformIO.

**Q:** Writing to HardwareSerial from within an interrupt handler gives me broken data. What can I do?  
**A:** This is usually the case if the UART/USART/LPUART IRQ handler has a priority lower or equal to the one in question. Assign a higher priority (lower value) to UART or increase the buffer size for HardwareSerial.

**Q:** Can I use compiler xyz instead of GCC?  
**A:** Most likely not. The code was written with the assumption that GCC is used to simplify code design in several places.

**Q:** How can I simply reduce the binary size?  
**A:** Define the macros `STM32CUBEDUINO_SMALL_FLASH` and `STM32CUBEDUINO_FLOAT_STRING` in the board definition or via build flag. This trades off execution speed for flash size. Additionally, the build flag `-ffast-math` can improve code size. Also try to disable unused periphery by defining `STM32CUBEDUINO_DISABLE_USB` for example.

**Q:** How do I generate a custom `initVariant()` function for my board?  
**A:** Download the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html). Create a new `STM32 Project` for the used chip in there and activate all relevant peripherals. Complete the clock configuration and generate the code. The content from the generated `SystemClock_Config()` function in `main.c` is the base for the new `initVariant()` function. Finally, replace all calls to `Error_Handler()` with `systemErrorHandler()`. See also [Getting Started](doc/starting.md).

**Q:** Passing `-flto` to the compiler and linker breaks my build. The firmware is not starting anymore.
**A:** The is most likely due to an old bug in [binutils](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83967). Try to update to a [more recent toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm).

Issues
======

- periphery locking is not implemented for STM32MP1
- handling of analog control register ASC0 is missing in pinModeEx()
- `__HAL_LOCK()` is basically [broken](https://community.st.com/s/question/0D50X00009XkeOGSAZ/questions-surrounding-hallock) but [may be fixed](https://community.st.com/s/question/0D50X0000C5Tns8SQC/bug-stm32-hal-driver-lock-mechanism-is-not-interrupt-safe) in your version.
- PWM output setup creates spikes (maybe due to `TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE); /* Enable the Capture compare channel */`)

License
=======

See [LICENSE](LICENSE).  

Contributions
=============

No content contributions are accepted. Please file a bug report or feature request instead.  
This decision was made in consideration of the used license.
