/**
 * @file USBCore.h
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-05-21
 * @version 2020-05-21
 */
#ifndef __USBCORE_H__
#define __USBCORE_H__

#include "USBAPI.h"


/* standard requests */
#define GET_STATUS                             0
#define CLEAR_FEATURE                          1
#define SET_FEATURE                            3
#define SET_ADDRESS                            5
#define GET_DESCRIPTOR                         6
#define SET_DESCRIPTOR                         7
#define GET_CONFIGURATION                      8
#define SET_CONFIGURATION                      9
#define GET_INTERFACE                          10
#define SET_INTERFACE                          11


/* bmRequestType */
#define REQUEST_HOSTTODEVICE                   0x00
#define REQUEST_DEVICETOHOST                   0x80
#define REQUEST_DIRECTION                      0x80

#define REQUEST_STANDARD                       0x00
#define REQUEST_CLASS                          0x20
#define REQUEST_VENDOR                         0x40
#define REQUEST_TYPE                           0x60

#define REQUEST_DEVICE                         0x00
#define REQUEST_INTERFACE                      0x01
#define REQUEST_ENDPOINT                       0x02
#define REQUEST_OTHER                          0x03
#define REQUEST_RECIPIENT                      0x03

#define REQUEST_DEVICETOHOST_CLASS_INTERFACE    (REQUEST_DEVICETOHOST | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE    (REQUEST_HOSTTODEVICE | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_INTERFACE)


/* class requests */
#define CDC_SET_LINE_CODING                    0x20
#define CDC_GET_LINE_CODING                    0x21
#define CDC_SET_CONTROL_LINE_STATE             0x22
#define CDC_SEND_BREAK                         0x23

#define MSC_RESET                              0xFF
#define MSC_GET_MAX_LUN                        0xFE


/* descriptors */
#define USB_DEVICE_DESC_SIZE                   18
#define USB_CONFIGUARTION_DESC_SIZE            9
#define USB_INTERFACE_DESC_SIZE                9
#define USB_ENDPOINT_DESC_SIZE                 7

#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5


/* usb_20.pdf Table 9.6 Standard Feature Selectors */
#define ENDPOINT_HALT                          0
#define DEVICE_REMOTE_WAKEUP                   1
#define TEST_MODE                              2
#define B_HNP_ENABLE                           3
#define A_HNP_SUPPORT                          4
#define A_ALT_HNP_SUPPORT                      5


/* usb_20.pdf Figure 9-4. Information Returned by a GetStatus() Request to a Device */
#define FEATURE_SELFPOWERED_ENABLED           (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED         (1 << 1)

#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF

#define USB_CONFIG_POWERED_MASK                0x40
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20


/* bMaxPower in configuration descriptor */
#define USB_CONFIG_POWER_MA(mA)                ((mA) / 2)
#ifndef USB_CONFIG_POWER
#define USB_CONFIG_POWER                       500
#endif


/* bEndpointAddress in endpoint descriptor */
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 uint8_t(((addr) & 0x7F) | 0x00)
#define USB_ENDPOINT_IN(addr)                  uint8_t(((addr) & 0x7F) | 0x80)

#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03

#define TOBYTES(x)                             ((x) & 0xFF), (((x) >> 8) & 0xFF)

#define CDC_V1_10                              0x0110
#define CDC_COMMUNICATION_INTERFACE_CLASS      0x02
#define CDC_CALL_MANAGEMENT                    0x01
#define CDC_ABSTRACT_CONTROL_MODEL             0x02
#define CDC_HEADER                             0x00
#define CDC_ABSTRACT_CONTROL_MANAGEMENT        0x02
#define CDC_UNION                              0x06
#define CDC_CS_INTERFACE                       0x24
#define CDC_CS_ENDPOINT                        0x25
#define CDC_DATA_INTERFACE_CLASS               0x0A

#define MSC_SUBCLASS_SCSI                      0x06
#define MSC_PROTOCOL_BULK_ONLY                 0x50

#ifndef USB_VERSION
#define USB_VERSION                            0x200
#endif


struct DeviceDescriptor {
	uint8_t len; /* 18 */
	uint8_t dtype; /* 1 USB_DEVICE_DESCRIPTOR_TYPE */
	uint16_t usbVersion; /* 0x200 or 0x210 */
	uint8_t deviceClass;
	uint8_t deviceSubClass;
	uint8_t deviceProtocol;
	uint8_t packetSize0; /* packet 0 */
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t deviceVersion; /* 0x100 */
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} __attribute__((packed));


struct ConfigDescriptor {
	uint8_t len; /* 9 */
	uint8_t dtype; /* 2 */
	uint16_t clen; /* total length */
	uint8_t numInterfaces;
	uint8_t config;
	uint8_t iconfig;
	uint8_t attributes;
	uint8_t maxPower;
} __attribute__((packed));


struct InterfaceDescriptor {
	uint8_t len; /* 9 */
	uint8_t dtype; /* 4 */
	uint8_t number;
	uint8_t alternate;
	uint8_t numEndpoints;
	uint8_t interfaceClass;
	uint8_t interfaceSubClass;
	uint8_t protocol;
	uint8_t iInterface;
} __attribute__((packed));


struct EndpointDescriptor {
	uint8_t len; /* 7 */
	uint8_t dtype; /* 5 */
	uint8_t addr;
	uint8_t attr;
	uint16_t packetSize;
	uint8_t interval;
} __attribute__((packed));


/* Interface Association Descriptor */
/* Used to bind 2 interfaces together in CDC composite device. */
struct IADDescriptor {
	uint8_t len; /* 8 */
	uint8_t dtype; /* 11 */
	uint8_t firstInterface;
	uint8_t interfaceCount;
	uint8_t functionClass;
	uint8_t funtionSubClass;
	uint8_t functionProtocol;
	uint8_t iInterface;
} __attribute__((packed));


struct CDCCSInterfaceDescriptor {
	uint8_t len; /* 5 */
	uint8_t dtype; /* 0x24 */
	uint8_t subtype;
	uint8_t d0;
	uint8_t d1;
} __attribute__((packed));


struct CDCCSInterfaceDescriptor4 {
	uint8_t len; /* 4 */
	uint8_t dtype; /* 0x24 */
	uint8_t subtype;
	uint8_t d0;
} __attribute__((packed));


struct CMFunctionalDescriptor {
	uint8_t len;
	uint8_t dtype; /* 0x24 */
	uint8_t subtype; /* 1 */
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} __attribute__((packed));


struct ACMFunctionalDescriptor {
	uint8_t len;
	uint8_t dtype; /* 0x24 */
	uint8_t subtype; /* 1 */
	uint8_t bmCapabilities;
} __attribute__((packed));


struct MSCDescriptor {
	InterfaceDescriptor msc;
	EndpointDescriptor in;
	EndpointDescriptor out;
} __attribute__((packed));


#define D_DEVICE(_class ,_subClass, _proto, _packetSize0, _vid, _pid, _version, _im, _ip, _is, _configs) \
	{18, 1, USB_VERSION, _class, _subClass, _proto, _packetSize0, _vid, _pid, _version, _im, _ip, _is, _configs}

#define D_CONFIG(_totalLength, _interfaces) \
	{9, 2, _totalLength, _interfaces, 1, 0, USB_CONFIG_BUS_POWERED | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(USB_CONFIG_POWER)}

#define D_INTERFACE(_n, _numEndpoints, _class, _subClass, _protocol) \
	{9, 4, _n, 0, _numEndpoints, _class, _subClass, _protocol, 0}

#define D_ENDPOINT(_addr, _attr, _packetSize, _interval) \
	{7, 5, _addr, _attr, _packetSize, _interval}

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
	{8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0}

#define D_CDCCS(_subtype, _d0, _d1) \
	{5, 0x24, _subtype, _d0, _d1}

#define D_CDCCS4(_subtype, _d0) \
	{4, 0x24, _subtype, _d0}



#endif /* __USBCORE_H__ */
