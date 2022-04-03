#!/bin/gawk -f
# @file _irqGenerate.awk
# @author Daniel Starke
# @copyright Copyright 2020-2022 Daniel Starke
# @date 2020-11-14
# @version 2022-04-03
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643\db\mcu\IP
# grep -hr _IRQn NVIC* | sort -u | CXX_OUT="wiring_irq.cpp" H_OUT="wiring_irq.h" ./_irqGenerate.awk
# requires a gawk version with custom comparator support in asort()

function basename(file) {
	sub(".*/", "", file)
	return file
}

function toInt(str) {
	return str + 0
}

function cmpNatural(i1, v1, i2, v2) {
	delete _s1
	delete _s2
	split(v1, _s1, /[0-9]+/, _S1)
	split(v2, _s2, /[0-9]+/, _S2)
	_n1 = length(_s1) + length(_S1)
	_n2 = length(_s2) + length(_S2)
	_nMax = (_n1 > _n2) ? _n1 : _n2
	_I = 0
	for (_i = 1; (_i + _I) <= _nMax;) {
		if (_i > _I) {
			# ensure that longer sub-strings come first
			_s1[_i] = sprintf("%s\xFF", _s1[_i])
			_s2[_i] = sprintf("%s\xFF", _s2[_i])
			# compare strings
			if (_s1[_i] < _s2[_i]) {
				return -1
			} else if (_s1[_i] > _s2[_i]) {
				return 1
			}
			_I++
		} else {
			# compare numbers
			_NUM1 = toInt(_S1[_I])
			_NUM2 = toInt(_S2[_I])
			if (_NUM1 < _NUM2) {
				return -1
			} else if (_NUM1 > _NUM2) {
				return 1
			}
			_i++
		}
	}
	return 0
}

function outputCondIfDef(insts, output) {
	_list = ","
	for (_k in usedIrqMap) {
		if (insts ~ "," _k "[0-9]*," && _list !~ "," usedIrqMap[_k] ",") {
			_list = _list usedIrqMap[_k] ","
		}
	}
	_disableList = ","
	for (_k in disableIrqMap) {
		if (insts ~ "," _k "[0-9]*," && _disableList !~ "," disableIrqMap[_k] ",") {
			_disableList = _disableList disableIrqMap[_k] ","
		}
	}
	if (_list == ",") {
		if (_disableList == ",") {
			printf("#ifdef STM32CUBEDUINO_MAP_ALL_IRQS\n") >> output
		} else {
			delete _disableListAry
			_disableCount = split(_disableList, _disableListAry, ",")
			printf("#if defined(STM32CUBEDUINO_MAP_ALL_IRQS) || (") >> output
			for (_j = 2; _j < _disableCount; _j++) {
				if (_j != 2) printf(" && ") >> output
				printf("!defined(%s)", _disableListAry[_j]) >> output
			}
			printf(")\n") >> output
		}
	} else {
		delete _listAry
		_count = split(_list, _listAry, ",")
		printf("#if defined(STM32CUBEDUINO_MAP_ALL_IRQS)") >> output
		for (_i = 2; _i < _count; _i++) {
			printf(" || (", _listAry[_i]) >> output
			if (_disableList != ",") {
				delete _disableListAry
				_disableCount = split(_disableList, _disableListAry, ",")
				for (_j = 2; _j < _disableCount; _j++) {
					printf("!defined(%s) && ", _disableListAry[_j]) >> output
				}
			}
			printf("defined(%s))", _listAry[_i]) >> output
		}
		printf("\n") >> output
	}
}

function outputCondEndIf(insts, output) {
	_list = ","
	for (_k in usedIrqMap) {
		if (insts ~ "," _k "[0-9]*," && _list !~ "," usedIrqMap[_k] ",") {
			_list = _list usedIrqMap[_k] ","
		}
	}
	delete _listAry
	_count = split(_list, _listAry, ",")
	printf("#endif /* STM32CUBEDUINO_MAP_ALL_IRQS") >> output
	for (_i = 2; _i < _count; _i++) {
		printf(" or %s", _listAry[_i]) >> output
	}
	printf(" */\n") >> output
}


BEGIN {
	if ( ! "CXX_OUT" in ENVIRON ) {
		printf("Error: Missing environment variable CXX_OUT.\n") > "/dev/stderr"
		exit 1
	}
	if ( ! "H_OUT" in ENVIRON ) {
		printf("Error: Missing environment variable H_OUT.\n") > "/dev/stderr"
		exit 1
	}
	C = ENVIRON["CXX_OUT"]
	H = ENVIRON["H_OUT"]
	# valAry indices
	IDX_IRQ = 1
	IDX_MASKABLE = 2
	IDX_INSTANCES = 3
	IDX_HANDLERS = 4
	IDX_CONDITION = 5
	IDX_DMA_INST = 4
	IDX_DMA_CHAN = 5
	# create output files
	printf("") > C
	printf("") > H
	# initialize maps/lists
	# instance to interrupt line map
	itLineMap["ADC"] = "ADC"
	itLineMap["AES"] = "AES"
	itLineMap["CAN"] = "CAN"
	itLineMap["CEC"] = "CEC"
	itLineMap["CLK_CTRL"] = "CLK_CTRL"
	itLineMap["COMP1"] = "COMP1"
	itLineMap["COMP2"] = "COMP2"
	itLineMap["COMP3"] = "COMP3"
	itLineMap["CRS"] = "CRS"
	itLineMap["DAC"] = "DAC"
	itLineMap["DMA"] = "DMAMUX1"
	itLineMap["DMA1_CH1"] = "DMA1_CH1"
	itLineMap["DMA1_CH2"] = "DMA1_CH2"
	itLineMap["DMA1_CH3"] = "DMA1_CH3"
	itLineMap["DMA1_CH4"] = "DMA1_CH4"
	itLineMap["DMA1_CH5"] = "DMA1_CH5"
	itLineMap["DMA1_CH6"] = "DMA1_CH6"
	itLineMap["DMA1_CH7"] = "DMA1_CH7"
	itLineMap["DMA2_CH1"] = "DMA2_CH1"
	itLineMap["DMA2_CH2"] = "DMA2_CH2"
	itLineMap["DMA2_CH3"] = "DMA2_CH3"
	itLineMap["DMA2_CH4"] = "DMA2_CH4"
	itLineMap["DMA2_CH5"] = "DMA2_CH5"
	itLineMap["EXTI0"] = "EXTI0"
	itLineMap["EXTI1"] = "EXTI1"
	itLineMap["EXTI2"] = "EXTI2"
	itLineMap["EXTI3"] = "EXTI3"
	itLineMap["EXTI4"] = "EXTI4"
	itLineMap["EXTI5"] = "EXTI5"
	itLineMap["EXTI6"] = "EXTI6"
	itLineMap["EXTI7"] = "EXTI7"
	itLineMap["EXTI8"] = "EXTI8"
	itLineMap["EXTI9"] = "EXTI9"
	itLineMap["EXTI10"] = "EXTI10"
	itLineMap["EXTI11"] = "EXTI11"
	itLineMap["EXTI12"] = "EXTI12"
	itLineMap["EXTI13"] = "EXTI13"
	itLineMap["EXTI14"] = "EXTI14"
	itLineMap["EXTI15"] = "EXTI15"
	itLineMap["FDCAN1"] = "FDCAN1_IT0,FDCAN1_IT1"
	itLineMap["FDCAN2"] = "FDCAN2_IT0,FDCAN2_IT1"
	itLineMap["FLASH"] = "FLASH_ECC,FLASH_ITF"
	itLineMap["I2C1"] = "I2C1"
	itLineMap["I2C2"] = "I2C2"
	itLineMap["I2C3"] = "I2C3"
	itLineMap["LPTIM1"] = "LPTIM1"
	itLineMap["LPTIM2"] = "LPTIM2"
	itLineMap["LPUART1"] = "LPUART1"
	itLineMap["LPUART2"] = "LPUART2"
	itLineMap["PWR"] = "PVDOUT,PVMOUT,VDDIO2"
	itLineMap["RNG"] = "RNG"
	itLineMap["RTC"] = "RTC,RTC_ALRA,RTC_TSTAMP,RTC_WAKEUP"
	itLineMap["SPI1"] = "SPI1"
	itLineMap["SPI2"] = "SPI2"
	itLineMap["SPI3"] = "SPI3"
	itLineMap["TAMPER"] = "TAMPER"
	itLineMap["TIM1"] = "TIM1_BRK,TIM1_CC,TIM1_CCU,TIM1_TRG,TIM1_UPD"
	itLineMap["TIM2"] = "TIM2"
	itLineMap["TIM3"] = "TIM3"
	itLineMap["TIM4"] = "TIM4"
	itLineMap["TIM6"] = "TIM6"
	itLineMap["TIM7"] = "TIM7"
	itLineMap["TIM14"] = "TIM14"
	itLineMap["TIM15"] = "TIM15"
	itLineMap["TIM16"] = "TIM16"
	itLineMap["TIM17"] = "TIM17"
	itLineMap["TSC"] = "TSC_EOA,TSC_MCE"
	itLineMap["UCPD1"] = "UCPD1"
	itLineMap["UCPD2"] = "UCPD2"
	itLineMap["USART1"] = "USART1"
	itLineMap["USART2"] = "USART2"
	itLineMap["USART3"] = "USART3"
	itLineMap["USART4"] = "USART4"
	itLineMap["USART5"] = "USART5"
	itLineMap["USART6"] = "USART6"
	itLineMap["USART7"] = "USART7"
	itLineMap["USART8"] = "USART8"
	itLineMap["USB"] = "USB"
	itLineMap["WWDG"] = "EWDG,WWDG"
	for (i in itLineMap) {
		instList[i] = i
	}
	ignTim1Map["TIM1_BRK_IRQn"] = "TIM1_BRK_IRQn"
	ignTim1Map["TIM1_BRK_TIM9_IRQn"] = "TIM1_BRK_TIM9_IRQn"
	ignTim1Map["TIM1_BRK_TIM15_IRQn"] = "TIM1_BRK_TIM15_IRQn"
	ignTim1Map["TIM1_CC_IRQn"] = "TIM1_CC_IRQn"
	ignTim1Map["TIM1_TRG_COM_IRQn"] = "TIM1_TRG_COM_IRQn"
	ignTim1Map["TIM1_TRG_COM_TIM11_IRQn"] = "TIM1_TRG_COM_TIM11_IRQn"
	ignTim1Map["TIM1_TRG_COM_TIM17_IRQn"] = "TIM1_TRG_COM_TIM17_IRQn"
	# conditional include map
	usedIrqMap["ADC"] = "ADC_SOFTWARE_START"
	usedIrqMap["DAC"] = "DAC_TRIGGER_NONE"
	usedIrqMap["I2C"] = "IS_I2C_ADDRESSING_MODE"
	usedIrqMap["LPTIM"] = "LPTIM_PRESCALER_DIV1"
	usedIrqMap["LPUART"] = "IS_UART_MODE"
	usedIrqMap["TIM"] = "TIM_CLOCKPRESCALER_DIV1"
	usedIrqMap["UART"] = "IS_UART_MODE"
	usedIrqMap["USART"] = "IS_UART_MODE"
	usedIrqMap["USB"] = "PCD_PHY_EMBEDDED"
	usedIrqMap["USB_OTG_FS"] = "PCD_PHY_EMBEDDED"
	# disabling macros
	disableIrqMap["ADC"] = "STM32CUBEDUINO_DISABLE_ADC"
	disableIrqMap["DAC"] = "STM32CUBEDUINO_DISABLE_DAC"
	disableIrqMap["EXTI"] = "STM32CUBEDUINO_DISABLE_EXTI"
	disableIrqMap["I2C"] = "STM32CUBEDUINO_DISABLE_I2C"
	disableIrqMap["LPTIM"] = "STM32CUBEDUINO_ENABLE_TIMER"
	disableIrqMap["LPUART"] = "STM32CUBEDUINO_DISABLE_SERIAL"
	disableIrqMap["TIM"] = "STM32CUBEDUINO_DISABLE_ALL_TIMERS"
	disableIrqMap["UART"] = "STM32CUBEDUINO_DISABLE_SERIAL"
	disableIrqMap["USART"] = "STM32CUBEDUINO_DISABLE_SERIAL"
	disableIrqMap["USB"] = "STM32CUBEDUINO_DISABLE_USB"
	disableIrqMap["USB_OTG_FS"] = "STM32CUBEDUINO_DISABLE_USB"
}

/PossibleValue/ {
	delete valAry
	delete cmtAry
	if (match($0, /Value="([^"]+)"/, valAry) && match($0, /Comment="([^"]*)"/, cmtAry)) {
		delete spVal
		delete spInsts
		delete spDma
		delete spCh
		split(valAry[1], spVal, ":")
		if (spVal[IDX_INSTANCES] ~ /DMA/ && spVal[IDX_HANDLERS] !~ /IRQHandler/) {
			dmaCount = split(spVal[IDX_DMA_INST], spDma, ",")
			chCount = split(spVal[IDX_DMA_CHAN], spCh, ";")
			spCount = 0
			# convert DMA instance and channels to DMA<inst>#<chan> list
			for (i = 1; i <= dmaCount; i++) {
				delete idxAry
				match(spCh[i], /([0-9]+)(,([0-9]+))?/, idxAry)
				if (idxAry[3] != "") {
					# handle channel range from to
					for (r = toInt(idxAry[1]); r <= toInt(idxAry[3]); r++) {
						spInsts[++spCount] = spDma[i] "_CH" r
					}
				} else {
					# handle single channel
					spInsts[++spCount] = spDma[i] "_CH" idxAry[1]
				}
			}
		} else {
			spCount = split(spVal[IDX_INSTANCES], spInsts, ",")
			if (spVal[IDX_IRQ] ~ /FLASH/ && spVal[IDX_INSTANCES] !~ /FLASH/) {
				spInsts[++spCount] = "FLASH"
			}
		}
		irqList[spVal[IDX_IRQ]] = spVal[IDX_IRQ]
		for (i = 1; i <= spCount; i++) {
			instList[spInsts[i]] = spInsts[i]
			if (irqMap[spVal[IDX_IRQ]] == "") {
				irqMap[spVal[IDX_IRQ]] = "," spInsts[i] ","
			} else if ( ! match(irqMap[spVal[IDX_IRQ]], "," spInsts[i] ",") ) {
				irqMap[spVal[IDX_IRQ]] = irqMap[spVal[IDX_IRQ]] spInsts[i] ","
			}
			if (spVal[IDX_IRQ] in cmtMap) {
				if (cmtMap[spVal[IDX_IRQ]] != cmtAry[1]) {
					printf("Warning: Found multiple different instances of %s.\n", spVal[IDX_IRQ]) > "/dev/stderr"
					printf("Previous: %s\n", cmtMap[spVal[IDX_IRQ]]) > "/dev/stderr"
					printf("New:      %s\n", cmtAry[1]) > "/dev/stderr"
					if (length(cmtAry[1]) > length(cmtMap[spVal[IDX_IRQ]])) {
						cmtMap[spVal[IDX_IRQ]] = cmtAry[1]
					}
				}
			} else {
				cmtMap[spVal[IDX_IRQ]] = cmtAry[1]
			}
		}
	}
}

END {
	instListCount = asort(instList, instList, "cmpNatural")
	irqListCount = asort(irqList, irqList, "cmpNatural")
	itLineListCount = asorti(itLineMap, itLineList, "cmpNatural")
	
	# output header
	printf("/**\n") >> H
	printf(" * @file %s\n", basename(H)) >> H
	printf(" * @author Daniel Starke\n") >> H
	printf(" * @copyright Copyright 2020-2022 Daniel Starke\n") >> H
	printf(" * @date 2020-11-13\n") >> H
	printf(" * @version %s\n", strftime("%Y-%m-%d")) >> H
	printf(" * \n") >> H
	printf(" * @internal For internal use only.\n") >> H
	printf(" */\n") >> H
	printf("#ifndef __WIRING_IRQ_H__\n") >> H
	printf("#define __WIRING_IRQ_H__\n") >> H
	printf("\n") >> H
	printf("\n") >> H
	printf("#if defined(STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS) && defined(STM32CUBEDUINO_MAP_ALL_IRQS)\n") >> H
	printf("#error STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS and STM32CUBEDUINO_MAP_ALL_IRQS cannot be defined together.\n") >> H
	printf("#endif /* STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS and STM32CUBEDUINO_MAP_ALL_IRQS */\n") >> H
	printf("\n") >> H
	printf("\n") >> H
	printf("/* Ensure that there are no macro re-mappings enabled. */\n") >> H
	lastInst = ""
	for (i = 1; i <= irqListCount; i++) {
		delete valAry
		match(irqList[i], /([a-zA-Z]+).*/, valAry)
		if (lastInst != "" && valAry[1] != lastInst) {
			printf("\n") >> H
		}
		lastInst = valAry[1]
		printf("#undef %s\n", irqList[i]) >> H
	}
	printf("\n") >> H
	printf("\n") >> H
	printf("/* C++ SFINAE for selecting the right IRQn */\n") >> H
	printf("/**\n") >> H
	printf(" * Defines a getIrqNumForXXX() function overload which returns the given IRQn for\n") >> H
	printf(" * the passed STM32 HAL instance, if such an IRQ exists.\n") >> H
	printf(" * \n") >> H
	printf(" * @param[in] INST - STM32 HAL instance\n") >> H
	printf(" * @param[in] IRQn - STM32 HAL IRQ number\n") >> H
	printf(" * @internal Use getIrqNumFor() to obtain a specific IRQ number of a given STM32 HAL instance.\n") >> H
	printf(" */\n") >> H
	printf("#define __DEF_GET_IRQ(INST, IRQn) \\\n") >> H
	printf("	template <typename T> \\\n") >> H
	printf("	constexpr static inline IRQn_Type getIrqNumFor##INST(decltype(T::IRQn) *) { \\\n") >> H
	printf("		return T::IRQn; \\\n") >> H
	printf("	}\n") >> H
	printf("\n") >> H
	printf("/**\n") >> H
	printf(" * Returns the IRQ number for the given STM32 HAL instance.\n") >> H
	printf(" * \n") >> H
	printf(" * @param[in] INST - STM32 HAL instance\n") >> H
	printf(" * @return corresponding IRQ number or linker error\n") >> H
	printf(" */\n") >> H
	printf("#define getIrqNumFor(INST) getIrqNumFor##INST<IRQn_Type>(0)\n") >> H
	printf("\n") >> H
	printf("\n") >> H
	printf("/* Map IRQs to the possible associated STM32 HAL instances for all IRQs available. */") >> H
	for (i = 1; i <= irqListCount; i++) {
		if (irqMap[irqList[i]] == "") continue # ignore non-instance IRQs
		delete valAry
		valCount = split(irqMap[irqList[i]], valAry, ",")
		delete valAry[1]
		delete valAry[valCount]
		valCount = asort(valAry, valAry, "cmpNatural")
		first = 1
		for (j = 1; j <= valCount; j++) {
			if (valAry[j] == "TIM1" && irqList[i] in ignTim1Map) continue # avoid ambiguous function definition
			if (first == 1) {
				printf("\n") >> H
				first = 0
			}
			printf("__DEF_GET_IRQ(%s, %s)\n", valAry[j], irqList[i]) >> H
		}
	}
	printf("\n") >> H
	printf("\n") >> H
	printf("/* C++ SFINAE for clearing the right NVIC IRQn */\n") >> H
	printf("/**\n") >> H
	printf(" * Defines a clearNvicItForXXX() function overload which clears the given IRQn,\n") >> H
	printf(" * if such an IRQ exists.\n") >> H
	printf(" * \n") >> H
	printf(" * @param[in] IRQn - STM32 HAL IRQ number constant\n") >> H
	printf(" */\n") >> H
	printf("#define __DEF_CLEAR_NVIC_IRQ(IRQn) \\\n") >> H
	printf("	template <typename T> \\\n") >> H
	printf("	static inline void clearNvicIrqFor##IRQn(decltype(T::IRQn) *) { \\\n") >> H
	printf("		HAL_NVIC_ClearPendingIRQ(T::IRQn); \\\n") >> H
	printf("		__ISB(); /* Ensure that the IRQ was really cleared at this point. */ \\\n") >> H
	printf("	} \\\n") >> H
	printf("	template <typename T> \\\n") >> H
	printf("	static inline void clearNvicIrqFor##IRQn(...) {}\n") >> H
	printf("\n") >> H
	printf("/**\n") >> H
	printf(" * Clears the given pending NVIC interrupt.\n") >> H
	printf(" * \n") >> H
	printf(" * @param[in] IRQn - STM32 HAL IRQ number constant\n") >> H
	printf(" */\n") >> H
	printf("#define clearNvicIrqFor(IRQn) clearNvicIrqFor##IRQn<IRQn_Type>(0)\n") >> H
	printf("\n") >> H
	printf("\n") >> H
	printf("/* Define functions to clear pending NVIC IRQs if available for the current platform for all IRQs available. */\n") >> H
	lastInst = ""
	for (i = 1; i <= irqListCount; i++) {
		delete valAry
		match(irqList[i], /([a-zA-Z]+).*/, valAry)
		if (lastInst != "" && valAry[1] != lastInst) {
			printf("\n") >> H
		}
		lastInst = valAry[1]
		printf("__DEF_CLEAR_NVIC_IRQ(%s)\n", irqList[i]) >> H
	}
	printf("\n") >> H
	printf("\n") >> H
	printf("extern \"C\" {\n") >> H
	printf("/* Remapped IRQ routines. One per STM32 HAL instance. These may be overwritten by STM32CubeDuino handlers or the user. */\n") >> H
	for (i = 1; i <= instListCount; i++) {
		printf("extern void STM32CubeDuinoIrqHandlerFor%s(void);\n", instList[i]) >> H
	}
	printf("} /* extern \"C\" */\n") >> H
	printf("\n") >> H
	printf("\n") >> H
	printf("#endif /* __WIRING_IRQ_H__ */\n") >> H
	
	# output code
	printf("/**\n") >> C
	printf(" * @file %s\n", basename(C)) >> C
	printf(" * @author Daniel Starke\n") >> C
	printf(" * @copyright Copyright 2020-2022 Daniel Starke\n") >> C
	printf(" * @date 2020-11-13\n") >> C
	printf(" * @version %s\n", strftime("%Y-%m-%d")) >> C
	printf(" */\n") >> C
	printf("#include \"Arduino.h\"\n") >> C
	printf("#include \"%s\"\n", basename(H)) >> C
	printf("\n") >> C
	printf("\n") >> C
	printf("#if !defined(STM32CUBEDUINO_DISABLE_TIMER) || !defined(STM32CUBEDUINO_DISABLE_PWM)\n") >> C
	printf("#undef STM32CUBEDUINO_DISABLE_ALL_TIMERS\n") >> C
	printf("#else\n") >> C
	printf("#define STM32CUBEDUINO_DISABLE_ALL_TIMERS\n") >> C
	printf("#endif\n") >> C
	printf("\n") >> C
	printf("\n") >> C
	printf("#ifdef __HAL_GET_PENDING_IT /* STM32F0 and STM32G0 specific interrupt line handling. */\n") >> C
	for (i = 1; i <= itLineListCount; i++) {
		delete valAry
		valCount = split(itLineMap[itLineList[i]], valAry, ",")
		for (j = 1; j <= valCount; j++) {
			printf("#ifdef HAL_ITLINE_%s\n", valAry[j]) >> C
			printf("#define ifIsPendingIrqFor%s() if (__HAL_GET_PENDING_IT(HAL_ITLINE_%s))\n", valAry[j], valAry[j]) >> C
			printf("#else /* not HAL_ITLINE_%s */\n", valAry[j]) >> C
			printf("#define ifIsPendingIrqFor%s() if ( false )\n", valAry[j]) >> C
			printf("#endif /* not HAL_ITLINE_%s */\n", valAry[j]) >> C
		}
	}
	printf("#else /* not __HAL_GET_PENDING_IT */\n") >> C
	for (i = 1; i <= itLineListCount; i++) {
		delete valAry
		valCount = split(itLineMap[itLineList[i]], valAry, ",")
		for (j = 1; j <= valCount; j++) {
			printf("#define ifIsPendingIrqFor%s()\n", valAry[j]) >> C
		}
	}
	printf("#endif /* not __HAL_GET_PENDING_IT */\n") >> C
	printf("\n") >> C
	printf("\n") >> C
	printf("/* Ensure that there are no macro re-mappings enabled. */\n") >> C
	lastInst = ""
	for (i = 1; i <= irqListCount; i++) {
		if (irqMap[irqList[i]] == "") continue # ignore non-instance IRQs
		delete valAry
		match(irqList[i], /([a-zA-Z]+).*/, valAry)
		if (lastInst != "" && valAry[1] != lastInst) {
			printf("\n") >> C
		}
		lastInst = valAry[1]
		printf("#undef %s\n", gensub(/_IRQn$/, "_IRQHandler", "g", irqList[i])) >> C
	}
	printf("\n") >> C
	printf("\n") >> C
	printf("extern \"C\" {\n") >> C
	printf("extern const char _wiring_irq; /**< @see _wiring_irq_forced_linkage in Arduino.cpp */\n") >> C
	printf("const char _wiring_irq = 1;\n") >> C
	printf("\n") >> C
	printf("/* Remapped IRQ routines. One per STM32 HAL instance. Empty fallback implementation is provided here. */\n") >> C
	for (i = 1; i <= instListCount; i++) {
		printf("__attribute__((weak)) void STM32CubeDuinoIrqHandlerFor%s(void) {}\n", instList[i]) >> C
	}
	printf("\n") >> C
	printf("\n") >> C
	printf("#ifndef STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS\n") >> C
	printf("/* STM32CubeDuinoIrqHandlerForXXX() wrapper implementation of all STM32 HAL IRQ handlers. Unused functions are removed by the linker. */\n") >> C
	first = 1
	for (i = 1; i <= irqListCount; i++) {
		if (irqMap[irqList[i]] == "") continue # ignore non-instance IRQs
		if (first != 1) {
			printf("\n\n") >> C
		}
		first = 0
		outputCondIfDef(irqMap[irqList[i]], C)
		printf("/** %s */\n", cmtMap[irqList[i]]) >> C
		printf("void %s(void) {\n", gensub(/_IRQn$/, "_IRQHandler", "g", irqList[i])) >> C
		printf("	clearNvicIrqFor(%s);\n", irqList[i]) >> C
		delete valAry
		valCount = split(irqMap[irqList[i]], valAry, ",")
		delete valAry[1]
		delete valAry[valCount]
		valCount = asort(valAry, valAry, "cmpNatural")
		for (j = 1; j <= valCount; j++) {
			if (valAry[j] in itLineMap) {
				delete valAry2
				valCount2 = split(itLineMap[valAry[j]], valAry2, ",")
				for (k = 1; k <= valCount2; k++) {
					printf("	ifIsPendingIrqFor%s() STM32CubeDuinoIrqHandlerFor%s();\n", valAry2[k], valAry[j]) >> C
				}
			} else {
				printf("	STM32CubeDuinoIrqHandlerFor%s();\n", valAry[j]) >> C
			}
		}
		printf("}\n") >> C
		outputCondEndIf(irqMap[irqList[i]], C)
	}
	printf("#endif /* STM32CUBEDUINO_CUSTOM_IRQ_HANDLERS */\n") >> C
	printf("} /* extern \"C\" */\n") >> C
}
