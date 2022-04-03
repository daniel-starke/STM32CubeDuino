#!/bin/sh
# @file manual-test.sh
# @author Daniel Starke
# @copyright Copyright 2022 Daniel Starke
# @date 2022-03-29
# @version 2022-03-29
#
# Run from within STM32CubeDuino directory.

Error() {
	echo "Error: $@"
	exit 1
}

rm -rf test || Error "Failed to remove temporary files."
while read SERIES ARCH MCU
do
	export SERIES ARCH MCU
	cp -pr etc/test . || Error "Failed to copy test template directory."
	chmod ugo+x test/build.sh || Error "Failed to set test script permissions."
	test/build.sh || Error "Test failed for SERIES=${SERIES} ARCH=${ARCH} MCU=${MCU}."
	rm -rf test || Error "Failed to remove temporary files."
done <<_LIST
f0 cortex-m0 stm32f072vbt6
f1 cortex-m3 stm32f103c8t6
f2 cortex-m3 stm32f207zet6
f3 cortex-m4 stm32f303cct6
f4 cortex-m4 stm32f401ccu6
f7 cortex-m7 stm32f730r8t6
g0 cortex-m0plus stm32g030c8t6
g4 cortex-m4 stm32g473cbt6
h7 cortex-m7 stm32h743vit6
l0 cortex-m0plus stm32l053r8t6
l1 cortex-m3 stm32l152rbt6
l4 cortex-m4 stm32l432kcu6
l5 cortex-m33 stm32l552cct6
_LIST
