/**
 * @file Print.cpp
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-09
 * @version 2022-03-22
 */
#include <math.h>
#include <string.h>
#include "Arduino.h"
#include "Print.h"


/* Defined in WString.cpp. */
extern const char * _charMap;


/**
 * Constructor.
 */
Print::Print():
	writeError(0)
{}


/**
 * Returns the last write error.
 * 
 * @return last write error
 */
int Print::getWriteError(void) {
	return this->writeError;
}


/**
 * Sets the last write error to zero.
 */
void Print::clearWriteError(void) {
	this->setWriteError(0);
}


/**
 * Writes out the given null-terminated string.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::write(const char * str) {
	if (str == NULL) return 0;
	const size_t l = strlen(str);
	return this->write(reinterpret_cast<const uint8_t *>(str), l);
}


/**
 * Writes out the given buffer with the specific size.
 * 
 * @param[in] buffer - buffer to write
 * @param[in] size - number of bytes to write
 * @return number of bytes written
 */
size_t Print::write(const uint8_t * buffer, size_t size) {
	if (buffer == NULL) return 0;
	for (size_t i = 0; i < size; i++, buffer++) {
		if (this->write(*buffer) == 0) return i;
	}
	return size;
}


/**
 * Writes out the given buffer with the specific size.
 * 
 * @param[in] buffer - buffer to write
 * @param[in] size - number of characters to write
 * @return number of characters written
 */
size_t Print::write(const char * buffer, size_t size) {
	return this->write(reinterpret_cast<const uint8_t *>(buffer), size);
}


/**
 * Returns the number of bytes available for write.
 * 
 * @return remaining number of bytes in the write queue
 */
int Print::availableForWrite(void) {
	return 0;
}


/**
 * Writes out the given string.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::print(const __FlashStringHelper * str) {
	return this->print(reinterpret_cast<const char *>(str));
}


/**
 * Writes out the given string.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::print(const String & str) {
	return this->write(str.c_str(), str.length());
}


/**
 * Writes out the given string.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::print(const char str[]) {
	return this->write(str);
}


/**
 * Writes out the given character.
 * 
 * @param[in] c - character to write
 * @return number of characters written
 */
size_t Print::print(char c) {
	return this->write(c);
}


/**
 * Writes out the given integral number as string.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::print(unsigned char val, int base) {
	return this->print(static_cast<unsigned long>(val), base);
}


/**
 * Writes out the given integral number as string.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::print(int val, int base) {
	return this->print(static_cast<long>(val), base);
}


/**
 * Writes out the given integral number as string.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::print(unsigned int val, int base) {
	return this->print(static_cast<unsigned long>(val), base);
}


/**
 * Writes out the given integral number as string.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::print(long val, int base) {
	if (base == 0) return this->write(static_cast<uint8_t>(val));
	if (base != 10) return this->printNumber(val, static_cast<uint8_t>(base));
	if (val < 0) {
		if (this->write('-') == 0) return 0;
		return this->printNumber(-val, 10) + 1;
	}
	return this->printNumber(val, 10);
}


/**
 * Writes out the given integral number as string.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::print(unsigned long val, int base) {
	if (base == 0) return this->write(static_cast<uint8_t>(val));
	return this->printNumber(val, static_cast<uint8_t>(base));
}


/**
 * Writes out the given floating point number as string.
 * 
 * @param[in] val - number to write
 * @param[in] digits - convert number to string with this number of digits
 * @return number of characters written
 */
size_t Print::print(FloatType val, int digits) {
	return this->printFloat(val, static_cast<uint8_t>(digits));
}


/**
 * Writes out the given printable object.
 * 
 * @param[in] obj - object to print
 * @return number of characters written
 */
size_t Print::print(const Printable & obj) {
	return obj.printTo(*this);
}


/**
 * Writes out the given string with a line break.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::println(const __FlashStringHelper * str) {
	return this->println(reinterpret_cast<const char *>(str));
}


/**
 * Writes out the given string with a line break.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::println(const String & str) {
	return this->print(str) + this->println();
}


/**
 * Writes out the given string with a line break.
 * 
 * @param[in] str - string to write
 * @return number of characters written
 */
size_t Print::println(const char str[]) {
	return this->print(str) + this->println();
}


/**
 * Writes out the given character with a line break.
 * 
 * @param[in] c - character to write
 * @return number of characters written
 */
size_t Print::println(char c) {
	return this->print(c) + this->println();
}


/**
 * Writes out the given integral number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::println(unsigned char val, int base) {
	return this->println(static_cast<unsigned long>(val), base);
}


/**
 * Writes out the given integral number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::println(int val, int base) {
	return this->println(static_cast<long>(val), base);
}


/**
 * Writes out the given integral number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::println(unsigned int val, int base) {
	return this->println(static_cast<unsigned long>(val), base);
}


/**
 * Writes out the given integral number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::println(long val, int base) {
	return this->print(val, base) + this->println();
}


/**
 * Writes out the given integral number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::println(unsigned long val, int base) {
	return this->print(val, base) + this->println();
}


/**
 * Writes out the given floating point number as string with a line break.
 * 
 * @param[in] val - number to write
 * @param[in] digits - convert number to string with this number of digits
 * @return number of characters written
 */
size_t Print::println(FloatType val, int digits) {
	return this->print(val, static_cast<uint8_t>(digits)) + this->println();
}


/**
 * Writes out the given printable object with a line break.
 * 
 * @param[in] obj - object to print
 * @return number of characters written
 */
size_t Print::println(const Printable & obj) {
	return this->print(obj) + this->println();
}


/**
 * Writes out a line break.
 * 
 * @return number of characters written
 */
size_t Print::println(void) {
	return this->write("\r\n");
}


/**
 * Waits until the output buffer is completely written.
 */
void Print::flush(void) {
}


/**
 * Sets a new write error.
 * 
 * @param[in] err - value to set
 */
void Print::setWriteError(int err) {
	this->writeError = err;
}


/**
 * Internal function to convert an integral number
 * into a string and write that out.
 * 
 * @param[in] val - number to write
 * @param[in] base - convert number to string using this base
 * @return number of characters written
 */
size_t Print::printNumber(unsigned long val, uint8_t base) {
	char buf[8 * sizeof(long) + 1];
	char * str = buf + sizeof(buf) - 1;

	*str = 0;
	if (base < 2) base = 10;

	do {
		*(--str) = _charMap[val % base];
		val /= base;
	} while (val);

	return this->write(str);
}


/**
 * Internal function to convert a floating point number
 * into a string and write that out.
 * 
 * @param[in] val - number to write
 * @param[in] digits - convert number to string with this number of digits
 * @return number of characters written
 */
size_t Print::printFloat(FloatType val, uint8_t digits) {
	if (isnan(val) != 0) return this->print("nan");
	if (isinf(val) != 0) return this->print("inf");
#ifndef STM32CUBEDUINO_FLOAT_STRING
	/* values taken from Arduino core for compatibility (limit of a float mantissa + 1 bit) */
	if (val > 4294967040.0) return this->print("ovf");
	if (val < -4294967040.0) return this->print("ovf");
#endif /* not STM32CUBEDUINO_FLOAT_STRING */
	
	size_t written = 0;
	
	if (val < 0) {
		written += this->write('-');
		val = -val;
	}
	
	FloatType rounding = 0.5f;
	for (uint8_t i = 0; i < digits; i++) {
		rounding /= 10.0f;
	}
	val += rounding;
	
	unsigned long integer = static_cast<unsigned long>(val);
	val -= static_cast<FloatType>(integer);
	
	written += this->print(integer);
	if (digits > 0) written += this->write('.');
	
	for (; digits > 0; digits--) {
		val *= 10.0f;
		const uint8_t digit = static_cast<uint8_t>(val);
		written += this->write(static_cast<uint8_t>(digit + '0'));
		val -= static_cast<FloatType>(digit);
	}
	
	return written;
}
