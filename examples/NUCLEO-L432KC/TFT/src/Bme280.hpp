/**
 * @file Bme280.hpp
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-07-02
 * @version 2022-03-20
 * 
 * BME280 sensor library.
 */
#ifndef __BME280_HPP__
#define __BME280_HPP__

#include <Arduino.h>
#include <Wire.h>


/**
 * Class for reading a BME280 sensor module via I2C.
 * 
 * @warning Be aware that a self-heating may occur at the sensor increasing the reading by 1-2
 * degrees Celsius.
 */
class Bme280 {
public:
	/** Possible sampling rates. */
	enum Sampling {
		SAMPLING_NONE   = 0,
		SAMPLING_X1     = 1,
		SAMPLING_X2     = 2,
		SAMPLING_X4     = 3,
		SAMPLING_X8     = 4,
		SAMPLING_X16    = 5
	};
	/** Possible device operation modes. */
	enum Mode {
		MODE_SLEEP      = 0, /**< Sleep mode. */
		MODE_FORCED     = 1, /**< Manual update mode. */
		MODE_NORMAL     = 3  /**< Frequent update mode. */
	};
	/** Value filter rate. */
	enum Filter {
		FILTER_OFF      = 0,
		FILTER_X2       = 1,
		FILTER_X4       = 2,
		FILTER_X8       = 3,
		FILTER_X16      = 4
	};
	/** Standby duration after each automatic update. Only valid in MODE_NORMAL. */
	enum Standby {
		STANDBY_MS_0_5  = 0, /**<    0.5ms */
		STANDBY_MS_10   = 6, /**<   10ms   */
		STANDBY_MS_20   = 7, /**<   20ms   */
		STANDBY_MS_62_5 = 1, /**<   62.5ms */
		STANDBY_MS_125  = 2, /**<  125ms   */
		STANDBY_MS_250  = 3, /**<  250ms   */
		STANDBY_MS_500  = 4, /**<  500ms   */
		STANDBY_MS_1000 = 5  /**< 1000ms   */
	};
private:
	/** Possible device registers. */
	enum {
		BME280_REG_CHIPID     = 0xD0,
		BME280_REG_VERSION    = 0xD1,
		BME280_REG_SOFTRESET  = 0xE0,
		
		BME280_REG_DIG_T1     = 0x88,
		BME280_REG_DIG_T2     = 0x8A,
		BME280_REG_DIG_T3     = 0x8C,
		
		BME280_REG_DIG_P1     = 0x8E,
		BME280_REG_DIG_P2     = 0x90,
		BME280_REG_DIG_P3     = 0x92,
		BME280_REG_DIG_P4     = 0x94,
		BME280_REG_DIG_P5     = 0x96,
		BME280_REG_DIG_P6     = 0x98,
		BME280_REG_DIG_P7     = 0x9A,
		BME280_REG_DIG_P8     = 0x9C,
		BME280_REG_DIG_P9     = 0x9E,
		
		BME280_REG_DIG_H1     = 0xA1,
		BME280_REG_DIG_H2     = 0xE1,
		BME280_REG_DIG_H3     = 0xE3,
		BME280_REG_DIG_H4     = 0xE4,
		BME280_REG_DIG_H5     = 0xE5,
		BME280_REG_DIG_H6     = 0xE7,
		
		BME280_REG_CTRL_HUMID = 0xF2,
		BME280_REG_STATUS     = 0XF3,
		BME280_REG_CONTROL    = 0xF4,
		BME280_REG_CONFIG     = 0xF5,
		BME280_REG_PRESS_DATA = 0xF7,
		BME280_REG_TEMP_DATA  = 0xFA,
		BME280_REG_HUMID_DATA = 0xFD
	};
private:
	uint8_t address; /**< device I2C address */
	uint16_t cT1; /**< calibration data for temperature */
	int16_t cT2; /**< calibration data for temperature */
	int16_t cT3; /**< calibration data for temperature */
	uint16_t cP1; /**< calibration data for pressure */
	int16_t cP2; /**< calibration data for pressure */
	int16_t cP3; /**< calibration data for pressure */
	int16_t cP4; /**< calibration data for pressure */
	int16_t cP5; /**< calibration data for pressure */
	int16_t cP6; /**< calibration data for pressure */
	int16_t cP7; /**< calibration data for pressure */
	int16_t cP8; /**< calibration data for pressure */
	int16_t cP9; /**< calibration data for pressure */
	uint8_t cH1; /**< calibration data for humidity */
	int16_t cH2; /**< calibration data for humidity */
	uint8_t cH3; /**< calibration data for humidity */
	int16_t cH4; /**< calibration data for humidity */
	int16_t cH5; /**< calibration data for humidity */
	int8_t cH6; /**< calibration data for humidity */
	bool ioErr; /**< set true on I/O error */
	uint8_t control; /**< cached value for update() */
public:
	/**
	 * Constructor.
	 */
	inline explicit Bme280():
		address(0)
	{}

	/**
	 * Initializes the given BME280 sensor.
	 * 
	 * @param[in] addr - BME280 I2C address (alternatively 0x77)
	 */
	bool begin(const uint8_t addr = 0x76) {
		this->address = addr;
		Wire.begin();
		
		/* check if device is available and a BME280 */
		if (this->readU8(BME280_REG_CHIPID) != 0x60) return false;
		
		/*  perform a soft reset of the sensor */
		if ( ! this->writeU8(BME280_REG_SOFTRESET, 0xB6) ) return false;
		
		/* wait for the soft reset to complete */
		delay(300);
		
		/* further delay until chip has completely booted */
		while ((this->readU8(BME280_REG_STATUS, 0xFF) & 1) == 1) {
			if ( this->ioErr ) return false;
			delay(100);
		}
		
		/* read calibration data */
		this->cT1 = this->readU16LE(BME280_REG_DIG_T1);
		if ( this->ioErr ) return false;
		this->cT2 = this->readI16LE(BME280_REG_DIG_T2);
		if ( this->ioErr ) return false;
		this->cT3 = this->readI16LE(BME280_REG_DIG_T3);
		if ( this->ioErr ) return false;
		this->cP1 = this->readU16LE(BME280_REG_DIG_P1);
		if ( this->ioErr ) return false;
		this->cP2 = this->readI16LE(BME280_REG_DIG_P2);
		if ( this->ioErr ) return false;
		this->cP3 = this->readI16LE(BME280_REG_DIG_P3);
		if ( this->ioErr ) return false;
		this->cP4 = this->readI16LE(BME280_REG_DIG_P4);
		if ( this->ioErr ) return false;
		this->cP5 = this->readI16LE(BME280_REG_DIG_P5);
		if ( this->ioErr ) return false;
		this->cP6 = this->readI16LE(BME280_REG_DIG_P6);
		if ( this->ioErr ) return false;
		this->cP7 = this->readI16LE(BME280_REG_DIG_P7);
		if ( this->ioErr ) return false;
		this->cP8 = this->readI16LE(BME280_REG_DIG_P8);
		if ( this->ioErr ) return false;
		this->cP9 = this->readI16LE(BME280_REG_DIG_P9);
		if ( this->ioErr ) return false;
		this->cH1 = this->readU8(BME280_REG_DIG_H1);
		if ( this->ioErr ) return false;
		this->cH2 = this->readU16LE(BME280_REG_DIG_H2);
		if ( this->ioErr ) return false;
		this->cH3 = this->readU8(BME280_REG_DIG_H3);
		if ( this->ioErr ) return false;
		this->cH4 = int16_t((int16_t(this->readU8(BME280_REG_DIG_H4)) << 4) | int16_t(this->readU8(BME280_REG_DIG_H4 + 1) & 0x0F));
		if ( this->ioErr ) return false;
		this->cH5 = int16_t((int16_t(this->readU8(BME280_REG_DIG_H5 + 1)) << 4) | int16_t(this->readU8(BME280_REG_DIG_H5) >> 4));
		if ( this->ioErr ) return false;
		this->cH6 = int8_t(this->readU8(BME280_REG_DIG_H6));
		if ( this->ioErr ) return false;
		
		/* set default operation mode and configuration */
		if ( ! this->setConfig() ) return false;
		
		/* wait until the device is ready */
		if ( ! this->waitForUpdate() ) return false;
		
		return true;
	}
	
	/**
	 * Releases the specific BME280 sensor.
	 */
	inline void end() {
		this->address = 0;
	}
	
	/**
	 * Sets the device configuration.
	 * 
	 * @param[in] mode - operation mode
	 * @param[in] tSp - temperature sampling rate
	 * @param[in] pSp - pressure sampling rate
	 * @param[in] hSp - humidity sampling rate
	 * @param[in] flt - filter mode
	 * @param[in] dur - standby duration
	 * @return true on success, else false
	 * @remarks It is recommended to wait at least 100ms after this before reading from the sensor.
	 */
	bool setConfig(Mode mode = MODE_NORMAL, Sampling tSp = SAMPLING_X16, Sampling pSp = SAMPLING_X16, Sampling hSp = SAMPLING_X16, Filter flt = FILTER_OFF, Standby dur = STANDBY_MS_0_5) {
		const uint8_t ctrlHumid(hSp);
		const uint8_t config(uint8_t((dur << 5) | (flt << 2) | 1));
		this->control = uint8_t(uint8_t((tSp << 5) | (pSp << 2) | mode));
		if ( ! this->writeU8(BME280_REG_CTRL_HUMID, ctrlHumid) ) return false;
		if ( ! this->writeU8(BME280_REG_CONFIG, config) ) return false;
		if ( ! this->writeU8(BME280_REG_CONTROL, this->control) ) return false;
		return true;
	}
	
	/**
	 * Forces an update of the measurement data on the device. This is only needed in forced
	 * operation mode.
	 * 
	 * @return true on success, else false
	 */
	bool update() {
		if ((this->control & 0x03) != MODE_FORCED) return true;
		/* set to forced mode again to force an measurement update */
		if ( ! this->writeU8(BME280_REG_CONTROL, this->control) ) return false;
        /* wait until the update completed */
		return this->waitForUpdate();
	}
	
	/**
	 * Waits until a possible ongoing update on the device has finished.
	 * 
	 * @return true on success, else false on I/O error
	 */
	bool waitForUpdate() {
        while ((this->readU8(BME280_REG_STATUS) & 0x08) != 0) {
			if ( this->ioErr ) return false;
			delay(1);
		}
		return ( ! this->ioErr );
	}
	
	/**
	 * Returns the fine resolution temperature used in all calibration calculations.
	 * 
	 * @return fine resolution temperature or 0x00800000 on error
	 * @remarks Sets ioErr accordingly.
	 */
	int32_t readFineTemperature() {
		int32_t val(this->readU24BE(BME280_REG_TEMP_DATA, uint32_t(0x00800000)));
		if (val == int32_t(0x00800000)) return val;
		
		/* incorporate calibration data */
		val >>= 7;
		const int32_t b = ((val - (int32_t(this->cT1) << 1)) * int32_t(this->cT2)) >> 11;
		const int32_t c = (((((val >> 1) - int32_t(this->cT1)) * ((val >> 1) - int32_t(this->cT1))) >> 12) * int32_t(this->cT3)) >> 14;
		return b + c;
	}
	
	/**
	 * Reads the current temperature value in degrees Celsius from the sensor.
	 * 
	 * @return temperature in degrees Celsius or NAN on error
	 */
	inline float readTemperature() {
		return this->readTemperature(this->readFineTemperature());
	}
	
	/**
	 * Reads the current temperature value in degrees Celsius from the sensor.
	 * 
	 * @param[in] fineTemp - cached result from readFineTemperature()
	 * @return temperature in degrees Celsius or NAN on error
	 */
	float readTemperature(const int32_t fineTemp) {
		if (fineTemp == int32_t(0x00800000)) return NAN;
		return float(((fineTemp * 5) + 128) >> 8) / 100.0f;
	}
	
	/**
	 * Reads the current air pressure value in Pascal from the sensor.
	 * 
	 * @return air pressure in Pascal or NAN on error
	 */
	inline float readPressure() {
		return this->readPressure(this->readFineTemperature());
	}
	
	/**
	 * Reads the current air pressure value in Pascal from the sensor.
	 * 
	 * @param[in] fineTemp - cached result from readFineTemperature()
	 * @return air pressure in Pascal or NAN on error
	 */
	float readPressure(const int32_t fineTemp) {
		if (fineTemp == int32_t(0x00800000)) return NAN;
		int32_t val(this->readU24BE(BME280_REG_PRESS_DATA, uint32_t(0x00800000)));
		if (val == int32_t(0x00800000)) return NAN;
		
		/* incorporate calibration data */
		val >>= 4;
		int64_t a = int64_t(fineTemp) - int64_t(128000);
		int64_t b = a * a * int64_t(this->cP6);
		b = b + ((a * int64_t(this->cP5)) << 17);
		b = b + (int64_t(this->cP4) << 35);
		a = ((a * a * int64_t(this->cP3)) >> 8) + ((a * int64_t(this->cP2)) << 12);
		a = (((int64_t(1) << 47) + a) * int64_t(this->cP1)) >> 33;

		if (a == 0) return 0; /* handle zero divisor */
		int64_t p(int32_t(1048576) - val);
		p = (((p << 31) - b) * int64_t(3125)) / a;
		a = (int64_t(this->cP9) * (p >> 13) * (p >> 13)) >> 25;
		b = (int64_t(this->cP8) * p) >> 19;

		p = ((p + a + b) >> 8) + (int64_t(this->cP7) << 4);
		return float(p) / 256.0f;
	}
	
	/**
	 * Reads the current humidity value in %RH from the sensor.
	 * 
	 * @return humidity in %RH or NAN on error
	 */
	inline float readHumidity() {
		return this->readHumidity(this->readFineTemperature());
	}
	
	/**
	 * Reads the current humidity value in %RH from the sensor.
	 * 
	 * @param[in] fineTemp - cached result from readFineTemperature()
	 * @return humidity in %RH or NAN on error
	 */
	float readHumidity(const int32_t fineTemp) {
		if (fineTemp == int32_t(0x00800000)) return NAN;
		int32_t val(this->readU16BE(BME280_REG_HUMID_DATA, uint16_t(0x8000)));
		if (val == int32_t(0x00008000)) return NAN;
		
		/* incorporate calibration data */
		int32_t a(fineTemp - int32_t(76800));
		a = (((((val << 14) - (int32_t(this->cH4) << 20) - (int32_t(this->cH5) * a)) + int32_t(16384)) >> 15) *
		    (((((((a * int32_t(this->cH6)) >> 10) * (((a * int32_t(this->cH3)) >> 11) + int32_t(32768))) >> 10) +
		    int32_t(2097152)) * int32_t(this->cH2) + 8192) >> 14));

		a = a - (((((a >> 15) * (a >> 15)) >> 7) * int32_t(this->cH1)) >> 4);

		/* cap result */
		a = (a < 0) ? 0 : a;
		a = (a > int32_t(419430400)) ? int32_t(419430400) : a;
		
		return float(a >> 12) / 1024.0f;
	}
private:
	/**
	 * Helper function to read a 8-bit unsigned value from the given register of the device.
	 * 
	 * @param[in] reg - register to read
	 * @param[in] def - value returned on error
	 * @return 8-bit value from the register
	 * @remarks Sets ioErr accordingly.
	 */
	uint8_t readU8(const uint8_t reg, const uint8_t def = 0) {
		Wire.beginTransmission(this->address);
		Wire.write(reg);
		if (Wire.endTransmission() != 0) {
			this->ioErr = true;
			return def;
		}
		Wire.requestFrom(this->address, uint8_t(1));
		const int res = Wire.read();
		if (res < 0) {
			this->ioErr = true;
			return def;
		}
		this->ioErr = false;
		return uint8_t(res);
	}
	
	/**
	 * Helper function to read a 16-bit unsigned value big endian from the given register of the device.
	 * 
	 * @param[in] reg - register to read
	 * @param[in] def - value returned on error
	 * @return 16-bit big endian value from the register
	 * @remarks Sets ioErr accordingly.
	 */
	uint16_t readU16BE(const uint8_t reg, const uint16_t def = 0) {
		Wire.beginTransmission(this->address);
		Wire.write(reg);
		if (Wire.endTransmission() != 0) {
			this->ioErr = true;
			return def;
		}
		Wire.requestFrom(this->address, uint8_t(2));
		const int res1 = Wire.read();
		const int res2 = Wire.read();
		if (res1 < 0 || res2 < 0) {
			this->ioErr = true;
			return def;
		}
		this->ioErr = false;
		return uint16_t((uint16_t(res1) << 8) | uint16_t(res2));
	}
	
	/**
	 * Helper function to read a 24-bit unsigned value big endian from the given register of the device.
	 * 
	 * @param[in] reg - register to read
	 * @param[in] def - value returned on error
	 * @return 24-bit big endian value from the register
	 * @remarks Sets ioErr accordingly.
	 */
	uint32_t readU24BE(const uint8_t reg, const uint32_t def = 0) {
		Wire.beginTransmission(this->address);
		Wire.write(reg);
		if (Wire.endTransmission() != 0) {
			this->ioErr = true;
			return def;
		}
		Wire.requestFrom(this->address, uint8_t(3));
		const int res1 = Wire.read();
		const int res2 = Wire.read();
		const int res3 = Wire.read();
		if (res1 < 0 || res2 < 0 || res3 < 0) {
			this->ioErr = true;
			return def;
		}
		this->ioErr = false;
		return (uint32_t(res1) << 16) | (uint32_t(res2) << 8) | uint32_t(res3);
	}
	
	/**
	 * Helper function to read a 16-bit unsigned value little endian from the given register of the device.
	 * 
	 * @param[in] reg - register to read
	 * @param[in] def - value returned on error
	 * @return 16-bit little endian value from the register
	 * @remarks Sets ioErr accordingly.
	 */
	uint16_t readU16LE(const uint8_t reg, const uint16_t def = 0) {
		const uint16_t res = this->readU16BE(reg, def);
		if ( this->ioErr ) return def;
		return uint16_t(((res >> 8) & 0x00FF) | ((res << 8) & 0xFF00));
	}
	
	/**
	 * Helper function to read a 16-bit signed value little endian from the given register of the device.
	 * 
	 * @param[in] reg - register to read
	 * @param[in] def - value returned on error
	 * @return 16-bit little endian value from the register
	 * @remarks Sets ioErr accordingly.
	 */
	inline int16_t readI16LE(const uint8_t reg, const int16_t def = 0) {
		return int16_t(this->readU16LE(reg, uint16_t(def)));
	}
	
	/**
	 * Helper function to write a 8-bit value to the given register of the device.
	 * 
	 * @param[in] reg - register to write
	 * @param[in] val - value to set
	 * @return true on success, else false
	 * @remarks Sets ioErr accordingly.
	 */
	bool writeU8(const uint8_t reg, const uint8_t val) {
		Wire.beginTransmission(this->address);
		Wire.write(reg);
		Wire.write(val);
		this->ioErr = (Wire.endTransmission() != 0);
		return ( ! this->ioErr );
	}
};


#endif /* __BME280_HPP__ */
