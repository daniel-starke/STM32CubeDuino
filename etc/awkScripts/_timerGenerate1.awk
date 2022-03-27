#!/bin/gawk -f
# @file _timerGenerate1.awk
# @author Daniel Starke
# @copyright Copyright 2020-2022 Daniel Starke
# @date 2020-10-18
# @version 2022-02-08
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643\db\mcu
# DEVICE_FILE=../../_devices.txt ./_timerGenerate1.awk _timerTable.txt >_timerTable.cpp

function appendTargetDefine(list, item) {
	listItemCheck = "defined\\(" item "\\)"
	if (list ~ listItemCheck) return list
	if (item in DefAmbiguous) {
		DefAmbiguous[item]++
	} else {
		DefAmbiguous[item] = 1
	}
	if (list != "") {
		return list " || defined(" item ")"
	}
	return "defined(" item ")"
}

# add target defines from the given target to the passed list
function getTargetDefine(list, target) {
	delete _ary
	_target = target
	if ( match(target, /(.*)\((.+)\)(.*)/, _ary) ) {
		# multi-target given
		delete _ary2
		_count = split(_ary[2], _ary2, "-")
		_found = 0
		for (_n = 1; _n <= _count; _n++) {
			# handle each single target
			_targetN = _ary[1] _ary2[_n] _ary[3]
			for (_tdef in TDef) {
				if (_targetN ~ TDef[_tdef]) {
					_found = 1
					list = appendTargetDefine(list, _tdef)
				}
			}
		}
		if (_found == 0) {
			# unmatched multi-target
			list = appendTargetDefine(list, "<error: " _target ">")
			printf("Error: Missing target definition for target %s.\n", _target) > "/dev/stderr"
		}
		return list
	} else {
		# single target given
		for (_tdef in TDef) {
			if (target ~ TDef[_tdef]) {
				return appendTargetDefine(list, _tdef)
			}
		}
	}
	# unmatched target
	printf("Error: Missing target definition for target %s.\n", _target) > "/dev/stderr"
	return appendTargetDefine(list, "<error: " _target ">")
}

function outputCaseStatement(instNum, chanNum, invNum, afNum, pinName, signalName) {
	printf("\n	case %s:%s res = _TimerPinMap{%2i, %i, %i, %2i}.value; break; /* %s */", pinName, length(pinName) > 4 ? "" : " ", instNum, chanNum, invNum, afNum, signalName)
}

function outputList() {
	firstIf = 1
	for (i = 0; i < length(ComTargets); i++) {
		firstInTarget = 1
		lastPort = 0
		endIfPort = 0
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				pinName = Port[port] pin
				idx = i "_" pinName
				if (Map[idx] == "") continue
				if (firstInTarget == 1) {
					delete ary
					TCount = split(ComTargets[i], ary, ",")
					TDefList = ""
					for (TN = 1; TN <= TCount; TN++) {
						TDefList = getTargetDefine(TDefList, ary[TN])
					}
					if (firstIf == 1) {
						printf("\n#if %s", TDefList)
						firstIf = 0
					} else {
						printf("\n#elif %s", TDefList)
					}
					firstInTarget = 0
				}
				if (lastPort > 0 && port != lastPort) {
					if (lastPort > 1) {
						printf("\n#endif /* GPIO%c_BASE */", substr(Port[lastPort], 2, 1))
					}
					printf("\n#ifdef GPIO%c_BASE", substr(Port[port], 2, 1))
				}
				lastPort = port
				delete ary
				delete pp
				if ( ! match(Map[idx], /((LP)?TIM([0-9]+)_CH([0-9]+)(N)?)\/(.+)/, ary) ) {
					delete ary2
					match(Map[idx], /(LPTIM([0-9]+)_OUT)\/(.+)/, ary2)
					ary[1] = ary2[1]
					ary[2] = "LP"
					ary[3] = ary2[2]
					ary[4] = "1"
					ary[5] = ""
					ary[6] = ary2[3]
				}
				match(pinName, /(P[A-Z])([0-9]+)/, pp)
				off = ary[2] != "" ? 24 : 0
				inv = ary[5] != "" ? 1 : 0
				if (ary[6] == "default") {
					af = 0
					afStr = ary[1] "/default"
				} else {
					af = Remap[ary[6]] != "" ? Remap[ary[6]] : ary[6] 
					afStr = Remap[ary[6]] != "" ? Map[idx] : ary[1] "/GPIO_AF" ary[6]
				}
				outputCaseStatement(ary[3] + off, ary[4] - 1, inv, af, pp[1] "_" pp[2], afStr)
			}
		}
		if (lastPort > 1) {
			printf("\n#endif /* GPIO%c_BASE */", substr(Port[lastPort], 2, 1))
		}
	}
	if (firstInTarget == 0) {
		printf("\n#else")
		printf("\n#warning There is no timer pin mapping defined for this target.")
		printf("\n#endif")
	}
}

BEGIN {
	FS = "\t"
	# list of possible ports
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
	# remap function to number mapping (reverted in _f1PinModeTimer())
	Remap["__HAL_AFIO_REMAP_TIM1_ENABLE"] = "1"
	Remap["__HAL_AFIO_REMAP_TIM1_PARTIAL"] = "2"
	Remap["__HAL_AFIO_REMAP_TIM2_PARTIAL_1"] = "3"
	Remap["__HAL_AFIO_REMAP_TIM2_PARTIAL_2"] = "4"
	Remap["__HAL_AFIO_REMAP_TIM3_ENABLE"] = "5"
	Remap["__HAL_AFIO_REMAP_TIM3_PARTIAL"] = "6"
	Remap["__HAL_AFIO_REMAP_TIM4_ENABLE"] = "7"
	Remap["__HAL_AFIO_REMAP_TIM9_ENABLE"] = "8"
	Remap["__HAL_AFIO_REMAP_TIM10_ENABLE"] = "9"
	Remap["__HAL_AFIO_REMAP_TIM11_ENABLE"] = "10"
	Remap["__HAL_AFIO_REMAP_TIM12_ENABLE"] = "11"
	Remap["__HAL_AFIO_REMAP_TIM13_ENABLE"] = "12"
	Remap["__HAL_AFIO_REMAP_TIM14_ENABLE"] = "13"
	Remap["__HAL_AFIO_REMAP_TIM15_ENABLE"] = "14"
	Remap["__HAL_AFIO_REMAP_TIM16_ENABLE"] = "15"
	Remap["__HAL_AFIO_REMAP_TIM17_ENABLE"] = "16"
	# list of target defines (see _devicesParse1.awk)
	if ( ! "DEVICE_FILE" in ENVIRON ) {
		printf("Error: Missing environment variable DEVICE_FILE.\n") > "/dev/stderr"
		exit 1
	}
	delete TDef
	while ((getline < ENVIRON["DEVICE_FILE"]) > 0) {
		TDef[$1] = $2
	}
	if (length(TDef) < 1) {
		printf("Error: File in DEVICE_FILE does not exists.\n") > "/dev/stderr"
		exit 1
	}
	
	# get unique list of target platforms
	delete Targets
	first = 1
	while ((getline < ARGV[1]) > 0) {
		if (first == 1) {
			first = 0
			continue
		}
		Targets[$1] = $1
	}
	close(ARGV[1])
	TargetCount = asort(Targets)
	
	# collect pin mapping
	delete ComTargets
	delete Map
	for (TargetN = 1; TargetN <= TargetCount; TargetN++) {
		delete TmpMap
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				TmpMap[Port[port] pin] = ""
			}
		}
		target = Targets[TargetN]
		first = 1
		while ((getline < ARGV[1]) > 0) {
			if ($1 != target) {
				if (first != 1) break # early out
				continue # skip
			}
			for (port = 0; port < 12; port++) {
				for (pin = 0; pin < 16; pin++) {
					field = $((port * 16) + pin + 4)
					if (field == "") continue
					idx = Port[port] pin
					if (TmpMap[idx] == "") TmpMap[idx] = field
				}
			}
		}
		close(ARGV[1])
		# check if the current target is compatible with any previous target
		matchingIndex = -1
		for (i = 0; i < length(ComTargets); i++) {
			mismatch = 0
			for (port = 0; port < 12 && mismatch == 0; port++) {
				for (pin = 0; pin < 16; pin++) {
					pinName = Port[port] pin
					idx = i "_" pinName
					if (Map[idx] != "" && TmpMap[pinName] != "" && Map[idx] != TmpMap[pinName]) {
						mismatch = 1
						break
					}
				}
			}
			if (mismatch == 0) {
				matchingIndex = i
				break
			}
		}
		if (matchingIndex < 0 || TargetN == TargetCount) {
			# this target is incompatible with a previous ones -> add new map entry
			newEntry = length(ComTargets)
			ComTargets[newEntry] = target
			for (port = 0; port < 12; port++) {
				for (pin = 0; pin < 16; pin++) {
					pinName = Port[port] pin
					idx = newEntry "_" pinName
					Map[idx] = TmpMap[pinName]
				}
			}
		} else {
			# this target was compatible with the previous ones -> add new pins
			ComTargets[matchingIndex] = ComTargets[matchingIndex] "," target
			for (port = 0; port < 12; port++) {
				for (pin = 0; pin < 16; pin++) {
					pinName = Port[port] pin
					idx = matchingIndex "_" pinName
					if (Map[idx] == "" && TmpMap[pinName] != "") {
						Map[idx] = TmpMap[pinName]
					}
				}
			}
		}
	}
	
	# output function header
	printf("__attribute__((weak))\n")
	printf("_TimerPinMap::_TimerPinMap(const uint32_t pin) {\n")
	printf("	/* This enables the compiler to create a compact mapping table. */\n")
	printf("	uint16_t res = 0;\n")
	printf("	switch (pin) {")
	
	# output function body
	delete DefAmbiguous
	outputList()
	
	# output function footer
	printf("\n	default: break;\n")
	printf("	}\n")
	printf("	this->value = res;\n")
	printf("}\n")
	
	# check if target defines are distinctly used
	delete DefAmbIdx;
	for (def in DefAmbiguous) {
		DefAmbIdx[length(DefAmbIdx)] = def
	}
	Count = asort(DefAmbIdx)
	for (i = 1; i <= Count; i++) {
		def = DefAmbIdx[i]
		if (DefAmbiguous[def] > 1) {
			printf("Warning: Target define %s is used for multiple configurations (%i).\n", def, DefAmbiguous[def]) > "/dev/stderr"
		}
	}
	
	# end of script
	exit 0
}