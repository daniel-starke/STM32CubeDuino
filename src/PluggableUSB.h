/**
 * @file PluggableUSB.h
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-05-21
 * @version 2020-06-21
 */
#ifndef __PLUGGABLEUSB_H__
#define __PLUGGABLEUSB_H__

#include <stdint.h>
#include "USBAPI.h"


#ifdef USBCON
#ifdef PLUGGABLE_USB_ENABLED


class PluggableUSBModule {
protected:
	friend class PluggableUSB_;
	
	uint8_t pluggedInterface;
	uint8_t pluggedEndpoint;
	const uint8_t numEndpoints;
	const uint8_t numInterfaces;
	const uint8_t * endpointType; /**< @warning Pointer type is compatible to AVR Arduino, not SAMD Arduino. */
	PluggableUSBModule * next = NULL;
public:
	PluggableUSBModule(const uint8_t numEps, const uint8_t numIfs, const uint8_t * epType):
		numEndpoints(numEps),
		numInterfaces(numIfs),
		endpointType(epType)
	{}
	virtual ~PluggableUSBModule() {}
protected:
	virtual bool setup(USBSetup & setup) = 0;
	virtual int getInterface(uint8_t * interfaceCount) = 0;
	virtual int getDescriptor(USBSetup & setup) = 0;
	virtual uint8_t getShortName(char * name) {
		name[0] = char('A' + this->pluggedInterface);
		return 1;
	}
};


class PluggableUSB_ {
private:
	uint8_t lastIf;
	uint8_t lastEp;
	PluggableUSBModule * rootNode;
public:
	PluggableUSB_();
	
	bool plug(PluggableUSBModule * node);
	int getInterface(uint8_t * interfaceCount);
	int getDescriptor(USBSetup & setup);
	bool setup(USBSetup & setup);
	uint8_t getShortName(char * iSerialNum);
};


/* Replacement for global singleton to prevent static initialization issues. */
PluggableUSB_ & PluggableUSB();


#endif /* PLUGGABLE_USB_ENABLED */
#endif /* USBCON */
#endif /* __PLUGGABLEUSB_H__ */
