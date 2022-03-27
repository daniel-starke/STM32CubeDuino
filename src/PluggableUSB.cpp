/**
 * @file PluggableUSB.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2022-03-22
 */
#include "Arduino.h"
#include "PluggableUSB.h"


#ifdef USBCON
#ifdef PLUGGABLE_USB_ENABLED


/* USB endpoint description; defined and used in USBCore.cpp */
extern uint8_t _usbEndpoints[USB_ENDPOINTS];


/**
 * Constructor.
 */
PluggableUSB_::PluggableUSB_():
	lastIf(0),
	lastEp(1),
	rootNode(NULL)
{}


/**
 * Adds a new USB interface node.
 * 
 * @param[in] node - node to add
 * @return true on success, else false
 */
bool PluggableUSB_::plug(PluggableUSBModule * node) {
	/* maximum number of endpoints or dedicated USB memory reached? */
	if ((this->lastEp + node->numEndpoints) > USB_ENDPOINTS || (((this->lastEp + node->numEndpoints) * USB_EP_SIZE) + 64) > USB_PMASIZE) return false;

	/* get last node */
	if (this->rootNode == NULL) {
		this->rootNode = node;
	} else {
		PluggableUSBModule * current = this->rootNode;
		while (current->next != NULL) current = current->next;
		current->next = node;
	}

	/* append new node */
	node->pluggedInterface = this->lastIf;
	node->pluggedEndpoint = this->lastEp;
	this->lastIf = uint8_t(this->lastIf + node->numInterfaces);
	for (uint8_t i = 0; i < node->numEndpoints; i++) {
		_usbEndpoints[this->lastEp] = node->endpointType[i];
		this->lastEp++;
	}
	return true;
}


/**
 * Sends the USB interface description for each plugged node to the host.
 * 
 * @param[out] interfaceCount - filled with the number of plugged interfaces
 * @return bytes sent
 */
int PluggableUSB_::getInterface(uint8_t * interfaceCount) {
	int sent = 0;
	for (PluggableUSBModule * node = this->rootNode; node != NULL; node = node->next) {
		int res = node->getInterface(interfaceCount);
		if (res < 0) return -1;
		sent += res;
	}
	return sent;
}


/**
 * Sends the USB device descriptor for each plugged node to the host.
 * 
 * @param[in,out] setup - USB setup message
 * @return bytes sent
 */
int PluggableUSB_::getDescriptor(USBSetup & setup) {
	for (PluggableUSBModule * node = this->rootNode; node != NULL; node = node->next) {
		int res = node->getDescriptor(setup);
		if (res != 0) return res; /* request has been processed */
	}
	return 0;
}


/**
 * Calls the USB setup handler of each plugged node.
 * 
 * @param[in] setup - USB setup message
 * @return true on success, else false
 */
bool PluggableUSB_::setup(USBSetup & setup) {
	for (PluggableUSBModule * node = this->rootNode; node != NULL; node = node->next) {
		if ( node->setup(setup) ) return true;
	}
	return false;
}


/**
 * Returns the USB device serial number made up of all plugged nodes.
 * 
 * @param[out] name - copy serial number to this buffer
 * @return number of bytes copied
 */
uint8_t PluggableUSB_::getShortName(char * iSerialNum) {
	uint8_t size = 0;
	for (PluggableUSBModule * node = this->rootNode; node != NULL; node = node->next) {
		size = uint8_t(size + node->getShortName(iSerialNum + size));
	}
	return size;
}


/**
 * Replacement for global singleton to prevent static initialization issues.
 * 
 * @return Global PluggableUSB_ instance.
 */
PluggableUSB_ & PluggableUSB() {
	static PluggableUSB_ obj;
	return obj;
}


#endif /* PLUGGABLE_USB_ENABLED */
#endif /* USBCON */
