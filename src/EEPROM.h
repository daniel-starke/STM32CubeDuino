/**
 * @file EEPROM.h
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-05-29
 * @version 2022-05-29
 */
#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stddef.h>
#include <stdint.h>


#if !defined(STM32CUBEDUINO_DISABLE_EEPROM) && defined(DATA_EEPROM_BASE) /* STM32 HAL FLASH EX header was included */

#ifndef E2END
/** Last valid EEPROM cell index. */
#define E2END ((DATA_EEPROM_END) - (DATA_EEPROM_BASE))
#endif


/**
 * EEPROM cell reference and I/O handling class.
 */
class EERef {
private:
	size_t index;
public:
	/**
	 * Constructor.
	 * 
	 * @param[in] idx - EEPROM cell index
	 */
	inline EERef(const size_t idx):
		index(idx)
	{}
	
	/**
	 * Reads the value at the current EEPROM cell index.
	 * 
	 * @return stored value
	 */
	inline uint8_t operator* () const {
		if (this->index < E2END) {
			return *reinterpret_cast<volatile const uint8_t *>(DATA_EEPROM_BASE + this->index);
		}
		return 0;
	}
	
	/**
	 * Reads the value at the current EEPROM cell index.
	 * 
	 * @return stored value
	 */
	inline operator uint8_t() const {
		return **this;
	}
	
	/**
	 * Writes the value at the passed reference to the
	 * current EEPORM cell.
	 * 
	 * @param[in] ref - write value from this EEPROM cell
	 * @return current EEPROM cell
	 */
	inline EERef & operator= (const EERef & ref) {
		*this = *ref;
		return *this;
	}
	
	/**
	 * Writes the given value to the current EEPROM cell.
	 * 
	 * @param[in] val - write this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator= (const uint8_t val) {
		if (this->index < E2END) {
			HAL_FLASHEx_DATAEEPROM_Unlock();
			HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_BASE + this->index, val);
			HAL_FLASHEx_DATAEEPROM_Lock();
		}
		return *this;
	}
	
	/**
	 * Adds the given value to the current EEPROM cell value.
	 * 
	 * @param[in] val - add this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator+= (const uint8_t val) {
		*this = uint8_t(**this + val);
		return *this;
	}
	
	/**
	 * Subtracts the given value from the current EEPROM cell value.
	 * 
	 * @param[in] val - subtract this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator-= (const uint8_t val) {
		*this = uint8_t(**this - val);
		return *this;
	}
	
	/**
	 * Multiplies the given value to the current EEPROM cell value.
	 * 
	 * @param[in] val - multiply this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator*= (const uint8_t val) {
		*this = uint8_t(**this * val);
		return *this;
	}
	
	/**
	 * Divides the given value from the current EEPROM cell value.
	 * 
	 * @param[in] val - divide by this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator/= (const uint8_t val) {
		*this = uint8_t(**this / val);
		return *this;
	}
	
	/**
	 * XOR the given value to the current EEPROM cell value.
	 * 
	 * @param[in] val - XOR this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator^= (const uint8_t val) {
		*this = **this ^ val;
		return *this;
	}
	
	/**
	 * Modulo the given value from the current EEPROM cell value.
	 * 
	 * @param[in] val - modulo by this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator%= (const uint8_t val) {
		*this = uint8_t(**this % val);
		return *this;
	}
	
	/**
	 * AND the given value to the current EEPROM cell value.
	 * 
	 * @param[in] val - AND this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator&= (const uint8_t val) {
		*this = **this & val;
		return *this;
	}
	
	/**
	 * OR the given value to the current EEPROM cell value.
	 * 
	 * @param[in] val - OR this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator|= (const uint8_t val) {
		*this = **this | val;
		return *this;
	}
	
	/**
	 * Shift left by the given value for the current EEPROM cell value.
	 * 
	 * @param[in] val - shift left by this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator<<= (const uint8_t val) {
		*this = uint8_t(**this << val);
		return *this;
	}
	
	/**
	 * Shift right by the given value for the current EEPROM cell value.
	 * 
	 * @param[in] val - shift right by this value
	 * @return current EEPROM cell
	 */
	inline EERef & operator>>= (const uint8_t val) {
		*this = uint8_t(**this >> val);
		return *this;
	}
	
	/**
	 * Writes the current EEPROM cell value only if the new value differs.
	 * 
	 * @param[in] val - write this value
	 * @return current EEPORM cell
	 */
	inline EERef & update(const uint8_t val) {
		if (val != *this) {
			*this = val;
		}
		return *this;
	}
	
	/**
	 * Increments (prefix) the current EEPROM cell value by one.
	 * 
	 * @return current EEPORM cell
	 */
	inline EERef & operator++ () {
		*this += 1;
		return *this;
	}
	
	/**
	 * Increments (postfix) the current EEPROM cell value by one.
	 * 
	 * @return current EEPORM cell
	 */
	inline uint8_t operator++ (int) {
		const uint8_t ret(*this);
		*this += 1;
		return ret;
	}
	
	/**
	 * Decrements (prefix) the current EEPROM cell value by one.
	 * 
	 * @return current EEPORM cell
	 */
	inline EERef & operator-- () {
		*this -= 1;
		return *this;
	}
	
	/**
	 * Decrements (postfix) the current EEPROM cell value by one.
	 * 
	 * @return current EEPORM cell
	 */
	inline uint8_t operator-- (int) {
		const uint8_t ret(*this);
		*this -= 1;
		return ret;
	}
};


/**
 * EEPROM cell pointer handling class.
 */
class EEPtr {
private:
	size_t index;
public:
	/**
	 * Constructor.
	 * 
	 * @param[in] idx - EEPROM cell index
	 */
	inline EEPtr(const size_t idx):
		index(idx)
	{}
	
	/**
	 * Returns the current EEPROM cell index.
	 * 
	 * @return current EEPROM cell index
	 */
	operator size_t() const {
		return this->index;
	}
	
	/**
	 * Sets the current EEPROM cell index.
	 * 
	 * @param[in] idx - new EEPROM cell index
	 * @return current EEPROM cell pointer
	 */
	EEPtr & operator= (const size_t idx) {
		this->index = idx;
		return *this;
	}
	
	/**
	 * Compares two EEPROM cell pointers.
	 * 
	 * @param[in] ptr - rhs pointer
	 * @return true if different, else false
	 */
	bool operator!= (const EEPtr & ptr) {
		return this->index != ptr.index;
	}
	
	/**
	 * Returns the current EEPROM cell index.
	 * 
	 * @return current EEPROM cell reference
	 */
	EERef operator* () {
		return this->index;
	}
	
	/**
	 * Increments (prefix) the current EEPROM cell index by one.
	 * 
	 * @return current EEPROM cell index
	 */
	EEPtr & operator++ () {
		this->index++;
		return *this;
	}
	
	/**
	 * Increments (postfix) the current EEPROM cell index by one.
	 * 
	 * @return current EEPROM cell index
	 */
	EEPtr operator++ (int) {
		return this->index++;
	}
	
	/**
	 * Decrements (prefix) the current EEPROM cell index by one.
	 * 
	 * @return current EEPROM cell index
	 */
	EEPtr & operator-- () {
		this->index--;
		return *this;
	}
	
	/**
	 * Decrements (postfix) the current EEPROM cell index by one.
	 * 
	 * @return current EEPROM cell index
	 */
	EEPtr operator-- (int) {
		return this->index--;
	}
};


/**
 * EEPROM base class.
 */
class EEPROMClass {
public:
	/**
	 * Returns a reference to the EEPROM cell at the given index.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @return EEPROM cell reference
	 */
	inline EERef operator[] (const size_t idx) {
		return EERef(idx);
	}
		
	/**
	 * Reads the value from the given EEPROM cell index.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @return stored value
	 */
	inline uint8_t read(const size_t idx) const {
		return *EERef(idx);
	}
	
	/**
	 * Writes a value to the given EEPROM cell index.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @param[in] val - write this value
	 */
	inline void write(const size_t idx, const uint8_t val) {
		EERef ref(idx);
		ref = val;
	}
	
	/**
	 * Writes a value to the given EEPROM cell index if
	 * the value differs from the stored one.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @param[in] val - write this value
	 */
	inline void update(const size_t idx, const uint8_t val) {
		EERef(idx).update(val);
	}
	
	/**
	 * C++11 iterator compatible `begin()` function.
	 * 
	 * @return pointer to the first EEPROM cell
	 */
	inline EEPtr begin() {
		return EEPtr(0);
	}
	
	/**
	 * C++11 iterator compatible `end()` function.
	 * 
	 * @return pointer to one after the last EEPROM cell
	 */
	inline EEPtr end() {
		return length();
	}
	
	/**
	 * Returns the number of valid EEPROM cells.
	 * 
	 * @return maximum EEPROM cell count
	 */
	inline size_t length() const {
		return E2END + 1;
	}
	
	/**
	 * Reads a value of an arbitrary type from the given
	 * EEPROM cell index in host endian order.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @param[out] val - output value reference
	 * @return value read
	 */
	template <typename T>
	T & get(const size_t idx, T & val) const {
		EEPtr inPtr = idx;
		uint8_t * outPtr = reinterpret_cast<uint8_t *>(&val);
		for (size_t n = 0; n < sizeof(T); ++n, ++inPtr) {
			*outPtr++ = *inPtr;
		}
		return val;
	}
	
	/**
	 * Writes a value of an arbitrary type to the given
	 * EEPROM cell index in host endian order.
	 * 
	 * @param[in] idx - EEPROM cell index
	 * @param[in] val - write this value
	 * @return value written
	 */
	template <typename T>
	const T & put(const size_t idx, const T & val) {
		EEPtr outPtr = idx;
		const uint8_t * inPtr = reinterpret_cast<const uint8_t *>(&val);
		for (size_t n = 0; n < sizeof(T); ++n, ++outPtr) {
			EERef(outPtr).update(*inPtr++);
		}
		return val;
	}
};


/** EEPROM instance. */
static EEPROMClass EEPROM;


#endif /* DATA_EEPROM_BASE */
#endif /* __EEPROM_H__ */
