/**
 * @file USBDesc.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2022-03-21
 */
#ifndef __USBDESC_H__
#define __USBDESC_H__


#define PLUGGABLE_USB_ENABLED

#define ISERIAL_MAX_LEN     32
#define IMANUFACTURER        1
#define IPRODUCT             2
#define ISERIAL              3

/* Deduce a default value if not already defined.
 * Note that bi-directional endpoints count only as one. */
#ifndef USB_ENDPOINTS
#ifdef USB_OTG_FS_HOST_MAX_CHANNEL_NBR
#define USB_ENDPOINTS USB_OTG_FS_HOST_MAX_CHANNEL_NBR
#elif defined(USB_OTG_EPNUM) /* USB_OTG_EPNUM */
#define USB_ENDPOINTS USB_OTG_EPNUM
#else /* not (USB_OTG_FS_HOST_MAX_CHANNEL_NBR or USB_OTG_EPNUM) */
#if defined(PCD_ENDP15)
#define USB_ENDPOINTS 16
#elif defined(PCD_ENDP14)
#define USB_ENDPOINTS 15
#elif defined(PCD_ENDP13)
#define USB_ENDPOINTS 14
#elif defined(PCD_ENDP12)
#define USB_ENDPOINTS 13
#elif defined(PCD_ENDP11)
#define USB_ENDPOINTS 12
#elif defined(PCD_ENDP10)
#define USB_ENDPOINTS 11
#elif defined(PCD_ENDP9)
#define USB_ENDPOINTS 10
#elif defined(PCD_ENDP8)
#define USB_ENDPOINTS 9
#elif defined(PCD_ENDP7)
#define USB_ENDPOINTS 8
#elif defined(PCD_ENDP6)
#define USB_ENDPOINTS 7
#elif defined(PCD_ENDP5)
#define USB_ENDPOINTS 6
#elif defined(PCD_ENDP4)
#define USB_ENDPOINTS 5
#elif defined(PCD_ENDP3)
#define USB_ENDPOINTS 4
#elif defined(PCD_ENDP2)
#define USB_ENDPOINTS 3
#elif defined(PCD_ENDP1)
#define USB_ENDPOINTS 2
#elif defined(PCD_ENDP0)
#define USB_ENDPOINTS 1
#else
#define USB_ENDPOINTS 0
#endif
#endif /* USB_OTG_FS_HOST_MAX_CHANNEL_NBR */
#endif /* USB_ENDPOINTS */


#ifndef USB_PMASIZE
#define USB_PMASIZE 512 /* true for most devices and the minimum over all */
#endif /* USB_PMASIZE */


#endif /* __USBDESC_H__ */
