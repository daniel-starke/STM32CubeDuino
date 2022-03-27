#!/bin/gawk -f
# @file _gpioParse.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-12
# @version 2020-11-14
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643\db\mcu
# for i in IP/GPIO-*.xml; do ./_gpioParse.awk $i; done | sort >_gpio.txt

BEGIN {
	Version = ""
	Pin = ""
	PinSignal = ""
}

/Name="GPIO" Version=/ {
	Version = ""
	if ( match($0, /Name="GPIO" Version="([^"]*)"/, ary) ) {
		Version = ary[1]
	}
}

/GPIO_Pin PortName/ {
	Pin = ""
	if ( match($3, /Name="([^"_/(-]+)/, ary) ) {
		Pin = ary[1]
	}
}

/PinSignal/ {
	PinSignal = ""
	if ( match($2, /Name="([^"]+)"/, ary) ) {
		PinSignal = ary[1]
	}
}

/PossibleValue/ {
	if (Version != "" && Pin != "" && PinSignal != "") {
		if ( match($1, />GPIO_AF([0-9]+)_.*</, ary) ) {
			printf("%s\t%s\t%s\t%s\n", Version, Pin, PinSignal, ary[1])
			PinSignal = ""
		} else if ( match($1, />(.*_REMAP_.*)</, ary) ) {
			printf("%s\t%s\t%s\t%s\n", Version, Pin, PinSignal, ary[1])
			PinSignal = ""
		}
	}
}

/<\/GPIO_Pin>/ {
	Pin = ""
}
