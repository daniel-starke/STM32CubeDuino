#!/bin/gawk -f
# @file _timerParse.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-11
# @version 2020-11-14
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643\db\mcu
# for i in STM32*.xml; do ./_timerParse.awk $i | sort; done >_timer.txt

BEGIN {
	Pin = ""
	Gpio = ""
}

/IP ConfigFile="GPIO-/ {
	if ( match($0, /Version="([^"]+)"/, ary) ) {
		Gpio = ary[1]
	}
}

/Pin Name/ {
	Pin = ""
	if ( match($2, /Name="(P[A-Z][0-9]+)/, ary) ) {
		Pin = ary[1]
	}
}

/TIM[0-9]+_/ {
	if (Pin != "" && !match($2, /_BKIN/) && !match($2, /_ETR/)) {
		if ( match($2, /Name="([^"]+)"\/>/, ary) ) {
			file = FILENAME
			gsub(/\.xml$/, "", file)
			printf("%s\t%s\t%s\t%s\n", file, Pin, ary[1], Gpio)
		}
	}
}
