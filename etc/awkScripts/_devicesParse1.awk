#!/bin/gawk -f
# @file _devicesParse1.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-15
# @version 2020-11-14
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643
# unzip -p STM32CubeMX.jar "devices/*" -x devices/stm32boards.db | ./_devicesParse.awk >_devices.txt

BEGIN {
	delete DList
	delete TDef
}

/<device>/ {
	Products = ""
	Variants = ""
	Define = ""
}

/<PN>/ {
	delete ary
	if ( match($0, /<PN>([^<]+)/, ary) ) Products = ary[1]
}

/<variants>/ {
	delete ary
	if ( match($0, /<variants>([^<]+)/, ary) ) Variants = ary[1]
}

/<define>/ {
	delete ary
	if ( match($0, /<define>([^<]+)/, ary) ) Define = ary[1]
}

/<\/device>/ {
	if (Products != "" && Define != "") {
		DList[Define] = Define
		delete ary
		Count = split(Products, ary, ",")
		for (i = 1; i <= Count; i++) {
			x1 = ary[i] "\\|"
			x2 = ary[i] "$"
			if (TDef[Define] ~ x1 || TDef[Define] ~ x2) continue # already in list
			if (TDef[Define] != "") TDef[Define] = TDef[Define] "|"
			TDef[Define] = TDef[Define] ary[i]
		}
	}
}

END {
	Count = asort(DList)
	for (i = 1; i <= Count; i++) {
		Define = DList[i]
		gsub(/x/, ".", TDef[Define])
		printf("%s	^(%s)\n", Define, TDef[Define])
	}
}