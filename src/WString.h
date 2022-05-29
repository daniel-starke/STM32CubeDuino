/**
 * @file WString.h
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-10
 * @version 2022-04-17
 */
#ifndef __WSTRING_H__
#define __WSTRING_H__

#include <stdlib.h>
#include <string.h>


#ifndef STM32CUBEDUINO_DISABLE_STRING


/* for compatibility */
class __FlashStringHelper;
#define F(str) (reinterpret_cast<const __FlashStringHelper *>(PSTR(str)))


/* forward declaration */
class StringSumHelper;


class String {
private:
	typedef void (String::* StringIfHelperType)() const;
public:
	char * buffer;
	unsigned int capacity;
	unsigned int len;
public:
	explicit String(const String & str);
	explicit String(const char * cstr = "");
	explicit String(const __FlashStringHelper * str);
	explicit String(char c);
	explicit String(unsigned char val, unsigned char base = 10);
	explicit String(int val, unsigned char base = 10);
	explicit String(unsigned int val, unsigned char base = 10);
	explicit String(long val, unsigned char base = 10);
	explicit String(unsigned long val, unsigned char base = 10);
	explicit String(float val, unsigned char digits = 2);
	explicit String(double val, unsigned char digits = 2);
#if __cplusplus >= 201103L
	String(String && o);
	String(StringSumHelper && o);
#endif /* __cplusplus >= 201103L */

	~String(void);

	unsigned char reserve(unsigned int size);
	unsigned int length(void) const;

	String & operator= (const String & str);
	String & operator= (const char * cstr);
	String & operator= (const __FlashStringHelper * str);
#if __cplusplus >= 201103L
	String & operator= (String && o);
	String & operator= (StringSumHelper && o);
#endif /* __cplusplus >= 201103L */

	unsigned char concat(const String & str);
	unsigned char concat(const char * cstr);
	unsigned char concat(const __FlashStringHelper * str);
	unsigned char concat(char c);
	unsigned char concat(unsigned char val);
	unsigned char concat(int val);
	unsigned char concat(unsigned int val);
	unsigned char concat(long val);
	unsigned char concat(unsigned long val);
	unsigned char concat(float val);
	unsigned char concat(double val);

	String & operator+= (const String & str);
	String & operator+= (const char * cstr);
	String & operator+= (const __FlashStringHelper * str);
	String & operator+= (char c);
	String & operator+= (unsigned char val);
	String & operator+= (int val);
	String & operator+= (unsigned int val);
	String & operator+= (long val);
	String & operator+= (unsigned long val);
	String & operator+= (float val);
	String & operator+= (double val);

	friend StringSumHelper & operator+ (const StringSumHelper & o, const String & str);
	friend StringSumHelper & operator+ (const StringSumHelper & o, const char * cstr);
	friend StringSumHelper & operator+ (const StringSumHelper & o, const __FlashStringHelper * str);
	friend StringSumHelper & operator+ (const StringSumHelper & o, char c);
	friend StringSumHelper & operator+ (const StringSumHelper & o, unsigned char val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, int val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, unsigned int val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, long val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, unsigned long val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, float val);
	friend StringSumHelper & operator+ (const StringSumHelper & o, double val);

	operator StringIfHelperType(void) const;
	int compareTo(const String & str) const;
	unsigned char equals(const String & str) const;
	unsigned char equals(const char * cstr) const;
	unsigned char operator== (const String & str) const;
	unsigned char operator== (const char * cstr) const;
	unsigned char operator!= (const String & str) const;
	unsigned char operator!= (const char * cstr) const;
	unsigned char operator< (const String & str) const;
	unsigned char operator> (const String & str) const;
	unsigned char operator<= (const String & str) const;
	unsigned char operator>= (const String & str) const;
	unsigned char equalsIgnoreCase(const String & str) const;
	unsigned char startsWith(const String & prefix) const;
	unsigned char startsWith(const String & prefix, unsigned int offset) const;
	unsigned char endsWith(const String & suffix) const;

	char charAt(unsigned int index) const;
	void setCharAt(unsigned int index, char c);
	char operator[] (unsigned int index) const;
	char & operator[] (unsigned int index);
	void getBytes(unsigned char * buf, unsigned int size, unsigned int index = 0) const;
	void toCharArray(char * buf, unsigned int size, unsigned int index = 0) const;
	const char * c_str(void) const;
	char * begin(void);
	char * end(void);
	const char * begin(void) const;
	const char * end(void) const;

	int indexOf(char c) const;
	int indexOf(char c, unsigned int index) const;
	int indexOf(const String & str) const;
	int indexOf(const String & str, unsigned int index) const;
	int lastIndexOf(char c) const;
	int lastIndexOf(char c, unsigned int index) const;
	int lastIndexOf(const String & str) const;
	int lastIndexOf(const String & str, unsigned int index) const;
	String substring(unsigned int index) const;
	String substring(unsigned int beginIndex, unsigned int endIndex) const;

	void replace(char find, char repl);
	void replace(const String & find, const String & repl);
	void remove(unsigned int index);
	void remove(unsigned int index, unsigned int count);
	void toLowerCase(void);
	void toUpperCase(void);
	void trim(void);

	long toInt(void) const;
	float toFloat(void) const;
	double toDouble(void) const;
protected:
	void init(void);
	void invalidate(void);
	unsigned char changeBuffer(unsigned int maxStrLen);
	unsigned char concat(const char * cstr, unsigned int strLen);

	String & copy(const char * cstr, unsigned int strLen);
	String & copy(const __FlashStringHelper * str, unsigned int strLen);
	
#if __cplusplus >= 201103L
	void move(String & str);
#endif /* __cplusplus >= 201103L */
private:
	void StringIfHelper() const;
};


class StringSumHelper : public String {
public:
	StringSumHelper(const String & str);
	StringSumHelper(const char * cstr);
	StringSumHelper(__FlashStringHelper * str);
	StringSumHelper(char c);
	StringSumHelper(unsigned char val);
	StringSumHelper(int val);
	StringSumHelper(unsigned int val);
	StringSumHelper(long val);
	StringSumHelper(unsigned long val);
	StringSumHelper(float val);
	StringSumHelper(double val);
};


#endif /* not STM32CUBEDUINO_DISABLE_STRING */


#endif /* __WSTRING_H__ */
