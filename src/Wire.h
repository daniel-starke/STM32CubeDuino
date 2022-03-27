/**
 * @file Wire.h
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-11-30
 * @version 2022-03-11
 */
#ifndef __TWOWIRE_H__
#define __TWOWIRE_H__

#include "Stream.h"
#include "scdinternal/fifo.h"


#if !defined(STM32CUBEDUINO_DISABLE_I2C) && defined(IS_I2C_ADDRESSING_MODE) /* STM32 HAL I2C header was included */


#ifndef TWOWIRE_RX_BUFFER_SIZE
#define TWOWIRE_RX_BUFFER_SIZE 32
#endif
#ifndef TWOWIRE_TX_BUFFER_SIZE
#define TWOWIRE_TX_BUFFER_SIZE 32
#endif


/** `WIRE_HAS_END` means Wire has `end()` **/
#define WIRE_HAS_END 1

/** `WIRE_HAS_TIMEOUT` means Wire has `setWireTimeout()`, `getWireTimeoutFlag()` and `clearWireTimeout()`. */
#define WIRE_HAS_TIMEOUT 1
#define WIRE_DEFAULT_TIMEOUT 0
#define WIRE_DEFAULT_RESET_WITH_TIMEOUT false


class TwoWire : public Stream {
private:
	friend void HAL_I2C_AddrCallback(I2C_HandleTypeDef *, uint8_t, uint16_t);
	friend void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *);
	friend void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *);
	friend void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *);
	friend void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *);
protected:
	typedef typename _FifoIndexTypeFor<TWOWIRE_RX_BUFFER_SIZE>::type rx_buffer_index_t;
	typedef typename _FifoIndexTypeFor<TWOWIRE_TX_BUFFER_SIZE>::type tx_buffer_index_t;
	enum TargetMode {
		TARGET_MODE_LISTEN,
		TARGET_MODE_RECEIVE,
		TARGET_MODE_TRANSMIT
	};
	I2C_HandleTypeDef handle[1];
	IRQn_Type irqEv;
	IRQn_Type irqEr;
	uint8_t pins[2];
	uint8_t afns;
	uint8_t recv[1];
	uint8_t txAddress;
	bool isController;
	bool transmitting;
	bool initialized;
	bool timeoutFlag;
	volatile TargetMode targetMode;
	uint32_t timeout;
	HAL_StatusTypeDef (* lastInitFn)(I2C_HandleTypeDef * hI2c);
	rx_buffer_index_t rxTail;
	volatile rx_buffer_index_t rxHead;
	tx_buffer_index_t txBufferSize;
	uint8_t rxBuffer[TWOWIRE_RX_BUFFER_SIZE];
	uint8_t txBuffer[TWOWIRE_TX_BUFFER_SIZE];
	void (*onRequestCallback)(void);
	void (*onReceiveCallback)(int);
public:
	/** Possible return values for `endTransmission()`. */
	enum {
		I2C_OK = 0,
		I2C_DATA_TOO_LONG = 1,
		I2C_NACK_ADDR = 2,
		I2C_NACK_DATA = 3,
		I2C_ERROR = 4
	};
public:
	TwoWire(I2C_TypeDef * instance, const IRQn_Type irqEvNum, const IRQn_Type irqErNum, const PinName sclPin, const PinName sdaPin, const uint8_t sclAltFn, const uint8_t sdaAltFn);
	virtual ~TwoWire();
	
	inline void begin() { this->begin(false, TwoWire::init); }
	inline void begin(const bool enableGeneralCall) { this->begin(enableGeneralCall, TwoWire::init); }
	inline void begin(const uint8_t ownAddress) { this->begin(ownAddress, false, TwoWire::init); }
	inline void begin(const uint8_t ownAddress, const bool enableGeneralCall) { this->begin(ownAddress, enableGeneralCall, TwoWire::init); }
	void begin(const bool, HAL_StatusTypeDef (& initFn)(I2C_HandleTypeDef * hI2c)); /* STM32 specific */
	void begin(const uint8_t ownAddress, const bool enableGeneralCall, HAL_StatusTypeDef (& initFn)(I2C_HandleTypeDef * hI2c)); /* STM32 specific */
	void end();
	
	void setClock(const uint32_t clock);
	void setWireTimeout(const uint32_t newTimeout = WIRE_DEFAULT_TIMEOUT, const bool resetWithTimeout = WIRE_DEFAULT_RESET_WITH_TIMEOUT);
	
	bool getWireTimeoutFlag() const { return this->timeoutFlag; }
	void clearWireTimeoutFlag() { this->timeoutFlag = false; }
	
	void beginTransmission(const uint8_t address);
	
	uint8_t endTransmission();
	uint8_t endTransmission(const uint8_t sendStop);
	
	uint8_t requestFrom(const uint8_t address, const uint8_t length);
	uint8_t requestFrom(const uint8_t address, const uint8_t length, const uint8_t sendStop);
	uint8_t requestFrom(const uint8_t address, uint8_t length, const uint32_t iaddress, uint8_t isize, const uint8_t sendStop);
	
	virtual int available(void);
	virtual int availableForWrite(void);
	virtual int read(void);
	virtual int peek(void);
	virtual void flush(void);
	virtual size_t write(const uint8_t data);
	virtual size_t write(const uint8_t * data, const size_t length);

	void onReceive(void (* function)(int));
	void onRequest(void (* function)(void));
	
	using Print::write;
protected:
	static HAL_StatusTypeDef init(I2C_HandleTypeDef * hI2c);
	void onReceiveService();
	void onRequestService();
};


/* available if defined in board.cpp or by the user */
extern TwoWire Wire;
extern TwoWire Wire1;
extern TwoWire Wire2;
extern TwoWire Wire3;
extern TwoWire Wire4;
extern TwoWire Wire5;


#endif /* IS_I2C_ADDRESSING_MODE */
#endif /* __TWOWIRE_H__ */
