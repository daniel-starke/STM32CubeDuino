/**
 * @file Print.h
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-09
 * @version 2022-04-17
 */
#ifndef __PRINT_H__
#define __PRINT_H__

#include <stdint.h>
#include "WString.h"
#include "Printable.h"


#undef DEC
#undef HEX
#undef OCT
#undef BIN
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2


class Print {
private:
	int writeError;
#ifdef STM32CUBEDUINO_FLOAT_STRING
	typedef float FloatType;
#else /* not STM32CUBEDUINO_FLOAT_STRING */
	typedef double FloatType;
#endif /* not STM32CUBEDUINO_FLOAT_STRING */
public:
	Print();

	int getWriteError(void);
	void clearWriteError(void);
	virtual size_t write(uint8_t val) = 0;
	size_t write(const char * str);
	virtual size_t write(const uint8_t * buffer, size_t size);
	size_t write(const char * buffer, size_t size);
	virtual int availableForWrite(void);

#ifndef STM32CUBEDUINO_DISABLE_STRING
	size_t print(const __FlashStringHelper * str);
	size_t print(const String & str);
#endif /* not STM32CUBEDUINO_DISABLE_STRING */
	size_t print(const char str[]);
	size_t print(char c);
	size_t print(unsigned char val, int base = DEC);
	size_t print(int val, int base = DEC);
	size_t print(unsigned int val, int base = DEC);
	size_t print(long val, int base = DEC);
	size_t print(unsigned long val, int base = DEC);
	size_t print(FloatType val, int base = 2);
	size_t print(const Printable & obj);

#ifndef STM32CUBEDUINO_DISABLE_STRING
	size_t println(const __FlashStringHelper * str);
	size_t println(const String & str);
#endif /* not STM32CUBEDUINO_DISABLE_STRING */
	size_t println(const char str[]);
	size_t println(char c);
	size_t println(unsigned char val, int base = DEC);
	size_t println(int val, int base = DEC);
	size_t println(unsigned int val, int base = DEC);
	size_t println(long val, int base = DEC);
	size_t println(unsigned long val, int base = DEC);
	size_t println(FloatType val, int base = 2);
	size_t println(const Printable & obj);
	size_t println(void);

	virtual void flush(void);
protected:
	void setWriteError(int err = 1);
private:
	size_t printNumber(unsigned long val, uint8_t base);
	size_t printFloat(FloatType val, uint8_t digits);
};


#endif /* __PRINT_H__ */
