#!/bin/gawk -f
# @file _mcuParse.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-20
# @version 2020-11-14
#
# STM32CubeIDE\STM32CubeIDE\plugins\com.st.stm32cube.common.mx_6.0.1.202008101643
# unzip -p STM32CubeMX.jar "devices/*" -x devices/stm32boards.db | FAMILIES_FILE=db/mcu/families.xml ./_mcuParse.awk >_mcu.py

function addCoreTarget(core, target) {
	delete _ary
	if ( match(target, /(.*)\((.+)\)(.*)/, _ary) ) {
		# multi-target given
		delete _ary2
		_count = split(_ary[2], _ary2, "-")
		for (_n = 1; _n <= _count; _n++) {
			# handle each single target
			_target = _ary[1] _ary2[_n] _ary[3]
			if (CoreMap[core] == "") CoreMap[core] = ","
			CoreMap[core] = CoreMap[core] _target ","
		}
	} else {
		# single target given
		if (CoreMap[core] == "") CoreMap[core] = ","
		CoreMap[core] = CoreMap[core] target ","
	}
}

function addDistinctItem(list, item) {
	if (list == "") return "\"" item "\""
	if (index(list, "\"" item "\"") != 0) return list
	return list ", \"" item "\""
}

function addDistinctItems(list, items, sep) {
	delete _ary
	_count = split(items, _ary, sep)
	for (_i = 1; _i <= _count; _i++) {
		if (_ary[_i] == "") continue
		list = addDistinctItem(list, _ary[_i])
	}
	return list
}

function fixCore(core) {
	return gensub(/\+/, "plus", "g", core)
}

BEGIN {
	if ( ! "FAMILIES_FILE" in ENVIRON ) {
		printf("Error: Missing environment variable FAMILIES_FILE.\n") > "/dev/stderr"
		exit 1
	}
	delete CoreMap
	oldFS = FS
	FS = "[<>\"]"
	while ((getline < ENVIRON["FAMILIES_FILE"]) > 0) {
		if ($0 ~ /RefName/) target = $3
		if ($2 == "Core" && target != "") {
			gsub(/Arm /, "", $3)
			addCoreTarget(tolower($3), target)
		}
	}
	FS = oldFS
	if (length(CoreMap) < 1) {
		printf("Error: File in FAMILIES_FILE does not exists.\n") > "/dev/stderr"
		exit 1
	}
	printf("# List of possible targets. Each elements contains: Part Numbers, Variants, Define, Cores, RAM, Flash\n")
	printf("# @see _mcuParse.awk\n")
	printf("mcuList = [")
	FirstMcu = 1
	delete VMap # variant map for wildcard MCU names
	delete DMap # define map for wildcard MCU names
	delete CMap # core map for wildcard MCU names
	delete RMap # RAM map for wildcard MCU names
	delete FMap # FLASH map for wildcard MCU names
}

/<device>/ {
	Products = ""
	Variants = ""
	Define = ""
	RAM = 0
	FLASH = 0
}

/<PN>/ {
	delete ary
	if ( match($0, /<PN>([^<]+)/, ary) ) Products = ary[1]
}

/<variants>/ {
	delete ary
	if ( match($0, /<variants>([^<]+)/, ary) ) Variants = ary[1]
}

/<memory name=".*RAM.*"/ {
	delete ary
	if ( match($0, /size="([^"]+)"/, ary) ) RAM += ary[1]
}

/<memory name=".*FLASH.*"/ {
	delete ary
	if ( match($0, /size="([^"]+)"/, ary) ) FLASH += ary[1]
}

/<define>/ {
	delete ary
	if ( match($0, /<define>([^<]+)/, ary) ) Define = ary[1]
}

/<\/device>/ {
	if (Products != "" && Define != "") {
		delete ary
		Count = split(Products, ary, ",")
		POut = ""
		Core = ""
		CoreList = ""
		XProd = ""
		for (i = 1; i <= Count; i++) {
			QProduct = "\"" ary[i] "\""
			if (match(POut, QProduct) || ary[i] ~ /-/) continue
			if (ary[i] ~ /x/) {
				# wildcard MCU name
				VMap[ary[i]] = addDistinctItems(VMap[ary[i]], Variants, ",")
				DMap[ary[i]] = addDistinctItems(DMap[ary[i]], Define, ",")
				RMap[ary[i]] = RAM
				FMap[ary[i]] = FLASH
				XProd = ary[i]
			} else {
				if (POut != "") POut = POut ", "
				POut = POut QProduct
			}
			for (c in CoreMap) {
				if ( match(CoreMap[c], "," ary[i]) ) {
					Core = addDistinctItem(Core, c)
					if (CoreList == "") {
						CoreList = c
					} else {
						CoreList = CoreList "," c
					}
				}
			}
		}
		if (CoreList != "" && XProd != "") {
			CMap[XProd] = addDistinctItems(CMap[XProd], CoreList, ",")
		}
		delete ary
		Count = split(Variants, ary, ",")
		VOut = ""
		for (i = 1; i <= Count; i++) {
			if (i != 1) VOut = VOut ", "
			VOut = VOut "\"" ary[i] "\""
		}
		if (Core == "") {
			printf("Warning: No core architecture found for targets %s. Targets omitted.\n", POut) > "/dev/stderr"
		} else {
			if (FirstMcu == 1) {
				printf("\n")
				FirstMcu = 0
			} else {
				printf(",\n")
			}
			printf("	[[%s], [%s], \"%s\", [%s], %i, %i]", POut, VOut, Define, fixCore(Core), RAM, FLASH)
		}
	}
}

END {
	# output wildcard MCU names
	delete McuList
	for (MCU in VMap) {
		McuList[MCU] = MCU
	}
	Count = asort(McuList)
	FirstX = 1
	for (i = 1; i <= Count; i++) {
		if (DMap[McuList[i]] ~ /,/) continue # ignore conflicting definitions
		if (CMap[McuList[i]] == "") continue # ignore incomplete targets
		if (FirstMcu == 1) {
			printf("\n")
			FirstMcu = 0
		} else {
			printf(",\n")
		}
		if (FirstX == 1) {
			printf("	# wildcard MCU list\n")
			FirstX = 0
		}
		printf("	[[\"%s\"], [%s], %s, [%s], %i, %i]", McuList[i], VMap[McuList[i]], DMap[McuList[i]], fixCore(CMap[McuList[i]]), RMap[McuList[i]], FMap[McuList[i]])
	}
	printf("\n]\n")
}
