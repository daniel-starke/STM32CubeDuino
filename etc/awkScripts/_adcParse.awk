#!/bin/gawk -f
# @file _adcParse.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-03
# @version 2020-11-14
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643\db\mcu
# for i in STM32*.xml; do ./_adcParse.awk $i | sort; done >_adc.txt

BEGIN {
	Pin = ""
}

/Pin Name/ {
	Pin = ""
	if ( match($2, /Name="(P[A-Z][0-9]+)/, ary) ) {
		Pin = ary[1]
	}
}

/ADC/ {
	if (Pin != "" && !match($2, /EXTI/) && !match($2, /_INN/) && !match($2, /IN[0-9]*M/)) {
		if ( match($2, /Name="([^"]+)"\/>/, ary) ) {
			file = FILENAME
			gsub(/\.xml$/, "", file)
			printf("%s\t%s\t%s\n", file, Pin, ary[1])
		}
	}
}
