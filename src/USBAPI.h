/**
 * @file USBAPI.h
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-05-21
 * @version 2022-09-17
 */
#ifndef __USBAPI_H__
#define __USBAPI_H__


#ifndef USB_EP_SIZE
/** Defines the endpoint size. */
#if (defined(STM32L1) || defined(STM32F1) || defined(STM32F3))
#define USB_EP_SIZE 32
#else
#define USB_EP_SIZE 64
#endif
#endif /* USB_EP_SIZE */
#if (USB_EP_SIZE & 0x0F) != 0
#error USB_EP_SIZE needs to be a multiple of 16.
#endif
#if USB_EP_SIZE >= 256
#error USB_EP_SIZE is too large.
#endif
#if (defined(STM32L1) || defined(STM32F1) || defined(STM32F3))
#if USB_EP_SIZE > 48
#ifndef STM32CUBEDUINO_DISABLE_USB_CDC
#warning The highest USB endpoint number for this platform is 4 due to USB_EP_SIZE. USB CDC together with HID is not possible.
#else
#warning The highest USB endpoint number for this platform is 4 due to USB_EP_SIZE.
#endif
#elif USB_EP_SIZE > 32
#warning The highest USB endpoint number for this platform is 6 due to USB_EP_SIZE.
#endif
#endif


#if !defined(STM32CUBEDUINO_DISABLE_USB) && defined(PCD_PHY_EMBEDDED) /* STM32 HAL PCD header was included */
#define USBCON
#endif


#ifdef USBCON
#if USE_HAL_PCD_REGISTER_CALLBACKS == 1
#error PCD register callbacks are not supported.
#endif /* USE_HAL_PCD_REGISTER_CALLBACKS */
#include "USBDesc.h"
#include "USBCore.h"


#define EP_TYPE_CONTROL          (USB_ENDPOINT_TYPE_CONTROL)
#define EP_TYPE_BULK_IN          (USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_IN(0))
#define EP_TYPE_BULK_OUT         (USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_OUT(0))
#define EP_TYPE_INTERRUPT_IN     (USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0))
#define EP_TYPE_INTERRUPT_OUT    (USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_OUT(0))
#define EP_TYPE_ISOCHRONOUS_IN   (USB_ENDPOINT_TYPE_ISOCHRONOUS | USB_ENDPOINT_IN(0))
#define EP_TYPE_ISOCHRONOUS_OUT  (USB_ENDPOINT_TYPE_ISOCHRONOUS | USB_ENDPOINT_OUT(0))


struct USBSetup {
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint8_t wValueL;
	uint8_t wValueH;
	uint16_t wIndex;
	uint16_t wLength;
} __attribute__((packed));


#define TRANSFER_ZERO    0x20
#define TRANSFER_RELEASE 0x40
#define TRANSFER_PGM     0x80


class USBDeviceClass {
private:
	bool initialized;
public:
	USBDeviceClass() {}
	void init();
	bool end();
	bool attach();
	bool detach();
	void setAddress(uint32_t addr);

	bool configured();
	bool connected();

	void standby();

	/* setup API */
	bool handleClassInterfaceSetup(USBSetup & setup);
	bool handleStandardSetup(USBSetup & setup);
	bool sendDescriptor(USBSetup & setup);

	/* control endpoint API */
	uint32_t sendControl(const void * data, uint32_t len);
	uint32_t sendControl(int /* ep */, const void * data, uint32_t len) { return sendControl(data, len); }
	uint32_t recvControl(void * data, uint32_t len);
	bool sendConfiguration(uint32_t maxLen);
	bool sendStringDescriptor(const uint8_t * string, uint32_t maxLen);
	uint8_t SendInterfaces(uint32_t * total);
	void packMessages(bool val);

	/* generic endpoint API */
	void initEndpoints(void);
	void initEP(uint32_t ep, uint32_t type);

	uint32_t send(uint32_t ep, const void * data, uint32_t len);
	void sendZlp(uint32_t ep);
	uint32_t recv(uint32_t ep, void * data, uint32_t len);
	int recv(uint32_t ep);
	uint32_t available(uint32_t ep);
	void flush(uint32_t ep);
	void clear(uint32_t ep);
	void stall(uint32_t ep);
};


extern USBDeviceClass USBDevice;


int USB_SendControl(uint8_t flags, const void * data, int len);
int USB_RecvControl(void * data, int len);
int USB_RecvControlLong(void * data, int len);

uint8_t USB_Available(uint8_t ep);
uint8_t USB_SendSpace(uint8_t ep);
int USB_Send(uint8_t ep, const void * data, int len); /* blocking */
int USB_Recv(uint8_t ep, void * data, int len); /* non-blocking */
int USB_Recv(uint8_t ep); /* non-blocking */
void USB_Flush(uint8_t ep);


#include "CDC.h"


#endif /* USBCON */
#endif /* __PLUGGABLEUSB_H__ */
