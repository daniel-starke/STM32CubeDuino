#!/bin/gawk -f
# @file _usbCollect.awk
# @author Daniel Starke
# @copyright Copyright 2020 Daniel Starke
# @date 2020-10-06
# @version 2020-11-14
#
# ./_usbCollect.awk _usb.txt > _usbTable.txt

function initializeMap() {
	delete Map
	for (usb = 0; usb < 6; usb++) {
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				Map[USB[usb] "_" Port[port] pin] = ""
			}
		}
	}
}

function printMap() {
	for (usb = 0; usb < 6; usb++) {
		hasUsb = 0
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				if (Map[USB[usb] "_" Port[port] pin] != "") hasUsb = 1
			}
		}
		if (hasUsb == 0) continue
		printf("%s\t%s\t", CurModel, USB[usb])
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("%s", Map[USB[usb] "_" Port[port] pin] != "" ? "1" : "0")
			}
		}
		for (port = 0; port < 12; port++) {
			for (pin = 0; pin < 16; pin++) {
				printf("\t%s", Map[USB[usb] "_" Port[port] pin])
			}
		}
		printf("\n")
	}
	initializeMap()
}

BEGIN {
	CurModel = ""
	USB[0] = "USB"
	USB[1] = "USBH_HS1"
	USB[2] = "USBH_HS2"
	USB[3] = "USB_OTG_FS"
	USB[4] = "USB_OTG_HS"
	USB[5] = "USB_OTG_HS_ULPI"
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
	if ( match($3, /^(USB(H_HS[12]|_OTG_(FS|HS)(_ULPI)?)?)/, ary) ) {
		Map[ary[1] "_" $2] = $3
	}
}

END {
	printMap()
}