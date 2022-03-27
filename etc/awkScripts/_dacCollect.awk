#!/bin/gawk -f
# @file _dacCollect.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-05
# @version 2020-11-14
#
# ./_dacCollect.awk _dac.txt > _dacTable.txt

function initializeMap() {
	delete Map
	for (dac = 0; dac < 6; dac++) {
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				Map[DAC[dac] "_" Port[port] pin] = ""
			}
		}
	}
}

function printMap() {
	for (dac = 0; dac < 6; dac++) {
		hasDac = 0
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				if (Map[DAC[dac] "_" Port[port] pin] != "") hasDac = 1
			}
		}
		if (hasDac == 0) continue
		printf("%s\t%s\t", CurModel, DAC[dac])
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("%s", Map[DAC[dac] "_" Port[port] pin] != "" ? "1" : "0")
			}
		}
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("\t%s", Map[DAC[dac] "_" Port[port] pin])
			}
		}
		printf("\n")
	}
	initializeMap()
}

BEGIN {
	CurModel = ""
	DAC[0] = "DAC"
	DAC[1] = "DAC1"
	DAC[2] = "DAC2"
	DAC[3] = "DAC3"
	DAC[4] = "DAC4"
	DAC[5] = "DAC5"
	Port[ 0] = "PA"
	Port[ 1] = "PB"
	Port[ 2] = "PC"
	Port[ 3] = "PD"
	Port[ 4] = "PE"
	Port[ 5] = "PF"
	Port[ 6] = "PG"
	Port[ 7] = "PH"
	Port[ 8] = "PI"
	Port[ 9] = "PJ"
	Port[10] = "PK"
	Port[11] = "PZ"
	printf("Model\tInstance\tBitmap")
	for (port = 0; port < 12; port++) {
		for (pin = 0; pin < 16; pin++) {
			printf("\t%s", Port[port] pin)
		}
	}
	printf("\n")
}

/STM32/ {
	if (CurModel == "") {
		CurModel = $1
	} else if (CurModel != $1) {
		printMap()
		CurModel = $1
	}
	if ( match($3, /(DAC[0-9]*_)/, ary) ) {
		Map[ary[1] $2] = $3
	}
}

END {
	printMap()
}