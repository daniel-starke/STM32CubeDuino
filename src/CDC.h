/**
 * @file CDC.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2022-04-17
 */
#ifndef __CDC_H__
#define __CDC_H__

#include "USBAPI.h"


#if !defined(STM32CUBEDUINO_DISABLE_SERIAL) && defined(PLUGGABLE_USB_ENABLED) && defined(USBCON)
#include "HardwareSerial.h"
#include "PluggableUSB.h"


#define HAVE_CDCSERIAL


struct CDCDescriptor {
	/* interface association descriptor */
	IADDescriptor iad; /* only needed on compound device */
	/* control */
	InterfaceDescriptor cif;
	CDCCSInterfaceDescriptor header;
	CMFunctionalDescriptor callManagement; /* call management */
	ACMFunctionalDescriptor controlManagement; /* ACM */
	CDCCSInterfaceDescriptor functionalDescriptor; /* CDC_UNION */
	EndpointDescriptor cifin;
	/* data */
	InterfaceDescriptor dif;
	EndpointDescriptor out;
	EndpointDescriptor in;
} __attribute__((packed));


class Serial_ : public Stream, public PluggableUSBModule {
private:
	uint8_t epType[3];
public:
	enum {
		ONE_STOP_BIT = 0,
		ONE_AND_HALF_STOP_BIT = 1,
		TWO_STOP_BITS = 2
	};
	enum {
		NO_PARITY = 0,
		ODD_PARITY = 1,
		EVEN_PARITY = 2,
		MARK_PARITY = 3,
		SPACE_PARITY = 4
	};
public:
	Serial_();
	void begin(const unsigned long baudrate) { this->begin(baudrate, SERIAL_8N1); }
	void begin(const unsigned long baudrate, const uint8_t mode);
	void end(void);

	virtual int available(void);
	virtual int availableForWrite(void);
	virtual int peek(void);
	virtual int read(void);
	virtual void flush(void);
	virtual size_t write(const uint8_t val);
	virtual size_t write(const uint8_t * buffer, size_t size);

	int32_t readBreak();
	uint32_t baud();
	uint8_t stopbits();
	uint8_t paritytype();
	uint8_t numbits();
	bool dtr();
	bool rts();

	operator bool();
	
	using Print::write;
protected:
	int getInterface(uint8_t * interfaceCount);
	int getDescriptor(USBSetup & setup);
	bool setup(USBSetup & setup);
	uint8_t getShortName(char * name);
};


extern Serial_ SerialUSB;


#endif /* USBCON */
#endif /* __PLUGGABLEUSB_H__ */
