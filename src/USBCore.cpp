/**
 * @file USBCore.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2023-09-25
 * 
 * Control Endpoint:
 * @verbatim
 *              Host                        Device
 *               |                            |
 *  +----------->|                            |
 *  |    .----------------.   EP:OUT  .----------------.
 *  |    |                |---------->|                |
 *  |    |                |   SETUP   |                |
 *  |    |    Request     |           |    Response    |
 *  |    |                |   EP:IN   |                |
 *  |    |                |<----------|                |
 *  |    '----------------'    DATA   '----------------'
 *  |            |          (optional)        |
 *  |  +---------+                            +-----------+
 *  |  |         |                            |           |
 *  |  |         v                            v           |
 *  |  | .----------------.           .----------------.  |
 *  |  | |                |   EP:OUT  |                |  |
 *  |  | |    Settings    |---------->|    Applied     |  |
 *  |  | |                |   DATA    |                |  |
 *  |  | '----------------'           '----------------'  |
 *  |  |         |                            |           |
 *  |  +---------+                            +-----------+
 *  |            |                            |
 *  |            |                            v
 *  |            |                    .----------------.
 *  |            |         EP:IN      |                |
 *  |            |<-------------------|  Transaction   |
 *  |            |  zero-length-packet|   complete     |
 *  +------------+                    '----------------'
 * @endverbatim
 * 
 * Settings are replied by the device depending on the request type.
 * Settings may be sent to the device depending on the request type.
 * 
 * @remarks IN endpoints transfer data from device to host
 * @remarks OUT endpoints transfer data from host to device
 * @remarks HAL_PCD_EP_Receive() and HAL_PCD_EP_Transmit() are the STM32 HAL function to handle USB data packet transfers, not byte data transfer.
 * That means that both transmit exactly one USB data packet at a time and need to be called again for the next one. The size may be less than
 * USB_EP_SIZE for HAL_PCD_EP_Transmit() but not for HAL_PCD_EP_Receive(). Providing a buffer less than USB_EP_SIZE for HAL_PCD_EP_Receive() may
 * result in a buffer overrun. I.e. data after the buffer will be overwritten. Hence, the reception procedure requires 3 buffer: the device internal
 * dedicated reception buffer, the internal packet reception buffer and the FIFO to the upper Arduino API layer.
 * @remarks PMA memory is 32 bit aligned in general and 32 byte aligned for the packet buffers. Each 32 bit contain 16 bit of data. Each endpoint is
 * handled using four 16 bit values. That means 16 byte of data (32 byte in memory) are necessary for each endpoint (including IN and OUT). Hence, for
 * an MCU with 512 byte PMA memory (e.g. STM32F103, see UM0424) the highest endpoint number is 4 using 64 byte buffer per endpoint. USB CDC + HID is
 * not possible in this configuration.
 * @remarks HAL PCD handles endpoint transmission states in internal structures for IN and OUT that all contain the fields xfer_count, xfer_buff and
 * xfer_len. Handling of these fields is not consistent between the various STM32 series and HAL versions which makes it impossible to rely on those
 * to save some memory. The internal variables bytesPendingEp and bufferPtrEp are introduces instead.
 * @see https://higaski.at/custom-class-for-stm32-usb-device-library/
 * @see https://www.st.com/resource/en/user_manual/dm00108129-stm32cube-usb-device-library-stmicroelectronics.pdf
 * @remarks Note bug for PMA handling in STM32F1 https://community.st.com/s/question/0D50X00009XkXLz
 * @remarks USB HS is currently not supported.
 */
#include "Arduino.h"
#include "PluggableUSB.h"
#include "scdinternal/fifo.h"
#include "scdinternal/macro.h"


#ifdef USBCON


#ifndef USB_IRQ_PRIO
#error Please define USB_IRQ_PRIO in board.hpp.
#endif
#ifndef USB_IRQ_SUBPRIO
#error Please define USB_IRQ_SUBPRIO in board.hpp.
#endif


/* ensure that there is no macro remapping enforced for shared interrupt routines */
#undef USB_LP_CAN1_RX0_IRQHandler
#undef USB_LP_CAN1_RX0_IRQHandler


#ifndef USB_PRODUCT
/* if no product is provided, use USB IO Board */
#define USB_PRODUCT "USB IO Board"
#endif


#ifndef USB_MANUFACTURER
#if USB_VID == 0x2341
#define USB_MANUFACTURER "Arduino LLC"
#elif USB_VID == 0x0483
#define USB_MANUFACTURER "STMicroelectronics"
#else
/* fallback if no manufacturer name was provided */
#define USB_MANUFACTURER "Unknown"
#endif
#endif /* not USB_MANUFACTURER */


#ifndef PCD_EP_TYPE_CTRL
#define PCD_EP_TYPE_CTRL EP_TYPE_CTRL
#endif
#ifndef PCD_EP_TYPE_ISOC
#define PCD_EP_TYPE_ISOC EP_TYPE_ISOC
#endif
#ifndef PCD_EP_TYPE_BULK
#define PCD_EP_TYPE_BULK EP_TYPE_BULK
#endif
#ifndef PCD_EP_TYPE_INTR
#define PCD_EP_TYPE_INTR EP_TYPE_INTR
#endif


#ifndef USB_RX_SIZE
/** optimal memory/speed trace-off for USB FS was measured at USB_EP_SIZE * 2, maximum speed at USB_EP_SIZE * 3 for STM32F401 */
#define USB_RX_SIZE (USB_EP_SIZE * 2)
#endif /* USB_RX_SIZE */


#ifndef USB_TX_SIZE
/** optimal memory/speed trade-off for USB FS was measured at USB_EP_SIZE * 4 for STM32F401 */
#define USB_TX_SIZE (USB_EP_SIZE * 2)
#endif /* USB_TX_SIZE */


#if USB_RX_SIZE < (USB_EP_SIZE * 2)
#error USB_RX_SIZE needs to be at least 2 * USB_EP_SIZE.
#endif


#if USB_TX_SIZE < (USB_EP_SIZE * 2) || (USB_TX_SIZE % USB_EP_SIZE) != 0
#error USB_TX_SIZE needs to be a multiple of USB_EP_SIZE and at least 2 * USB_EP_SIZE.
#endif


/**
 * @macro USB_TX_TRANSACTIONAL
 * USB_TX_TRANSACTIONAL is defined in board.hpp if the used STM32Cube library handles
 * multi-packet USB transactions with a single HAL_PCD_DataInStageCallback() call.
 * Usually, the USB device will not enumerate properly if this is set incorrectly.
 */
#ifdef USB_TX_TRANSACTIONAL
#define USB_IO_TX_CHUNK(len) (len)
#else /* not USB_TX_TRANSACTIONAL */
#define USB_IO_TX_CHUNK(len) min(USB_EP_SIZE, (len))
#endif /* not USB_TX_TRANSACTIONAL */


#define DEVICE_CLASS 0xEF
#define DEVICE_SUB_CLASS 0x02
#define DEVICE_PROTOCOL 0x01
#define DEVICE_VERSION 0x100


/* Timeout for sends and receives. */
#define USB_WFI_TIMEOUT_MS 1000
#define USB_IO_TIMEOUT_MS 70
#define PP_CAT(x, y) x##y
#ifdef __thumb__
#define INST_TYPE uint16_t
#else
#define INST_TYPE uint32_t
#endif
/* timeout enabled wait to avoid deadlocks; timeout is only a rough estimation here */
#define USB_BUSY_WAIT_UNTIL(cond) [&]() -> bool { \
	const uint32_t _instCount = reinterpret_cast<const INST_TYPE *>(&&PP_CAT(_jmp_end_, __LINE__)) - reinterpret_cast<const INST_TYPE *>(&&PP_CAT(_jmp_start_, __LINE__)); \
	uint32_t _timeout = microsecondsToClockCycles(USB_IO_TIMEOUT_MS * 1000) / _instCount; /* we assume that each instruction takes one cycle */ \
	PP_CAT(_jmp_start_, __LINE__): \
		if ( !! (cond) ) return true; \
		if (_timeout == 0) goto PP_CAT(_jmp_end_, __LINE__); \
		_timeout--; \
		goto PP_CAT(_jmp_start_, __LINE__); \
	PP_CAT(_jmp_end_, __LINE__): \
	return false; \
}()


/** STM32 HAL specific variable needed in usbFrameNumber(). */
extern uint32_t USBx_BASE;


extern const uint16_t STRING_LANGUAGE[];
extern const uint8_t STRING_PRODUCT[];
extern const uint8_t STRING_MANUFACTURER[];
extern const DeviceDescriptor USB_DeviceDescriptorIAD;


const DeviceDescriptor USB_DeviceDescriptorIAD = D_DEVICE(
	DEVICE_CLASS, DEVICE_SUB_CLASS, DEVICE_PROTOCOL, USB_EP_SIZE,
	USB_VID, USB_PID, DEVICE_VERSION,
	IMANUFACTURER, IPRODUCT, ISERIAL,
	1
);
const uint16_t STRING_LANGUAGE[2] = {(3 << 8) | (2 + 2), 0x0409 /* English */};
const uint8_t STRING_PRODUCT[] = USB_PRODUCT;
const uint8_t STRING_MANUFACTURER[] = USB_MANUFACTURER;


/* filled in PluggableUSB_::plug() */
uint8_t _usbEndpoints[USB_ENDPOINTS] = {
	USB_ENDPOINT_TYPE_CONTROL | USB_ENDPOINT_IN(0) | USB_ENDPOINT_OUT(0),
	0 /*, ... all zero */
};


namespace {
struct _UsbRxBuffer {
	enum { PacketSize = USB_EP_SIZE };
	typedef _FifoClass<USB_RX_SIZE - PacketSize> FifoType;
	uint8_t packet[PacketSize]; /**< packet reception buffers */
	FifoType fifo; /**< FIFO to upper layer */
};
struct _UsbTxBuffer {
	typedef _BlockFifoClass<USB_TX_SIZE, USB_EP_SIZE> FifoType;
	FifoType fifo; /**< FIFO to PHY */
	volatile bool commitLock; /**< true if no commits are allowed from ISR */
};


void * _usbBuf[USB_ENDPOINTS] = {NULL}; /* index + 1 = endpoint number */
uint8_t _usbCtrlRecvBuf[64]; /* holds data from the host associated to a class interface setup request */
bool isRemoteWakeUpEnabled = false;
bool isEndpointHalt = false;
uint8_t ctrlStatBuf[2];

volatile uint16_t txPendingEp = 0; /* IN endpoints */
volatile uint16_t rxPendingEp = 0; /* OUT endpoints */
volatile uint32_t bytesPendingEp[USB_ENDPOINTS + 1]; /* for each endpoint; two for control */
uint8_t * bufferPtrEp[USB_ENDPOINTS + 1]; /* for each endpoint; two for control */
} /* anonymous namespace */


volatile uint8_t _usbConfiguration = 0;
volatile uint8_t _usbSetInterface = 0;

bool _dry_run = false;
bool _pack_message = false;
uint16_t _pack_size = 0;
uint8_t _pack_buffer[256];
PCD_HandleTypeDef hPcdUsb[1];


#ifdef PCD_SNG_BUF
/* C++ SFINAE for handling the absence of HAL_PCDEx_PMAConfig() as a NOP */
DEF_NO_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_PCDEx_PMAConfig_Wrapper, HAL_PCDEx_PMAConfig, HAL_OK, PCD_HandleTypeDef *, uint16_t, uint16_t, uint32_t)
#endif /* PCD_SNG_BUF */

/* C++ SFINAE for handling the absence of HAL_PCDEx_SetTxFiFo() and HAL_PCDEx_SetRxFiFo() as a NOP */
DEF_NO_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_PCDEx_SetTxFiFo_Wrapper, HAL_PCDEx_SetTxFiFo, HAL_OK, PCD_HandleTypeDef *, uint8_t, uint16_t)
DEF_NO_FN_ARGS_WRAPPER(HAL_StatusTypeDef, HAL_PCDEx_SetRxFiFo_Wrapper, HAL_PCDEx_SetRxFiFo, HAL_OK, PCD_HandleTypeDef *, uint16_t)


/**
 * Calls the given functor or lambda for each available FS USB IRQn.
 * 
 * @param[in] fn - functor or lamba to call (accepting one parameter of type IRQn_Type)
 */
template <typename Fn>
static void usbCallForEachIrqNum(Fn && fn) {
	#define CALL_FOR_IRQ_NUM(x) CALL_FOR_EXISTING_FIELD(fn, IRQn_Type, x)
	CALL_FOR_IRQ_NUM(OTG_FS_EP1_IN_IRQn);
	CALL_FOR_IRQ_NUM(OTG_FS_EP1_OUT_IRQn);
	CALL_FOR_IRQ_NUM(OTG_FS_IRQn);
	CALL_FOR_IRQ_NUM(OTG_FS_WKUP_IRQn);
	CALL_FOR_IRQ_NUM(OTG_IRQn);
	CALL_FOR_IRQ_NUM(USBWakeUp_IRQn);
	CALL_FOR_IRQ_NUM(USBWakeUp_RMP_IRQn);
	CALL_FOR_IRQ_NUM(USB_FS_IRQn);
	CALL_FOR_IRQ_NUM(USB_FS_WKUP_IRQn);
	CALL_FOR_IRQ_NUM(USB_IRQn);
	CALL_FOR_IRQ_NUM(USB_LP_CAN1_RX0_IRQn);
	CALL_FOR_IRQ_NUM(USB_LP_CAN_RX0_IRQn);
	CALL_FOR_IRQ_NUM(USB_LP_IRQn);
	#undef CALL_FOR_IRQ_NUM
}


/**
 * Enables all interrupts for USB FS.
 */
static inline void usbNvicEnable() {
	usbCallForEachIrqNum([] (const IRQn_Type irqNum) -> void {
		HAL_NVIC_EnableIRQ(irqNum);
	});
}


/**
 * Disables all interrupts for USB FS.
 */
static inline void usbNvicDisable() {
	usbCallForEachIrqNum([] (const IRQn_Type irqNum) -> void {
		HAL_NVIC_DisableIRQ(irqNum);
	});
}


/**
 * Sets the priority of all interrupts for USB FS.
 */
static inline void usbNvicSetPriority() {
	usbCallForEachIrqNum([] (const IRQn_Type irqNum) -> void {
		HAL_NVIC_SetPriority(irqNum, USB_IRQ_PRIO, USB_IRQ_SUBPRIO);
	});
}


/**
 * Returns the lowest priority (i.e. highest value) of all USB FS interrupts.
 * 
 * @return lowest USB FS interrupt priority
 */
static inline uint32_t usbNvicGetLowestPriority() {
	uint32_t res = 0;
	usbCallForEachIrqNum([&res] (const IRQn_Type irqNum) {
		const uint32_t prio = NVIC_GetPriority(irqNum);
		if (prio > res) res = prio;
	});
	return res;
}


/**
 * Returns the associated reception buffer for the given endpoint number.
 * 
 * @param[in] epNum - endpoint number (>0)
 * @return RX buffer or NULL 
 */
static _UsbRxBuffer * usbRxBuffer(const uint8_t epNum) {
	const uint8_t config = _usbEndpoints[epNum];
	if ((config & USB_ENDPOINT_DIRECTION_MASK) != USB_ENDPOINT_OUT(0)) return NULL;
	return reinterpret_cast<_UsbRxBuffer *>(_usbBuf[epNum - 1]);
}


/**
 * Returns the associated transmission buffer for the given endpoint number.
 * 
 * @param[in] epNum - endpoint number (>0)
 * @return TX buffer or NULL 
 */
static _UsbTxBuffer * usbTxBuffer(const uint8_t epNum) {
	const uint8_t config = _usbEndpoints[epNum];
	if ((config & USB_ENDPOINT_DIRECTION_MASK) != USB_ENDPOINT_IN(0)) return NULL;
	return reinterpret_cast<_UsbTxBuffer *>(_usbBuf[epNum - 1]);
}


/**
 * Deletes all allocated reception and transmission buffers.
 */
static void usbDeleteBuffers() {
	for (uint8_t i = 0; i < (sizeof(_usbBuf) / sizeof(*_usbBuf)); i++) {
		if (_usbBuf[i] != NULL) {
			const uint8_t config = _usbEndpoints[i + 1];
			if ((config & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_OUT(0)) {
				delete usbRxBuffer(uint8_t(i + 1));
			} else {
				delete usbTxBuffer(uint8_t(i + 1));
			}
		}
		_usbBuf[i] = NULL;
	}
}


/**
 * Returns the current USB frame number.
 * 
 * @return USB frame number
 */
static uint32_t usbFrameNumber() {
#ifdef USB_OTG_DSTS_FNSOF
	USB_OTG_GlobalTypeDef * USBx __attribute__((unused)) = hPcdUsb->Instance;
	return uint32_t((USBx_DEVICE->DSTS & USB_OTG_DSTS_FNSOF_Msk) >> USB_OTG_DSTS_FNSOF_Pos);
#else
	return uint32_t(hPcdUsb->Instance->FNR & USB_FNR_FN);
#endif
}


/**
 * Sends data on the given endpoint. The operation blocks if there is still
 * a previous data transmission ongoing. Else returns immediately if blocking is false.
 * 
 * @param[in] ep - endpoint
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @param[in] blocking - block until all data was transferred
 * @return true on success, else false on timeout
 * @remarks The passed buffer needs to be valid until the data has been successfully transmitted.
 */
static bool sendOrBlock(const uint8_t ep, const void * data, const uint32_t len, const bool blocking = false) {
	const uint8_t epNum = uint8_t(ep & 0xF);
	const uint8_t epIdx = uint8_t(epNum + 1);
	const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
	if ( blocking ) {
		/* wait until last operation completed */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
		if ( ! USB_BUSY_WAIT_UNTIL((txPendingEp & epMask) == 0) ) return false;
#pragma GCC diagnostic pop
	} else if ((txPendingEp & epMask) != 0) {
		return false;
	}
	txPendingEp |= epMask; /* mark as pending */
	bufferPtrEp[epIdx] = const_cast<uint8_t *>(static_cast<const uint8_t *>(data));
	bytesPendingEp[epIdx] = len;
	HAL_PCD_EP_Transmit(hPcdUsb, ep, bufferPtrEp[epIdx], USB_IO_TX_CHUNK(len));
	if ( blocking ) {
		/* wait until the transmission completed */
		const uint32_t startTime = millis();
		while ((txPendingEp & epMask) != 0) {
			__WFI();
			if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) break;
		}
	}
	return true;
}


/**
 * Triggers the transmission of a non-control endpoint by pushing the buffered
 * data from its FIFO to the send routine. The function shall only be called if
 * data can be transmitted (i.e. no pending data for that endpoint).
 * 
 * @param[in] buf - output buffers
 * @param[in] epNum - endpoint number
 * @param[in] allowEmpty - true if empty packets are allowed, else false
 */
static void usbTriggerSend(_UsbTxBuffer & buf, const uint8_t epNum, const bool allowEmpty) {
	uint32_t blockSize;
	const uint8_t * blockPtr = buf.fifo.peek(blockSize);
	if (blockPtr == NULL) return;
	/* must disable interrupt to prevent handle lock contention */
	usbNvicDisable();
	__DMB(); __DSB(); __ISB(); /* data and instruction barrier */
#if USB_TX_SIZE >= (USB_EP_SIZE * 4) && defined(USB_TX_TRANSACTIONAL)
	const _UsbTxBuffer::FifoType::IndexType nextTail = _UsbTxBuffer::FifoType::IndexType(buf.fifo.tail + 1);
	if (blockSize >= USB_EP_SIZE && nextTail < _UsbTxBuffer::FifoType::Count && nextTail != buf.fifo.head) {
		/* queue two blocks at once */
		sendOrBlock(USB_ENDPOINT_IN(epNum), blockPtr, blockSize + buf.fifo.size[nextTail]);
	} else
#endif
	if (blockSize > 0) {
		sendOrBlock(USB_ENDPOINT_IN(epNum), blockPtr, blockSize);
	} else if ( allowEmpty ) {
		sendOrBlock(USB_ENDPOINT_IN(epNum), NULL, 0);
	}
	/* enable interrupt */
	usbNvicSetPriority();
	usbNvicEnable();
}


/**
 * Send data to an endpoint. Block until the transmission finished.
 * This function is needed to handle flags that may get passed together
 * with the endpoint in `USB_Send()`.
 * 
 * @param[in] flags - `TRANSFER_ZERO`, `TRANSFER_RELEASE` and/or `TRANSFER_PGM`
 * @param[in] ep - endpoint number
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @return bytes transmitted
 */
static uint32_t sendHelper(const uint8_t flags, const uint32_t ep, const void * data, uint32_t len) {
	if ( ! _usbConfiguration ) return uint32_t(-1);
	const uint8_t epNum = uint8_t(ep & 0xF);
	if (usbTxBuffer(epNum) == NULL) {
		if ( ! sendOrBlock(USB_ENDPOINT_IN(ep), data, len, true) ) return 0;
	} else {
		_UsbTxBuffer & buf = *usbTxBuffer(epNum);
		bool canWait = false;
		const bool zlp = (len == 0);
		const bool interruptsEnabled = ((__get_PRIMASK() & 0x1) == 0);
		const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
		if ( interruptsEnabled ) {
			const uint32_t irqExecutionNumber = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
			if (irqExecutionNumber == 0 || NVIC_GetPriority(IRQn_Type(irqExecutionNumber - 16)) > usbNvicGetLowestPriority()) {
				/* USB interrupt is enabled and we were not called from an interrupt with higher priority */
				canWait = true;
			}
		}
		/* write data to queue */
		buf.commitLock = true;
		if ( canWait ) {
			/* wait until space is available and add to queue */
			__DMB(); __DSB(); __ISB(); /* data and instruction barrier */
			const uint8_t * dataBuf = reinterpret_cast<const uint8_t *>(data);
			for (uint32_t i = 0; i < len; dataBuf++, i++) {
				const uint32_t startTime = millis();
				while ( ! buf.fifo.push(((flags & TRANSFER_ZERO) == 0) ? *dataBuf : 0) ) {
					if ((txPendingEp & epMask) == 0) {
						/* trigger send in case something clogged up */
						usbTriggerSend(buf, epNum, false);
						break;
					}
					__WFI();
					if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) {
						/* interface got stuck -> reattach it */
						USBDevice.detach();
						USBDevice.attach();
						return i;
					}
				}
			}
			if ((flags & TRANSFER_RELEASE) != 0) {
				const uint32_t startTime = millis();
				while ( ! buf.fifo.commitBlock() ) {
					if ((txPendingEp & epMask) == 0) {
						/* trigger send in case something clogged up */
						usbTriggerSend(buf, epNum, false);
						break;
					}
					__WFI();
					if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) {
						/* interface got stuck -> reattach it */
						USBDevice.detach();
						USBDevice.attach();
						return len;
					}
				}
			}
		} else {
			/* add to queue or fail if no space is available, because there is currently no chance to get data out */
			if (( ! buf.fifo.full() ) || (flags & TRANSFER_RELEASE) == 0) {
				uint32_t written;
				if ((flags & TRANSFER_ZERO) == 0) {
					written = buf.fifo.write(reinterpret_cast<const uint8_t *>(data), len);
					if (written < len && (txPendingEp & epMask) == 0) {
						usbTriggerSend(buf, epNum, false);
						written += buf.fifo.write(reinterpret_cast<const uint8_t *>(data) + written, uint32_t(len - written));
					}
				} else {
					/* write only zero bytes */
					for (written = 0; written < len && buf.fifo.push(0); written++);
					if (written < len && (txPendingEp & epMask) == 0) {
						usbTriggerSend(buf, epNum, false);
						for (; written < len && buf.fifo.push(0); written++);
					}
				}
				if ((flags & TRANSFER_RELEASE) != 0) {
					buf.fifo.commitBlock();
				}
				len = written;
			} else {
				/* TRANSFER_RELEASE was requested but no free block is available to handle this */
				len = 0;
			}
		}
		buf.commitLock = false;
		/* the interrupt routine may send out the queued data at this point */
		__DMB(); __DSB(); __ISB(); /* data and instruction barrier */
		if ((txPendingEp & epMask) != 0) return len;
		if ((flags & TRANSFER_RELEASE) != 0 || zlp) {
			/* start endpoint transmission from idle state */
			usbTriggerSend(buf, epNum, zlp);
		}
	}
	return len;
}


/**
 * Receives data on the given endpoint. The operation fails if there is still
 * a previous data reception ongoing. It may be set blocking until completion.
 * 
 * @param[in] ep - endpoint
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @param[in] blocking - block until all data was received
 * @return true on success (not necessary completion), else false
 * @remarks The passed buffer needs to be valid until the data has been successfully received.
 */
static bool recvOrFail(const uint8_t ep, void * data, const uint32_t len, const bool blocking = false) {
	const uint8_t epNum = uint8_t(ep & 0xF);
	const uint8_t epIdx = (epNum == 0) ? 0 : uint8_t(epNum + 1);
	const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
	if ( blocking ) {
		/* wait until last operation completed */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
		if ( ! USB_BUSY_WAIT_UNTIL((rxPendingEp & epMask) == 0) ) return false;
#pragma GCC diagnostic pop
	} else if ((rxPendingEp & epMask) != 0) {
		return false;
	}
	rxPendingEp |= epMask; /* mark as pending */
	bytesPendingEp[epIdx] = len;
	HAL_PCD_EP_Receive(hPcdUsb, ep, reinterpret_cast<uint8_t *>(data), len);
	if ( blocking ) {
		/* wait until the reception completed */
		const uint32_t startTime = millis();
		while ((rxPendingEp & epMask) != 0) {
			__WFI();
			if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) break;
		}
	}
	return true;
}


/**
 * Initializes the USB peripheral device.
 * 
 * @remarks Enabling DMA transfer is currently not supported.
 */
void USBDeviceClass::init() {
	/* initialize the USB device */
	hPcdUsb->pData = this;
#ifdef USB_OTG_FS
	hPcdUsb->Instance = USB_OTG_FS;
#else
	hPcdUsb->Instance = USB;
#endif
	hPcdUsb->Init.dev_endpoints = USB_ENDPOINTS; /* see USBDesc.h */
	hPcdUsb->Init.speed = PCD_SPEED_FULL;
	hPcdUsb->Init.phy_itface = PCD_PHY_EMBEDDED;
	hPcdUsb->Init.low_power_enable = DISABLE;
	hPcdUsb->Init.lpm_enable = DISABLE; /* Link Power Management */
	hPcdUsb->Init.battery_charging_enable = DISABLE;
	if (HAL_PCD_Init(hPcdUsb) != HAL_OK) {
		systemErrorHandler();
		return;
	}
	this->initialized = true;
}


/**
 * De-initializes the USB peripheral device.
 * 
 * @returns true on success, else false if no device had been previously initialized
 */
bool USBDeviceClass::end() {
	if ( ! this->initialized ) return false;
	HAL_PCD_DeInit(hPcdUsb);
	usbDeleteBuffers();
	return true;
}


/**
 * Attach and start the USB device.
 * 
 * @return true on success, else false
 */
bool USBDeviceClass::attach() {
	if ( ! this->initialized ) return false;
	_usbConfiguration = 0;
	txPendingEp = 0;
	rxPendingEp = 0;
	for (size_t i = 0; i < (sizeof(bytesPendingEp) / sizeof(*bytesPendingEp)); i++) {
		bytesPendingEp[i] = 0;
	}
	HAL_PCD_Start(hPcdUsb);
	return true;
}


/**
 * Detaches and stops the USB device.
 * 
 * @return true on success, else false
 */
bool USBDeviceClass::detach() {
	if ( ! this->initialized ) return false;
	HAL_PCD_Stop(hPcdUsb);
	usbDeleteBuffers();
	return true;
}


/**
 * Sets the USB device address.
 * 
 * @param[in] addr - new address
 */
void USBDeviceClass::setAddress(uint32_t addr) {
	HAL_PCD_SetAddress(hPcdUsb, uint8_t(addr));
}


/**
 * Returns whether the USB device had been configured.
 * 
 * @return true if configured, else false
 */
bool USBDeviceClass::configured() {
	return _usbConfiguration != 0;
}


/**
 * Returns whether the USB device is connected to a host.
 * 
 * @return true if connected, else false
 */
bool USBDeviceClass::connected() {
	const uint32_t frameNumber = usbFrameNumber();
	delay(3); /* we receive one frame every millisecond in connected state */
	return frameNumber != usbFrameNumber();
}


/**
 * Resumes operation from standby.
 * 
 * @remark This function is currently unsupported.
 */
void USBDeviceClass::standby() {
	/* returning from standby is unsupported */
}


/**
 * Handles the class interface setup request.
 * 
 * @param[in] setup - setup message
 * @return true on success, else false
 */
bool USBDeviceClass::handleClassInterfaceSetup(USBSetup & setup) {
	bool res = PluggableUSB().setup(setup);
	if (res == false || setup.bmRequestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {
		this->sendZlp(0);
	}
	return res;
}


/**
 * Handles the standard setup request.
 * 
 * @param[in] setup - setup message
 * @return true on success, else false
 */
bool USBDeviceClass::handleStandardSetup(USBSetup & setup) {
	const uint16_t wValue = uint16_t((uint16_t(setup.wValueH) << 8) | setup.wValueL);
	const uint8_t ep = uint8_t(setup.wIndex & 0xFF);
	switch (setup.bRequest) {
	case GET_STATUS:
		switch (setup.bmRequestType & REQUEST_RECIPIENT) {
		case REQUEST_DEVICE:
			/* send the device status */
			ctrlStatBuf[0] = 0;
			ctrlStatBuf[1] = 0;
			this->sendControl(ctrlStatBuf, 2);
			break;
		case REQUEST_ENDPOINT:
			/* send the endpoint status and check if the endpoint is currently halted */
			ctrlStatBuf[0] = isEndpointHalt ? 1 : 0;
			ctrlStatBuf[1] = 0;
			this->sendControl(ctrlStatBuf, 2);
			break;
		default:
			return false;
		}
		break;
	case CLEAR_FEATURE:
		/* remove selected feature */
		switch (wValue) {
		case ENDPOINT_HALT:
			isEndpointHalt = false;
			if ((ep & 0xF) != 0) {
				HAL_PCD_EP_ClrStall(hPcdUsb, ep);
			}
			this->sendZlp(0);
			break;
		case DEVICE_REMOTE_WAKEUP:
			/* disable remote wake-up and send a ZLP */
			HAL_PCD_DeActivateRemoteWakeup(hPcdUsb);
			ctrlStatBuf[0] = isEndpointHalt ? 1 : 0;
			ctrlStatBuf[1] = 0;
			this->sendControl(ctrlStatBuf, 2);
			break;
		default:
			break; /* silently ignored */
		}
		break;
	case SET_FEATURE:
		/* add selected feature */
		switch (wValue) {
		case ENDPOINT_HALT:
			/* halt endpoint */
			isEndpointHalt = true;
			if ((ep & 0xF) != 0 && setup.wLength == 0) {
				HAL_PCD_EP_SetStall(hPcdUsb, ep);
			}
			this->sendZlp(0);
			break;
		case DEVICE_REMOTE_WAKEUP:
			/* enable remote wake-up and send a ZLP */
			isRemoteWakeUpEnabled = true;
			HAL_PCD_ActivateRemoteWakeup(hPcdUsb);
			ctrlStatBuf[0] = 0;
			this->sendControl(ctrlStatBuf, 1);
			break;
		default:
			break; /* silently ignored */
		}
		break;
	case SET_ADDRESS:
		this->setAddress(setup.wValueL);
		this->sendZlp(0);
		break;
	case GET_DESCRIPTOR:
		return this->sendDescriptor(setup);
	case SET_DESCRIPTOR:
		return false;
	case GET_CONFIGURATION:
		ctrlStatBuf[0] = _usbConfiguration;
		this->sendControl(ctrlStatBuf, 1);
		break;
	case SET_CONFIGURATION:
		if ((setup.bmRequestType & REQUEST_RECIPIENT) == REQUEST_DEVICE) {
			this->initEndpoints();
			_usbConfiguration = setup.wValueL; /* bConfigurationValue */
			this->sendZlp(0);
		} else {
			return false;
		}
		break;
	case GET_INTERFACE:
		ctrlStatBuf[0] = _usbSetInterface;
		this->sendControl(ctrlStatBuf, 1);
		break;
	case SET_INTERFACE:
		_usbSetInterface = setup.wValueL;
		this->sendZlp(0);
		break;
	default:
		return false;
	}
	return true;
}


/**
 * Sends out the device descriptor for the request from given setup.
 * 
 * @param[in] setup - setup message
 * @return true on success, else false
 */
bool USBDeviceClass::sendDescriptor(USBSetup & setup) {
	const uint8_t * descAddr = NULL;
	uint8_t descLength = 0;

	if (setup.wValueH == USB_CONFIGURATION_DESCRIPTOR_TYPE) {
		return USBDevice.sendConfiguration(setup.wLength);
	}

#ifdef PLUGGABLE_USB_ENABLED
	const int res = PluggableUSB().getDescriptor(setup);
	if (res != 0) {
		return (res > 0 ? true : false);
	}
#endif /* PLUGGABLE_USB_ENABLED */

	switch (setup.wValueH) {
	case USB_DEVICE_DESCRIPTOR_TYPE:
		descAddr = reinterpret_cast<const uint8_t *>(&USB_DeviceDescriptorIAD);
		descLength = *descAddr;
		/* sent at the end of the function */
		break;
	case USB_STRING_DESCRIPTOR_TYPE:
		switch (setup.wValueL) {
		case 0:
			descAddr = reinterpret_cast<const uint8_t *>(&STRING_LANGUAGE);
			descLength = *descAddr;
			/* sent at the end of the function */
			break;
		case IPRODUCT:
			return this->sendStringDescriptor(STRING_PRODUCT, setup.wLength);
		case IMANUFACTURER:
			return this->sendStringDescriptor(STRING_MANUFACTURER, setup.wLength);
		case ISERIAL:
			{
				char name[ISERIAL_MAX_LEN] = {0};
				uint8_t len = 0;
#ifdef UID_BASE
				const uint32_t * uidPtr = reinterpret_cast<const uint32_t *>(UID_BASE);
				const uint32_t uid = uidPtr[0] ^ uidPtr[1] ^ uidPtr[2];
				for (; len < 8; len++) {
					name[len] = "0123456789ABCDEF"[(uid >> (4 * len)) & 0xF];
				}
#endif /* UID_BASE */
#ifdef PLUGGABLE_USB_ENABLED
				len = uint8_t(len + PluggableUSB().getShortName(name + len));
#endif /* PLUGGABLE_USB_ENABLED */
				if (len > 0) {
					return this->sendStringDescriptor(reinterpret_cast<const uint8_t *>(name), setup.wLength);
				}
			}
			return false;
		default:
			return false;
		}
		break;
	default:
		return false;
	}
	
	this->sendControl(descAddr, min(descLength, setup.wLength));
	return true;
}


/**
 * Sends the given data on the control endpoint.
 * 
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @return processed data length
 */
uint32_t USBDeviceClass::sendControl(const void * data, uint32_t len) {
	if ( _dry_run ) return len;
	if ( _pack_message ) {
		memcpy(_pack_buffer + _pack_size, data, len);
		_pack_size = uint16_t(_pack_size + len);
		return len;
	}
	sendOrBlock(USB_ENDPOINT_IN(0), data, len);
	return len;
}


/**
 * Received data from the control endpoint. This function blocks until
 * some data has been received.
 * 
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @return received data length
 */
uint32_t USBDeviceClass::recvControl(void * data, uint32_t len) {
	const uint32_t receivedLen = this->available(USB_ENDPOINT_OUT(0));
	const uint32_t received = min(len, receivedLen);
	if (received > 0) {
		memcpy(data, _usbCtrlRecvBuf, size_t(received));
	}
	return received;
}


/**
 * Construct and sends a dynamic configuration descriptor to the host.
 * 
 * @param[in] maxLen - requested maximum send size
 * @return true on success, else false
 */
bool USBDeviceClass::sendConfiguration(uint32_t maxLen) {
	/* get the total size for the interface configuration */
	_dry_run = true;
	uint32_t total = 0;
	const uint8_t interfaces = this->SendInterfaces(&total);
	const ConfigDescriptor config = D_CONFIG(uint16_t(sizeof(ConfigDescriptor) + total), interfaces);
	_dry_run = false;

	/* send the actual configuration */
	if (maxLen == sizeof(ConfigDescriptor)) {
		this->sendControl(&config, sizeof(ConfigDescriptor));
		return true;
	}
	total = 0;
	this->packMessages(true);
	this->sendControl(&config, sizeof(ConfigDescriptor));
	this->SendInterfaces(&total);
	/* configuration descriptor is sent complete without any truncation to maxLen */
	this->packMessages(false);

	return true;
}


/**
 * Sends the USB descriptor string. This function internally performs the
 * ASCII to UTF-16 conversion which is needed here.
 * 
 * @param[in] string - USB descriptor string
 * @param[in] maxLen - requested maximum send size
 * return true on success, else false
 */
bool USBDeviceClass::sendStringDescriptor(const uint8_t * string, uint32_t maxLen) {
	if (maxLen < 2) return false;
	if (maxLen == 2) {
		ctrlStatBuf[0] = 2; /* bLength */
		ctrlStatBuf[1] = 0x03; /* bDescriptorType: String */
		this->sendControl(ctrlStatBuf, 2);
		return true;
	}
	size_t outLen = (strlen(reinterpret_cast<const char *>(string)) + 1) * 2;
	if (outLen > maxLen) outLen = maxLen;
	/* reuse _pack_buffer to avoid additional memory allocations */
	if (outLen > sizeof(_pack_buffer)) outLen = sizeof(_pack_buffer);
	_pack_buffer[0] = uint8_t(outLen); /* bLength */
	_pack_buffer[1] = 0x03; /* bDescriptorType: String */
	/* bString: */
	for (size_t i = 2, j = 0; (i + 1) < outLen; j++) {
		_pack_buffer[i++] = string[j];
		_pack_buffer[i++] = 0;
	}
	return this->sendControl(_pack_buffer, outLen) == outLen;
}


uint8_t USBDeviceClass::SendInterfaces(uint32_t * total) {
	uint8_t interfaces = 0;
#ifdef PLUGGABLE_USB_ENABLED
	total[0] += PluggableUSB().getInterface(&interfaces);
#endif /* PLUGGABLE_USB_ENABLED */
	return interfaces;
}


/**
 * Activate or deactivate message packing. Deactivating this
 * immediately transfers all previously packed bytes.
 * 
 * @param[in] val - true for activation, else false
 */
void USBDeviceClass::packMessages(bool val) {
	if ( val ) {
		_pack_message = true;
		_pack_size = 0;
	} else {
		_pack_message = false;
		this->sendControl(_pack_buffer, _pack_size);
	}
}


/**
 * Initializes and starts all non-control endpoints with their associated configuration.
 */
void USBDeviceClass::initEndpoints(void) {
	for (size_t i = 1; i < USB_ENDPOINTS && _usbEndpoints[i] != 0; i++) {
		this->initEP(i, _usbEndpoints[i]);
	}
}


/**
 * Initializes and starts the given endpoint with the passed configuration.
 * 
 * @param[in] ep - endpoint number
 * @param[in] config - endpoint configuration
 */
void USBDeviceClass::initEP(uint32_t ep, uint32_t config) {
	const uint8_t epId = uint8_t((config & USB_ENDPOINT_DIRECTION_MASK) | ep);
	uint8_t epType = 0xFF; /* initialize with invalid type value */
	switch (config & USB_ENDPOINT_TYPE_MASK) {
	case USB_ENDPOINT_TYPE_CONTROL:     epType = PCD_EP_TYPE_CTRL; break;
	case USB_ENDPOINT_TYPE_ISOCHRONOUS: epType = PCD_EP_TYPE_ISOC; break;
	case USB_ENDPOINT_TYPE_BULK:        epType = PCD_EP_TYPE_BULK; break;
	case USB_ENDPOINT_TYPE_INTERRUPT:   epType = PCD_EP_TYPE_INTR; break;
	default: break;
	}
	if (epType == 0xFF || ep >= USB_ENDPOINTS) {
		systemErrorHandler();
		return;
	}
#ifdef PCD_SNG_BUF
	const uint8_t epIdx = (ep == 0 && (config & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_OUT(0)) ? 0 : uint8_t(ep + 1);
	/* we need space for the BTABLE at the beginning of the PMA memory (16 byte per endpoint number with IN/OUT); EP buffer needs to be 32 byte aligned */
	/* note that HAL internally calculates the PMA address as USB_PMAADDR + (offset * 2) */
	HAL_PCDEx_PMAConfig_Wrapper(hPcdUsb, epId, PCD_SNG_BUF, (epIdx * USB_EP_SIZE) + (16 * USB_ENDPOINTS));
#endif /* PCD_SNG_BUF */
	if ((config & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_IN(0)) {
		/* ED TX FIFO size needs to be at least 64 bytes and to be a multiple of 4 */
		HAL_PCDEx_SetTxFiFo_Wrapper(hPcdUsb, uint8_t(ep), uint16_t((USB_EP_SIZE < 64) ? 64 : (uint16_t((USB_EP_SIZE + 3) / 4) * 4)));
	}
	HAL_PCD_EP_Open(hPcdUsb, epId, uint16_t(USB_EP_SIZE), epType);
	if (ep > 0) {
		const uint16_t epMask = uint16_t(1 << uint16_t(ep));
		if ((config & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_OUT(0)) {
			_UsbRxBuffer * & buf = reinterpret_cast<_UsbRxBuffer **>(_usbBuf)[ep - 1];
			if (buf == NULL) {
				buf = new _UsbRxBuffer;
			} else {
				buf->fifo.clear();
			}
			rxPendingEp &= uint16_t(~epMask); /* clear bit */
			recvOrFail(uint8_t(ep), buf->packet, _UsbRxBuffer::PacketSize);
		} else {
			_UsbTxBuffer * & buf = reinterpret_cast<_UsbTxBuffer **>(_usbBuf)[ep - 1];
			if (buf == NULL) {
				buf = new _UsbTxBuffer;
			} else {
				buf->fifo.clear();
			}
			txPendingEp &= uint16_t(~epMask); /* clear bit */
			buf->commitLock = false;
		}
	}
}


/**
 * Send data to an endpoint. Block until the transmission finished.
 * 
 * @param[in] ep - endpoint number
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @return bytes transmitted
 */
uint32_t USBDeviceClass::send(uint32_t ep, const void * data, uint32_t len) {
	return sendHelper(TRANSFER_RELEASE, ep, data, len);
}


/**
 * Sends a zero length packet.
 * 
 * @param[in] ep - endpoint number
 * @remarks This acts as an acknowledgment on transaction level.
 */
void USBDeviceClass::sendZlp(uint32_t ep) {
	sendOrBlock(USB_ENDPOINT_IN(ep), NULL, 0);
}


/**
 * Non-blocking receive.
 * 
 * @param[in] ep - endpoint number
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @return Number of bytes copied to the passed data buffer.
 * @remarks uint32_t(-1) is returned if no USB device is connected for compatibility reasons.
 */
uint32_t USBDeviceClass::recv(uint32_t ep, void * data, uint32_t len) {
	if ( ! _usbConfiguration ) return uint32_t(-1);
	const uint8_t epNum = uint8_t(ep & 0xF);
	if (usbRxBuffer(epNum) == NULL) {
		/* no buffering installed -> perform blocking read */
		if ( ! recvOrFail(USB_ENDPOINT_OUT(ep), data, len, true) ) return 0;
		return this->available(ep);
	} else {
		_UsbRxBuffer & buf = *usbRxBuffer(epNum);
		/* copy data from FIFO to output buffer */
		uint32_t copied = buf.fifo.read(static_cast<uint8_t *>(data), len);
		const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
		if ((rxPendingEp & epMask) != 0) return copied; /* no more data is available, yet */
		/* copy data from received packet to output buffer */
		const uint8_t epIdx = (epNum == 0) ? 0 : uint8_t(epNum + 1);
		uint8_t * recvBuf = bufferPtrEp[epIdx];
		uint32_t received = bytesPendingEp[epIdx];
		uint8_t * outBuf = static_cast<uint8_t *>(data) + copied;
		while (received > 0 && copied < len) {
			*outBuf = *recvBuf;
			outBuf++;
			copied++;
			recvBuf++;
			received--;
		}
		/* copy data from received packet to FIFO */
		const uint32_t written = buf.fifo.write(recvBuf, received);
		/* request next packet */
		if (written >= received) {
			recvOrFail(uint8_t(ep), buf.packet, _UsbRxBuffer::PacketSize);
		} else {
			bufferPtrEp[epIdx] = recvBuf + written;
			bytesPendingEp[epIdx] = uint32_t(received - written);
		}
		return copied;
	}
}


/**
 * Receive one byte if available.
 * 
 * @param[in] ep - endpoint number
 * @return received byte or -1 if unavailable
 */
int USBDeviceClass::recv(uint32_t ep) {
	uint8_t c;
	if (this->recv(ep, &c, 1) != 1) return -1;
	return int(c);
}


/**
 * Returns how many bytes have been received on the given endpoint.
 * 
 * @param[in] ep - endpoint
 * @return received bytes count
 */
uint32_t USBDeviceClass::available(uint32_t ep) {
	const uint8_t epNum = uint8_t(ep & 0xF);
	if ((_usbEndpoints[epNum] & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_OUT(0)) {
		if (ep == 0 || usbRxBuffer(epNum) == NULL) {
			return uint32_t(bufferPtrEp[0] - _usbCtrlRecvBuf);
		}
		const _UsbRxBuffer & buf = *usbRxBuffer(epNum);
		return buf.fifo.availableForRead();
	}
	const uint16_t epMask = uint16_t(1 << uint16_t(ep & 0xF));
	if ((txPendingEp & epMask) != 0) return 0;
	return uint32_t(USB_EP_SIZE);
}


/**
 * Flush buffers of the given endpoint.
 * 
 * @param[in] ep - endpoint
 */
void USBDeviceClass::flush(uint32_t ep) {
	if (ep == 0) {
#ifndef PCD_SNG_BUF
		HAL_PCD_EP_Flush(hPcdUsb, USB_ENDPOINT_IN(0));
		HAL_PCD_EP_Flush(hPcdUsb, USB_ENDPOINT_OUT(0));
#endif /* not PCD_SNG_BUF */
		txPendingEp &= uint16_t(~1);
		rxPendingEp &= uint16_t(~1);
		bytesPendingEp[0] = 0;
		bytesPendingEp[1] = 0;
	} else {
		bool canWait = false;
		const bool interruptsEnabled = ((__get_PRIMASK() & 0x1) == 0);
		if ( interruptsEnabled ) {
			const uint32_t irqExecutionNumber = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;
			if (irqExecutionNumber == 0 || NVIC_GetPriority(IRQn_Type(irqExecutionNumber - 16)) > usbNvicGetLowestPriority()) {
				/* USB interrupt is enabled and we were not called from an interrupt with higher priority */
				canWait = true;
			}
		}
		if ( ! canWait ) return;
		const uint16_t epMask = uint16_t(1 << uint16_t(ep & 0xF));
		if ((_usbEndpoints[ep] & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_IN(0)) {
			const uint8_t epNum = uint8_t(ep & 0xF);
			if (usbTxBuffer(epNum) != NULL) {
				_UsbTxBuffer & buf = *usbTxBuffer(epNum);
				if ((txPendingEp & epMask) == 0) {
					/* trigger send in case something clogged up */
					buf.fifo.commitBlock();
					usbTriggerSend(buf, epNum, false);
				}
			}
			const uint32_t startTime = millis();
			while ((txPendingEp & epMask) != 0) {
				__WFI();
				if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) break;
			}
		} else {
			const uint32_t startTime = millis();
			while ((rxPendingEp & epMask) != 0) {
				__WFI();
				if (uint32_t(millis() - startTime) >= USB_WFI_TIMEOUT_MS) break;
			}
		}
	}
}


/**
 * Free any outstanding data.
 * 
 * @param[in] ep - endpoint
 */
void USBDeviceClass::clear(uint32_t ep) {
	const uint32_t isIn __attribute__((unused)) = ((_usbEndpoints[ep] & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_IN(0)) ? 1U : 0U;
#ifdef PCD_SET_EP_TX_CNT
	if ( isIn ) {
		PCD_SET_EP_TX_CNT(hPcdUsb->Instance, uint8_t(ep & 0xF), 0);
	}
#endif /* PCD_SET_EP_TX_CNT */
#ifdef PCD_SET_EP_RX_CNT
	if ( ! isIn ) {
		PCD_SET_EP_RX_CNT(hPcdUsb->Instance, uint8_t(ep & 0xF), 0);
	}
#endif /* PCD_SET_EP_RX_CNT */
	if (ep == 0) {
#ifdef PCD_FreeUserBuffer
		PCD_FreeUserBuffer(hPcdUsb->Instance, uint8_t(ep), 0U);
		PCD_FreeUserBuffer(hPcdUsb->Instance, uint8_t(ep), 1U);
#endif /* PCD_FreeUserBuffer */
		txPendingEp &= uint16_t(~1); /* clear bit */
		rxPendingEp &= uint16_t(~1); /* clear bit */
	} else {
#ifdef PCD_FreeUserBuffer
		PCD_FreeUserBuffer(hPcdUsb->Instance, uint8_t(ep), isIn);
#endif /* PCD_FreeUserBuffer */
		const uint16_t epMask = uint16_t(~(1 << uint16_t(ep & 0xF)));
		if ((ep & USB_ENDPOINT_DIRECTION_MASK) == USB_ENDPOINT_IN(0)) {
			txPendingEp &= epMask; /* clear bit */
		} else {
			rxPendingEp &= epMask; /* clear bit */
		}
	}
}


/**
 * Signal an error to the host for the given endpoint.
 * 
 * @param[in] ep - endpoint
 */
void USBDeviceClass::stall(uint32_t ep) {
	if (ep == 0) {
		HAL_PCD_EP_SetStall(hPcdUsb, USB_ENDPOINT_IN(0));
		HAL_PCD_EP_SetStall(hPcdUsb, USB_ENDPOINT_OUT(0));
	} else {
		HAL_PCD_EP_SetStall(hPcdUsb, uint8_t(ep) | (_usbEndpoints[ep] & USB_ENDPOINT_DIRECTION_MASK));
	}
}


/**
 * USB device class instance.
 */
USBDeviceClass USBDevice;


/* Arduino legacy functions */
/**
 * Sends the given data on the control endpoint.
 * 
 * @param[in] flags - `TRANSFER_ZERO`, `TRANSFER_RELEASE` and/or `TRANSFER_PGM`
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @return processed data length
 */
int USB_SendControl(uint8_t /* flags */, const void * data, int len) {
	return int(USBDevice.sendControl(data, len));
}


/**
 * Received data from the control endpoint. This function blocks until
 * some data has been received.
 * 
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @return received data length
 */
int USB_RecvControl(void * data, int len) {
	return int(USBDevice.recvControl(data, len));
}


/**
 * Received data from the control endpoint. This function blocks until
 * some data has been received.
 * 
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @return received data length
 */
int USB_RecvControlLong(void * data, int len) {
	return int(USBDevice.recvControl(data, len));
}


/**
 * Returns how many bytes have been received on the given endpoint.
 * 
 * @param[in] ep - endpoint
 * @return received bytes count
 */
uint8_t USB_Available(uint8_t ep) {
	const uint32_t available = USBDevice.available(ep);
	return uint8_t(min(255, available));
}


/**
 * Returns how many bytes can be written to the given endpoint
 * without blocking.
 * 
 * @param[in] ep - endpoint
 * @return number of bytes available for write
 */
uint8_t USB_SendSpace(uint8_t ep) {
	const uint16_t epMask = uint16_t(1 << uint16_t(ep & 0xF));
	if ((txPendingEp & epMask) != 0) {
		const uint8_t epNum = ep & 0xF;
		if (epNum == 0 || usbTxBuffer(epNum) == NULL) {
			return 0;
		} else {
			_UsbTxBuffer & buf = *usbTxBuffer(epNum);
			const uint32_t available = buf.fifo.availableForWrite();
			return uint8_t(min(255, available));
		}
	}
	return uint8_t(min(255, USB_EP_SIZE));
}


/**
 * Send data to an endpoint. Block until the transmission finished.
 * 
 * @param[in] ep - endpoint number; can be combined with `TRANSFER_ZERO`, `TRANSFER_RELEASE` and/or `TRANSFER_PGM`
 * @param[in] data - data buffer
 * @param[in] len - data length
 * @return bytes transmitted
 */
int USB_Send(uint8_t ep, const void * data, int len) {
	return int(sendHelper(ep & 0xF0, ep & 0xF, data, len));
}


/**
 * Non-blocking receive.
 * 
 * @param[in] ep - endpoint number
 * @param[out] data - data buffer
 * @param[in] len - buffer size
 * @return Number of bytes copied to the passed data buffer.
 * @remarks -1 is returned if no USB device is connected for compatibility reasons.
 */
int USB_Recv(uint8_t ep, void * data, int len) {
	return int(USBDevice.recv(ep, data, len));
}


/**
 * Receive one byte if available.
 * 
 * @param[in] ep - endpoint number
 * @return received byte or -1 if unavailable
 */
int USB_Recv(uint8_t ep) {
	return USBDevice.recv(ep);
}


/**
 * Flush buffers of the given endpoint.
 * 
 * @param[in] ep - endpoint
 */
void USB_Flush(uint8_t ep) {
	USBDevice.flush(ep);
}


extern "C" {
void STM32CubeDuinoIrqHandlerForUSB_OTG_FS(void) {
	HAL_PCD_IRQHandler(hPcdUsb);
}

void STM32CubeDuinoIrqHandlerForUSB(void) {
	HAL_PCD_IRQHandler(hPcdUsb);
}


/* implementation of STM32 HAL related callback hooks */
/**
 * Overwrites the STM32 HAL API handler for PCD MSP initialization request.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef * /* hPcd */) {
	/* activate clock of the associated port */
#ifdef ACTIVATE_USB_PORT
	/* possible implementation to force a USB enumeration, e.g. by pulling down D+ */
	{ ACTIVATE_USB_PORT(); }
#endif /* ACTIVATE_USB_PORT */
	/* USB FS is always on PA_11 (D-) and PA_12 (D+) */
#if defined(GPIO_AF10_OTG1_FS) || defined(GPIO_AF10_OTG2_FS) || defined(GPIO_AF10_OTG_FS) || defined(GPIO_AF10_USB_FS)
	pinModeEx(PA_11, ALTERNATE_FUNCTION, 10);
	pinModeEx(PA_12, ALTERNATE_FUNCTION, 10);
#elif defined(GPIO_AF14_USB)
	pinModeEx(PA_11, ALTERNATE_FUNCTION, 14);
	pinModeEx(PA_12, ALTERNATE_FUNCTION, 14);
#elif defined(__HAL_RCC_GPIOA_CLK_ENABLE)
	{ __HAL_RCC_GPIOA_CLK_ENABLE(); }
#endif /* __HAL_RCC_GPIOA_CLK_ENABLE */
	/* enable other clocks */
#ifdef HAL_REMAP_PA11_PA12
	/* remap pins to use USB */
	{ __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12); }
#endif /* HAL_REMAP_PA11_PA12 */
#ifdef __HAL_RCC_USB_OTG_FS_CLK_ENABLE
	{ __HAL_RCC_USB_OTG_FS_CLK_ENABLE(); }
#elif defined(__HAL_RCC_USB_CLK_ENABLE)
	{ __HAL_RCC_USB_CLK_ENABLE(); }
#endif
	/* enable interrupt */
	usbNvicSetPriority();
	usbNvicEnable();
}


/**
 * Overwrites the STM32 HAL API handler for PCD MSP de-initialization request.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef * /* hPcd */) {
	/* disable clock */
#ifdef __HAL_RCC_USB_OTG_FS_CLK_DISABLE
	__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
#elif defined(__HAL_RCC_USB_CLK_DISABLE)
	__HAL_RCC_USB_CLK_DISABLE();
#endif
	/* disable interrupt */
	usbNvicDisable();
}


/**
 * Handles the low level setup event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @param[in] pcdSetup - true if called from HAL_PCD_SetupStageCallback, else false
 */
static void handleLowLevelSetup(PCD_HandleTypeDef * hPcd, const bool pcdSetup) {
	/* retrieve received endpoint #0 setup from buffer */
	/* note: this works because all STM32 MCUs are little-endian */
	USBSetup & setup = *reinterpret_cast<USBSetup *>(hPcd->Setup);
	bool ok = false;
	/* handle additional data */
	if ((setup.bmRequestType & REQUEST_DIRECTION) == REQUEST_HOSTTODEVICE && setup.wLength > 0) {
		if ( pcdSetup ) {
			/* additional data is associated to the setup -> receive it before proceeding */
			if (setup.wLength > sizeof(_usbCtrlRecvBuf)) goto onError;
			if ( ! (ok = recvOrFail(USB_ENDPOINT_OUT(0), _usbCtrlRecvBuf, setup.wLength)) ) {
				goto onError;
			}
			return;
		}
		/* the additional data was received -> continue with the actual setup */
		/* note: the additional data can be retrieved via USBDevice.recvControl() for compatibility reasons */
	}
	switch (setup.bmRequestType & REQUEST_TYPE) {
	case REQUEST_STANDARD:
		ok = USBDevice.handleStandardSetup(setup);
		break;
	case REQUEST_CLASS:
	case REQUEST_VENDOR:
		ok = USBDevice.handleClassInterfaceSetup(setup);
		break;
	default:
		break;
	}
onError:
	if ( ! ok ) {
		USBDevice.stall(0);
		/* remove stall request (caution: this may reset the DATA toggle bit!) */
		HAL_PCD_EP_ClrStall(hPcdUsb, USB_ENDPOINT_IN(0));
		HAL_PCD_EP_ClrStall(hPcdUsb, USB_ENDPOINT_OUT(0));
	}
}


/**
 * Overwrites the STM32 HAL API handler for PCD setup event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef * hPcd) {
	rxPendingEp &= uint16_t(~1); /* clear bit */
	handleLowLevelSetup(hPcd, true);
}


/**
 * Overwrites the STM32 HAL API handler for PCD receive complete event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @param[in] ep - endpoint
 * @remarks host to device packets
 */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef * /* hPcd */, uint8_t ep) {
	/* remarks: xfer_count and xfer_buff are updated by STM32 HAL PCD; xfer_len may get changed */
	const uint8_t epNum = uint8_t(ep & 0xF);
	const uint8_t epIdx = (epNum == 0) ? 0 : uint8_t(epNum + 1);
	const uint32_t received = HAL_PCD_EP_GetRxCount(hPcdUsb, ep);
	uint8_t * recvBuf = bufferPtrEp[epIdx];
	bool callSetup = false;
	if (epNum == 0) {
		if (bytesPendingEp[0] > received) {
			/* reception incomplete -> receive next packet */
			const uint32_t remaining = uint32_t(bytesPendingEp[0] - received);
			bytesPendingEp[0] = remaining;
			HAL_PCD_EP_Receive(hPcdUsb, ep, recvBuf, remaining);
			return;
		} else if (received > 0) {
			/* reception complete -> signal the host that we are ready to send again */
			callSetup = true;
		}
		bytesPendingEp[epIdx] = 0;
	} else if (received > 0) {
		if (epNum != 0 && usbRxBuffer(epNum) != NULL) {
			_UsbRxBuffer & buf = *usbRxBuffer(epNum);
			recvBuf = buf.packet; /* revert pointer */
			/* transfer packet data to upper layer FIFO */
			const uint32_t written = buf.fifo.write(recvBuf, received);
			/* re-queue read operation if the packet was completely transfered to the FIFO */
			if (written >= received) {
				bufferPtrEp[epIdx] = buf.packet;
				bytesPendingEp[epIdx] = _UsbRxBuffer::PacketSize;
				HAL_PCD_EP_Receive(hPcdUsb, ep, buf.packet, _UsbRxBuffer::PacketSize);
				return;
			} else {
				bufferPtrEp[epIdx] = recvBuf + written;
				bytesPendingEp[epIdx] = uint32_t(received - written);
			}
			/* else we will need to request more data as soon as the FIFO gets read */
			/* @see USBDeviceClass::recv() */
		} /* else this was a blocking read without an intermediate buffer -> signal completion */
	}
	const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
	rxPendingEp &= uint16_t(~epMask); /* clear bit */
	if ( callSetup ) handleLowLevelSetup(hPcdUsb, false); /* call last to avoid race condition for bytesPendingEp and rxPendingEp */
}


/**
 * Helper function for the transmission interrupt handler to handle the
 * next packet transmission from FIFO.
 * 
 * @param[in,out] buf - transmission FIFO buffer
 * @param[in] ep - IN endpoint
 * @param[in] epIdx - endpoint index
 * @return true if a packet was sent, else false
 */
static bool sendNextPacket(_UsbTxBuffer & buf, const uint8_t ep, const uint8_t epIdx) {
	uint32_t blockSize = 0;
	const uint8_t * blockPtr = buf.fifo.peek(blockSize);
	if (blockPtr != NULL) {
		/* send out complete block */
#if USB_TX_SIZE >= (USB_EP_SIZE * 4) && defined(USB_TX_TRANSACTIONAL)
		const _UsbTxBuffer::FifoType::IndexType nextTail = _UsbTxBuffer::FifoType::IndexType(buf.fifo.tail + 1);
		if (blockSize >= USB_EP_SIZE && nextTail < _UsbTxBuffer::FifoType::Count && nextTail != buf.fifo.head) {
			/* queue two blocks at once */
			const uint32_t blockSize2 = blockSize + buf.fifo.size[nextTail];
			bufferPtrEp[epIdx] = const_cast<uint8_t *>(blockPtr);
			bytesPendingEp[epIdx] = blockSize2;
			HAL_PCD_EP_Transmit(hPcdUsb, ep, bufferPtrEp[epIdx], USB_IO_TX_CHUNK(blockSize2));
			return true;
		}
#endif
		bufferPtrEp[epIdx] = const_cast<uint8_t *>(blockPtr);
		bytesPendingEp[epIdx] = blockSize;
		HAL_PCD_EP_Transmit(hPcdUsb, ep, bufferPtrEp[epIdx], USB_IO_TX_CHUNK(blockSize));
		return true;
	} else if ( ! (buf.commitLock || buf.fifo.totallyEmpty()) ) {
		/* send out partial complete current block */
#if USB_IRQ_PRIO > 0 || USB_IRQ_SUBPRIO > 0
		/* avoid FIFO manipulation from another IRQ */
		__disable_irq();
		buf.fifo.commitBlock();
		__enable_irq();
#else /* USB interrupts have the highest priority */
		buf.fifo.commitBlock();
#endif
		blockPtr = buf.fifo.peek(blockSize);
		bufferPtrEp[epIdx] = const_cast<uint8_t *>(blockPtr);
		bytesPendingEp[epIdx] = blockSize;
		HAL_PCD_EP_Transmit(hPcdUsb, ep, bufferPtrEp[epIdx], USB_IO_TX_CHUNK(blockSize));
		return true;
	}
	return false;
}


/**
 * Overwrites the STM32 HAL API handler for PCD send complete event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @param[in] ep - endpoint
 * @remarks device to host packets
 */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef * /* hPcd */, uint8_t ep) {
	const uint8_t epNum = uint8_t(ep & 0xF);
	const uint8_t epIdx = uint8_t(epNum + 1);
	const uint32_t transmitted = uint32_t(USB_IO_TX_CHUNK(bytesPendingEp[epIdx]));
	bufferPtrEp[epIdx] += transmitted;
	bool recvZlp = false;
	if (bytesPendingEp[epIdx] > transmitted) {
		/* transmission incomplete -> send next packet */
		const uint32_t remaining = uint32_t(bytesPendingEp[epIdx] - transmitted);
		bytesPendingEp[epIdx] = remaining;
		HAL_PCD_EP_Transmit(hPcdUsb, ep, bufferPtrEp[epIdx], USB_IO_TX_CHUNK(remaining));
		return;
	} else if (epNum == 0) {
		/* transmission complete -> signal the host that we are ready to receive again */
		/* STM32 USB library calls HAL_PCD_EP_SetStall(hPcdUsb, USB_ENDPOINT_IN(0)); at this point. This may result in an actual device stall. */
		recvZlp = true;
	} else if (transmitted > 0) {
		/* transmission complete */
		if (usbTxBuffer(epNum) != NULL) {
			/* process TX queue */
			_UsbTxBuffer & buf = *usbTxBuffer(epNum);
#if USB_TX_SIZE >= (USB_EP_SIZE * 4) && defined(USB_TX_TRANSACTIONAL)
			const uint8_t * blockEndPtr = buf.fifo.block[buf.fifo.tail + 1];
			/* free up both transmitted blocks */
			if (bufferPtrEp[epIdx] > blockEndPtr) buf.fifo.pop();
#endif
			buf.fifo.pop();
			if ( sendNextPacket(buf, ep, epIdx) ) return;
		}
		bytesPendingEp[epIdx] = 0;
		if ((transmitted % USB_EP_SIZE) == 0) {
			/* no more data to send; last packet had max endpoint size -> send ZLP to signal end of transaction */
			HAL_PCD_EP_Transmit(hPcdUsb, ep, NULL, 0);
			return;
		}
	}
	/* transmission completed */
	if (epNum != 0 && usbTxBuffer(epNum) != NULL) {
		/* process TX queue */
		_UsbTxBuffer & buf = *usbTxBuffer(epNum);
		if ( sendNextPacket(buf, ep, epIdx) ) return;
	}
	const uint16_t epMask = uint16_t(1 << uint16_t(epNum));
	txPendingEp &= uint16_t(~epMask); /* clear bit */
	if ( recvZlp ) {
		/* call last to avoid race condition in handleLowLevelSetup() */
		HAL_PCD_EP_Receive(hPcdUsb, USB_ENDPOINT_OUT(0), NULL, 0);
	}
}


/**
 * Overwrites the STM32 HAL API handler for PCD SOF event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @remarks Output of the SOF signal needs to be explicitly enabled at USB HAL initialization.
 */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef * /* hPcd */) {
	//uint32_t frameNumber = usbFrameNumber();
}


/**
 * Overwrites the STM32 HAL API handler for PCD reset event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef * /* hPcd */) {
	/* end of reset -> initialize control endpoints */
	txPendingEp = 0;
	rxPendingEp = 0;
	/* RX FIFO needs to be defined before TX FIFO because HAL calculates the TX FIFO offset from the RX FIFO size */
	HAL_PCDEx_SetRxFiFo_Wrapper(hPcdUsb, uint16_t(((USB_EP_SIZE < 64) ? 64 : (uint16_t((USB_EP_SIZE + 3) / 4) * 4)) * 2));
	USBDevice.initEP(0, USB_ENDPOINT_TYPE_CONTROL | USB_ENDPOINT_OUT(0)); /* initialize OUT first! */
	USBDevice.initEP(0, USB_ENDPOINT_TYPE_CONTROL | USB_ENDPOINT_IN(0));
	isRemoteWakeUpEnabled = false;
	isEndpointHalt = false;
	_usbConfiguration = 0;
}


/**
 * Overwrites the STM32 HAL API handler for PCD suspend event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef * /* hPcd */) {
	/* nothing to be done */
}


/**
 * Overwrites the STM32 HAL API handler for PCD resume event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef * /* hPcd */) {
	/* nothing to be done */
}


/**
 * Overwrites the STM32 HAL API handler for PCD ISO receive incomplete event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @param[in] epNum - endpoint number
 */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef * /* hPcd */, uint8_t /* epNum */) {
	// @todo
}


/**
 * Overwrites the STM32 HAL API handler for PCD ISO send incomplete event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 * @param[in] epNum - endpoint number
 */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef * /* hPcd */, uint8_t /* epNum */) {
	// @todo
}


/**
 * Overwrites the STM32 HAL API handler for PCD connect event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef * /* hPcd */) {
	/* nothing to be done */
}


/**
 * Overwrites the STM32 HAL API handler for PCD disconnect event.
 * 
 * @param[in,out] hPcd - pointer to PCD handle
 */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef * /* hPcd */) {
	/* nothing to be done */
}
} /* extern "C" */


#endif /* USBCON */
