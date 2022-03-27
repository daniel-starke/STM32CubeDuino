#!/bin/gawk -f
# @file _adcCollect.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-03
# @version 2020-11-14
#
# ./_adcCollect.awk _adc.txt > _adcTable.txt

function initializeMap() {
	delete Map
	for (adc = 0; adc < 11; adc++) {
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				Map[ADC[adc] "_" Port[port] pin] = ""
			}
		}
	}
}

function printMap() {
	for (adc = 0; adc < 11; adc++) {
		hasAdc = 0
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				if (Map[ADC[adc] "_" Port[port] pin] != "") hasAdc = 1
			}
		}
		if (hasAdc == 0) continue
		printf("%s\t%s\t", CurModel, ADC[adc])
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("%s", Map[ADC[adc] "_" Port[port] pin] != "" ? "1" : "0")
			}
		}
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("\t%s", Map[ADC[adc] "_" Port[port] pin])
			}
		}
		printf("\n")
	}
	initializeMap()
}

BEGIN {
	CurModel = ""
	ADC[0] = "ADC"
	ADC[1] = "ADC1"
	ADC[2] = "ADC2"
	ADC[3] = "ADC3"
	ADC[4] = "ADC4"
	ADC[5] = "ADC5"
	ADC[6] = "SDADC1"
	ADC[7] = "SDADC2"
	ADC[8] = "SDADC3"
	ADC[9] = "SDADC4"
	ADC[10] = "SDADC5"
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
	if ( match($3, /((SD)?ADC[0-9]*_)/, ary) ) {
		Map[ary[1] $2] = $3
	}
}

END {
	printMap()
}