#!/bin/gawk -f
# @file _timerCollect.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-11
# @version 2020-11-14
#
# GPIO_FILE=_gpio.txt ./_timerCollect.awk _timer.txt > _timerTable.txt

function quote(s) {
	q = "'"
	gsub(q, q "\\" q q, s)
	return q s q
}

function initializeMap() {
	delete Map
	for (timer = 0; timer < 29; timer++) {
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				Map[TIMER[timer] "_" Port[port] pin] = ""
			}
		}
	}
}

function printMap() {
	for (timer = 0; timer < 29; timer++) {
		hasTimer = 0
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				if (Map[TIMER[timer] "_" Port[port] pin] != "") hasTimer = 1
			}
		}
		if (hasTimer == 0) continue
		printf("%s\t%s\t", CurModel, TIMER[timer])
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("%s", Map[TIMER[timer] "_" Port[port] pin] != "" ? "1" : "0")
			}
		}
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("\t%s", Map[TIMER[timer] "_" Port[port] pin])
			}
		}
		printf("\n")
	}
	initializeMap()
}

BEGIN {
	CurModel = ""
	TIMER[ 0] = "TIM1"
	TIMER[ 1] = "TIM2"
	TIMER[ 2] = "TIM3"
	TIMER[ 3] = "TIM4"
	TIMER[ 4] = "TIM5"
	TIMER[ 5] = "TIM6"
	TIMER[ 6] = "TIM7"
	TIMER[ 7] = "TIM8"
	TIMER[ 8] = "TIM9"
	TIMER[ 9] = "TIM10"
	TIMER[10] = "TIM11"
	TIMER[11] = "TIM12"
	TIMER[12] = "TIM13"
	TIMER[13] = "TIM14"
	TIMER[14] = "TIM15"
	TIMER[15] = "TIM16"
	TIMER[16] = "TIM17"
	TIMER[17] = "TIM18"
	TIMER[18] = "TIM19"
	TIMER[19] = "TIM20"
	TIMER[20] = "TIM21"
	TIMER[21] = "TIM22"
	TIMER[22] = "TIM23"
	TIMER[23] = "TIM24"
	TIMER[24] = "LPTIM1"
	TIMER[25] = "LPTIM2"
	TIMER[26] = "LPTIM3"
	TIMER[27] = "LPTIM4"
	TIMER[28] = "LPTIM5"
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
	if ( ! "GPIO_FILE" in ENVIRON ) {
		printf("Error: Missing environment variable GPIO_FILE.\n") > "/dev/stderr"
		exit 1
	}
	delete AfMap
	while ((getline < ENVIRON["GPIO_FILE"]) > 0) {
		if ( match($3, /^(LPTIM[0-9]+)_OUT|(TIM[0-9]+)_CH[0-9]+N?/, ary) ) {
			instance = ary[1] != "" ? ary[1] : ary[2]
			AfMap[instance "_" $2 "_" $1] = $4
		}
	}
	if (length(AfMap) < 1) {
		printf("Error: File in GPIO_FILE does not exists.\n") > "/dev/stderr"
		exit 1
	}
	close(ENVIRON["GPIO_FILE"])
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
	if ( match($3, /^(LPTIM[0-9]+)_OUT|(TIM[0-9]+)_CH[0-9]+N?/, ary) ) {
		instance = ary[1] != "" ? ary[1] : ary[2]
		if (AfMap[instance "_" $2 "_" $4] == "" && match($1, /^STM32F1/)) {
			Map[instance "_" $2] = $3 "/default"
		} else if (AfMap[instance "_" $2 "_" $4] != "") {
			Map[instance "_" $2] = $3 "/" AfMap[instance "_" $2 "_" $4]
		} else {
			printf("Warning: Alternate function for signal %s on pin %s is missing for target %s (see IP/GPIO-%s_Modes.xml).\n", $3, $2, $1, $4) > "/dev/stderr"
		}
	}
}

END {
	printMap()
}