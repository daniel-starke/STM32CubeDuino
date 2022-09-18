#!/bin/sh
# @file build.sh
# @author Daniel Starke
# @copyright Copyright 2022 Daniel Starke
# @date 2022-03-20
# @version 2022-09-17
#
# Running from within the library root directory.
# The following environment variables are used:
# SERIES - MCU series (e.g. l4)
# ARCH   - MCU architecture (e.g. cortex-m4)
# MCU    - MCU name (e.g. stm32l432kcu6)
#
# Optionally, set SIMPLE=1 to build only the trivial case.

Error() {
	echo "Error: $@"
	exit 1
}

[ "x" != "x${SERIES}" ] || Error "Missing environment variable SERIES."
[ "x" != "x${ARCH}" ] || Error "Missing environment variable ARCH."
[ "x" != "x${MCU}" ] || Error "Missing environment variable MCU."

# Update series in board header.
sed "s/??/$SERIES/g" etc/test/src/board/board.hpp > test/src/board/board.hpp || Error "Failed to fill template board.hpp with \"${SERIES}\"."

# Create test_board.json.
python BuildScript.py "${ARCH}" "${MCU}" "test_board" > test/boards/test_board.json || Error "Failed to build test_board.json for ${MCU} (${ARCH})."

cd test || Error "Failed to change directory to \"test\"."

# Build only trivial case.
if [ "x1" = "x${SIMPLE}" ]; then
	pio run || Error "Failed to build blinky for ${MCU} (${ARCH})."
	exit 0
fi

# Build with different settings.
while read DEF
do
	sed "s/-DNO_GPL/-DNO_GPL ${DEF}/g" ../etc/test/platformio.ini > platformio.ini || Error "Failed to add build flag ${DEF} to platformio.ini."
	pio run || Error "Failed to build blinky for ${MCU} (${ARCH}) with ${DEF}."
done <<"_LIST"

-UNO_GPL
-DSTM32CUBEDUINO_SMALL_FLASH
-DSTM32CUBEDUINO_FLOAT_STRING
-DSTM32CUBEDUINO_LEGACY_API
-DSTM32CUBEDUINO_INTERNALS
-DSTM32CUBEDUINO_CUSTOM_IRQ_HANDLERS
-DSTM32CUBEDUINO_MAP_ALL_IRQS
-DSTM32CUBEDUINO_DISABLE_ADC
-DSTM32CUBEDUINO_DISABLE_CDC
-DSTM32CUBEDUINO_DISABLE_DAC
-DSTM32CUBEDUINO_DISABLE_EXTI
-DSTM32CUBEDUINO_DISABLE_I2C
-DSTM32CUBEDUINO_DISABLE_PWM
-DSTM32CUBEDUINO_DISABLE_PRINTF_FLOAT
-DSTM32CUBEDUINO_DISABLE_SCANF_FLOAT
-DSTM32CUBEDUINO_DISABLE_SERIAL
-DSTM32CUBEDUINO_DISABLE_SPI
-DSTM32CUBEDUINO_DISABLE_STRING
-DSTM32CUBEDUINO_DISABLE_TIMER
-DSTM32CUBEDUINO_DISABLE_USB
-DSTM32CUBEDUINO_DISABLE_USB_CDC
_LIST

exit 0
