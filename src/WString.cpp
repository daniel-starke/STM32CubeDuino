/**
 * @file WString.cpp
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-10
 * @version 2022-03-22
 */
#include <ctype.h>
#include <stdio.h>
#include "WString.h"


/** Possible characters in base 2 to 36. */
const char * _charMap = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


namespace {
template <typename T> struct as_unsigned;
template <> struct as_unsigned<int> { typedef unsigned int type; };
template <> struct as_unsigned<long> { typedef unsigned long type; };
template <> struct as_unsigned<unsigned int> { typedef unsigned int type; };
template <> struct as_unsigned<unsigned long> { typedef unsigned long type; };


/**
 * Helper function to convert an signed or unsigned integer into a string.
 * 
 * @param[in] val - value to convert
 * @param[out] str - output buffer
 * @param[in] base - target base
 * @param[in] negative - add negative sign?
 * @return str on success, else false
 */
template <typename T>
char * intToStrWithSign(T val, char * str, int base, bool negative) {
	if (str == NULL || base < 2 || base > 36) {
		return NULL;
	}

	char buf[(sizeof(val) * 8) + 2];
	char * ptr = buf + sizeof(buf) - 1;

	*ptr = 0;
	do {
		*--ptr = _charMap[-(val % base)];
		val /= base;
	} while (val);
	
	if (negative) *--ptr = '-';

	const size_t len = static_cast<size_t>(buf + sizeof(buf) - ptr);
	return static_cast<char *>(memcpy(str, ptr, len));
}


/**
 * Helper function to convert an signed or unsigned integer into a string.
 * 
 * @param[in] val - value to convert
 * @param[out] str - output buffer
 * @param[in] base - target base
 * @return str on success, else false
 */
template <typename T>
char * intToStr(T val, char * str, int base) {
	if (val < 0) {
		return intToStrWithSign(static_cast<typename as_unsigned<T>::type>(-val), str, base, true);
	}
	return intToStrWithSign(static_cast<typename as_unsigned<T>::type>(val), str, base, false);
}
} /* anonymous namespace */


/**
 * Copy constructor.
 * 
 * @param[in] str - input string (copied)
 */
String::String(const String & str) {
	this->init();
	*this = str;
}


/**
 * Constructor.
 * 
 * @param[in] cstr - input string (copied)
 */
String::String(const char * cstr) {
	this->init();
	if (cstr != NULL) this->copy(cstr, static_cast<unsigned int>(strlen(cstr)));
}


/**
 * Constructor.
 * 
 * @param[in] str - input string (copied)
 */
String::String(const __FlashStringHelper * str) {
	this->init();
	*this = str;
}


/**
 * Constructor
 * 
 * @param[in] c - single input character
 */
String::String(char c) {
	const char buf[2] = {c, 0};
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] base - target base
 */
String::String(unsigned char val, unsigned char base) {
	char buf[(8 * sizeof(val)) + 1];
	intToStr(static_cast<unsigned int>(val), buf, base);
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] base - target base
 */
String::String(int val, unsigned char base) {
	char buf[(8 * sizeof(val)) + 2];
	intToStr(val, buf, base);
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] base - target base
 */
String::String(unsigned int val, unsigned char base) {
	char buf[(8 * sizeof(val)) + 1];
	intToStr(val, buf, base);
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] base - target base
 */
String::String(long val, unsigned char base) {
	char buf[(8 * sizeof(val)) + 2];
	intToStr(val, buf, base);
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] base - target base
 */
String::String(unsigned long val, unsigned char base) {
	char buf[(8 * sizeof(val)) + 1];
	intToStr(val, buf, base);
	this->init();
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] digits - number of digits
 */
String::String(float val, unsigned char digits) {
	char buf[33] = {0};
	this->init();
	sprintf(buf, "%.*f", digits, val);
	*this = buf;
}


/**
 * Constructor.
 * 
 * @param[in] val - input number
 * @param[in] digits - number of digits
 */
String::String(double val, unsigned char digits) {
	char buf[33];
	this->init();
	sprintf(buf, "%.*f", digits, val);
	*this = buf;
}


#if __cplusplus >= 201103L
/**
 * Move constructor.
 * 
 * @param[in] o - object to move
 */
String::String(String && o) {
	this->init();
	this->move(o);
}


/**
 * Move constructor.
 * 
 * @param[in] o - object to move
 */
String::String(StringSumHelper && o) {
	this->init();
	this->move(o);
}
#endif /* __cplusplus >= 201103L */


/**
 * Destructor.
 */
String::~String(void) {
	free(this->buffer);
}


unsigned char String::reserve(unsigned int size) {
	if (this->buffer != NULL && this->capacity >= size) return 1;
	if (this->changeBuffer(size) != 0) {
		if (this->len == 0) this->buffer[0] = 0;
		return 1;
	}
	return 0;
}


unsigned int String::length(void) const {
	return this->len;
}


String & String::operator= (const String & str) {
	if (this == &str) return *this;
	if (str.buffer != NULL) {
		this->copy(str.buffer, str.len);
	} else {
		this->invalidate();
	}
	return *this;
}


String & String::operator= (const char * cstr) {
	if (cstr != NULL) {
		this->copy(cstr, static_cast<unsigned int>(strlen(cstr)));
	} else {
		this->invalidate();
	}
	return *this;
}


String & String::operator= (const __FlashStringHelper * str) {
	return this->operator= (reinterpret_cast<const char *>(str));
}


#if __cplusplus >= 201103L
String & String::operator= (String && o) {
	if (this != &o) this->move(o);
	return *this;
}


String & String::operator= (StringSumHelper && o) {
	if (this != &o) this->move(o);
	return *this;
}
#endif /* __cplusplus >= 201103L */


unsigned char String::concat(const String & str) {
	return this->concat(str.buffer, str.len);
}


unsigned char String::concat(const char * cstr) {
	if (cstr == NULL) return 0;
	return this->concat(cstr, static_cast<unsigned int>(strlen(cstr)));
}


unsigned char String::concat(const __FlashStringHelper * str) {
	return this->concat(reinterpret_cast<const char *>(str));
}


unsigned char String::concat(char c) {
	const char buf[2] = {c, 0};
	return this->concat(buf, 1);
}


unsigned char String::concat(unsigned char val) {
	char buf[(3 * sizeof(val)) + 1];
	intToStr(static_cast<unsigned int>(val), buf, 10);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(int val) {
	char buf[(3 * sizeof(val)) + 2];
	intToStr(val, buf, 10);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(unsigned int val) {
	char buf[(3 * sizeof(val)) + 1];
	intToStr(val, buf, 10);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(long val) {
	char buf[(3 * sizeof(val)) + 2];
	intToStr(val, buf, 10);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(unsigned long val) {
	char buf[(3 * sizeof(val)) + 1];
	intToStr(val, buf, 10);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(float val) {
	char buf[20];
	sprintf(buf, "%4.2f", val);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


unsigned char String::concat(double val) {
	char buf[20];
	sprintf(buf, "%4.2f", val);
	return this->concat(buf, static_cast<unsigned int>(strlen(buf)));
}


String & String::operator+= (const String & str) {
	this->concat(str);
	return *this;
}


String & String::operator+= (const char * cstr) {
	this->concat(cstr);
	return *this;
}


String & String::operator+= (const __FlashStringHelper * str) {
	this->concat(str);
	return *this;
}


String & String::operator+= (char c) {
	this->concat(c);
	return *this;
}


String & String::operator+= (unsigned char val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (int val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (unsigned int val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (long val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (unsigned long val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (float val) {
	this->concat(val);
	return *this;
}


String & String::operator+= (double val) {
	this->concat(val);
	return *this;
}


String::operator StringIfHelperType(void) const {
	return (this->buffer != NULL) ? &String::StringIfHelper : NULL;
}


int String::compareTo(const String & str) const {
	if (buffer == NULL || str.buffer == NULL) {
		if (str.buffer && str.len > 0) return 0 - static_cast<unsigned char>(str.buffer[0]);
		if (this->buffer && this->len > 0) return static_cast<unsigned char>(this->buffer[0]);
		return 0;
	}
	return strcmp(this->buffer, str.buffer);
}


unsigned char String::equals(const String & str) const {
	return this->len == str.len && this->compareTo(str) == 0;
}


unsigned char String::equals(const char * cstr) const {
	if (this->len == 0) return cstr == NULL || *cstr == 0;
	if (cstr == NULL) return this->buffer[0] == 0;
	return strcmp(this->buffer, cstr) == 0;
}


unsigned char String::operator== (const String & str) const {
	return this->equals(str);
}


unsigned char String::operator== (const char * cstr) const {
	return this->equals(cstr);
}


unsigned char String::operator!= (const String & str) const {
	return !this->equals(str);
}


unsigned char String::operator!= (const char * cstr) const {
	return !this->equals(cstr);
}


unsigned char String::operator< (const String & str) const {
	return this->compareTo(str) < 0;
}


unsigned char String::operator> (const String & str) const {
	return this->compareTo(str) > 0;
}


unsigned char String::operator<= (const String & str) const {
	return this->compareTo(str) <= 0;
}


unsigned char String::operator>= (const String & str) const {
	return this->compareTo(str) >= 0;
}


unsigned char String::equalsIgnoreCase(const String & str) const {
	if (this == &str) return 1;
	if (this->len != str.len) return 0;
	if (this->len == 0) return 1;
	const char * p1 = this->buffer;
	const char * p2 = str.buffer;
	while (*p1 != 0) {
		if (tolower(*p1++) != tolower(*p2++)) return 0;
	} 
	return 1;
}


unsigned char String::startsWith(const String & prefix) const {
	if (this->len < prefix.len) return 0;
	return this->startsWith(prefix, 0);
}


unsigned char String::startsWith(const String & prefix, unsigned int offset) const {
	if (offset > this->len - prefix.len || this->buffer == NULL || prefix.buffer == NULL) return 0;
	return strncmp(this->buffer + offset, prefix.buffer, prefix.len) == 0;
}


unsigned char String::endsWith(const String & suffix) const {
	if (this->len < suffix.len || this->buffer == NULL || suffix.buffer == NULL) return 0;
	return strcmp(this->buffer + this->len - suffix.len, suffix.buffer) == 0;
}


char String::charAt(unsigned int index) const {
	return this->operator[] (index);
}


void String::setCharAt(unsigned int index, char c) {
	if (index < this->len) this->buffer[index] = c;
}


char String::operator[] (unsigned int index) const {
	if (this->buffer == NULL || index >= this->len) return 0;
	return this->buffer[index];
}


char & String::operator[] (unsigned int index) {
	static char dummy;
	if (this->buffer == NULL || index >= this->len) {
		dummy = 0;
		return dummy;
	}
	return this->buffer[index];
}


void String::getBytes(unsigned char * buf, unsigned int size, unsigned int index) const {
	if (buf == NULL || size <= 0) return;
	if (index >= this->len) {
		buf[0] = 0;
		return;
	}
	unsigned int n = size - 1;
	if (n > (this->len - index)) n = len - index;
	strncpy(reinterpret_cast<char *>(buf), this->buffer + index, n);
	buf[n] = 0;
}


void String::toCharArray(char * buf, unsigned int size, unsigned int index) const {
	this->getBytes(reinterpret_cast<unsigned char *>(buf), size, index);
}


const char * String::c_str(void) const {
	return this->buffer;
}


char * String::begin(void) {
	return this->buffer;
}


char * String::end(void) {
	return this->buffer + this->len;
}


const char * String::begin(void) const {
	return this->c_str();
}


const char * String::end(void) const {
	return this->c_str() + this->len;
}


int String::indexOf(char c) const {
	return this->indexOf(c, 0);
	
}


int String::indexOf(char c, unsigned int index) const {
	if (index >= this->len) return -1;
	const char * ptr = strchr(this->buffer + index, c);
	if (ptr == NULL) return -1;
	return static_cast<int>(ptr - this->buffer);
}


int String::indexOf(const String & str) const {
	return this->indexOf(str, 0);
}


int String::indexOf(const String & str, unsigned int index) const {
	if (index >= this->len) return -1;
	const char * ptr = strstr(this->buffer + index, str.buffer);
	if (ptr == NULL) return -1;
	return static_cast<int>(ptr - this->buffer);
}


int String::lastIndexOf(char c) const {
	return this->lastIndexOf(c, this->len - 1);
}


int String::lastIndexOf(char c, unsigned int index) const {
	if (index >= this->len) return -1;
	const char oldEnd = buffer[index + 1];
	this->buffer[index + 1] = 0;
	const char * ptr = strrchr(buffer, c);
	this->buffer[index + 1] = oldEnd;
	if (ptr == NULL) return -1;
	return static_cast<int>(ptr - buffer);
}


int String::lastIndexOf(const String & str) const {
	return this->lastIndexOf(str, this->len - str.len);
}


int String::lastIndexOf(const String & str, unsigned int index) const {
  	if (str.len == 0 || this->len == 0 || str.len > this->len) return -1;
	if (index >= this->len) index = this->len - 1;
	int pos = -1;
	for (const char * ptr = this->buffer; ptr <= this->buffer + index; ptr++) {
		ptr = strstr(ptr, str.buffer);
		if (ptr == NULL) break;
		if (static_cast<unsigned int>(ptr - this->buffer) <= index) {
			pos = static_cast<int>(ptr - this->buffer);
		}
	}
	return pos;
}


String String::substring(unsigned int index) const {
	return this->substring(index, this->len);
}


String String::substring(unsigned int beginIndex, unsigned int endIndex) const {
	if (beginIndex > endIndex) {
		const unsigned int tmp = endIndex;
		endIndex = beginIndex;
		beginIndex = tmp;
	}
	String res;
	if (beginIndex >= this->len) return res;
	if (endIndex > this->len) endIndex = this->len;
	const char tmp = this->buffer[endIndex];
	this->buffer[endIndex] = 0;	
	res = this->buffer + beginIndex;
	this->buffer[endIndex] = tmp;
	return res;
}


void String::replace(char find, char repl) {
	if (this->buffer == NULL) return;
	for (char * ptr = this->buffer; *ptr; ptr++) {
		if (*ptr == find) *ptr = repl;
	}
}


void String::replace(const String & find, const String & repl) {
#ifdef NO_GPL
	(void)find; (void)repl;
	abort();
#else /* the following code is taken from the original Arduino library and is licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	if (this->len == 0 || find.len == 0) return;
	const int diff = repl.len - find.len;
	char * readFrom = this->buffer;
	char * foundAt;
	if (diff == 0) {
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			memcpy(foundAt, repl.buffer, repl.len);
			readFrom = foundAt + repl.len;
		}
	} else if (diff < 0) {
		char * writeTo = this->buffer;
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			const unsigned int n = static_cast<unsigned int>(foundAt - readFrom);
			memcpy(writeTo, readFrom, n);
			writeTo += n;
			memcpy(writeTo, repl.buffer, repl.len);
			writeTo += repl.len;
			readFrom = foundAt + find.len;
			this->len += diff;
		}
		strcpy(writeTo, readFrom);
	} else {
		unsigned int size = this->len;
		while ((foundAt = strstr(readFrom, find.buffer)) != NULL) {
			readFrom = foundAt + find.len;
			size += diff;
		}
		if (size == this->len) return;
		if (size > this->capacity && !changeBuffer(size)) return; /* allocation errors are silently ignored */
		for (int index = this->len - 1; index >= 0 && (index = lastIndexOf(find, index)) >= 0; index--) {
			readFrom = this->buffer + index + find.len;
			memmove(readFrom + diff, readFrom, this->len - (readFrom - this->buffer));
			this->len += diff;
			this->buffer[this->len] = 0;
			memcpy(this->buffer + index, repl.buffer, repl.len);
		}
	}
#endif
}


void String::remove(unsigned int index) {
	this->remove(index, static_cast<unsigned int>(-1));
}


void String::remove(unsigned int index, unsigned int count) {
#ifdef NO_GPL
	(void)index; (void)count;
	abort();
#else /* the following code is taken from the original Arduino library and is licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	if (index >= this->len) return;
	if (count <= 0) return;
	if (count > (this->len - index)) count = this->len - index;
	char * writeTo = this->buffer + index;
	this->len = this->len - count;
	strncpy(writeTo, this->buffer + index + count, this->len - index);
	this->buffer[len] = 0;
#endif
}


void String::toLowerCase(void) {
	if (this->buffer == NULL) return;
	for (char * ptr = this->buffer; *ptr; ptr++) {
		*ptr = static_cast<char>(tolower(*ptr));
	}
}


void String::toUpperCase(void) {
	if (this->buffer == NULL) return;
	for (char * ptr = this->buffer; *ptr; ptr++) {
		*ptr = static_cast<char>(toupper(*ptr));
	}
}


void String::trim(void) {
	if (this->buffer == NULL || this->len == 0) return;
	char * first = this->buffer;
	while (isspace(*first) != 0) first++;
	char * last = this->buffer + this->len - 1;
	while (isspace(*last) != 0 && last >= first) last--;
	this->len = static_cast<unsigned int>(last + 1 - first);
	if (first > this->buffer) memcpy(this->buffer, first, this->len);
	this->buffer[this->len] = 0;
}


long String::toInt(void) const {
	if (this->buffer != NULL) return atol(this->buffer);
	return 0;
}


float String::toFloat(void) const {
	return float(this->toDouble());
}


double String::toDouble(void) const {
	if (this->buffer != NULL) return atof(this->buffer);
	return 0.0f;
}


void String::init(void) {
	this->buffer = NULL;
	this->capacity = 0;
	this->len = 0;
}


void String::invalidate(void) {
	if (this->buffer != NULL) free(this->buffer);
	this->buffer = NULL;
	this->capacity = 0;
	this->len = 0;
}


unsigned char String::changeBuffer(unsigned int maxStrLen) {
	char * newBuffer = static_cast<char *>(realloc(this->buffer, maxStrLen + 1));
	if (newBuffer) {
		this->buffer = newBuffer;
		this->capacity = maxStrLen;
		return 1;
	}
	return 0;
}


unsigned char String::concat(const char * cstr, unsigned int strLen) {
	const unsigned int newLen = this->len + strLen;
	if (cstr == NULL) return 0;
	if (strLen == 0) return 1;
	if (this->reserve(newLen) == 0) return 0;
	strcpy(this->buffer + this->len, cstr);
	this->len = newLen;
	return 1;
}


String & String::copy(const char * cstr, unsigned int strLen) {
	if (this->reserve(strLen) == 0) {
		this->invalidate();
		return *this;
	}
	this->len = strLen;
	strcpy(this->buffer, cstr);
	return *this;
}


String & String::copy(const __FlashStringHelper * str, unsigned int strLen) {
	return this->copy(reinterpret_cast<const char *>(str), strLen);
}


#if __cplusplus >= 201103L
void String::move(String & str) {
	if (this->buffer != NULL) {
		if (str && this->capacity >= str.len) {
			strcpy(this->buffer, str.buffer);
			this->len = str.len;
			str.len = 0;
			return;
		} else {
			free(this->buffer);
		}
	}
	this->buffer = str.buffer;
	this->capacity = str.capacity;
	this->len = str.len;
	str.buffer = NULL;
	str.capacity = 0;
	str.len = 0;
}
#endif /* __cplusplus >= 201103L */


void String::StringIfHelper() const {
}


StringSumHelper::StringSumHelper(const String & str):
	String(str)
{}


StringSumHelper::StringSumHelper(const char * cstr):
	String(cstr)
{}


StringSumHelper::StringSumHelper(__FlashStringHelper * str):
	String(str)
{}


StringSumHelper::StringSumHelper(char c):
	String(c)
{}


StringSumHelper::StringSumHelper(unsigned char val):
	String(val)
{}


StringSumHelper::StringSumHelper(int val):
	String(val)
{}


StringSumHelper::StringSumHelper(unsigned int val):
	String(val)
{}


StringSumHelper::StringSumHelper(long val):
	String(val)
{}


StringSumHelper::StringSumHelper(unsigned long val):
	String(val)
{}


StringSumHelper::StringSumHelper(float val):
	String(val)
{}


StringSumHelper::StringSumHelper(double val):
	String(val)
{}


StringSumHelper & operator+ (const StringSumHelper & o, const String & str) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(str.buffer, str.len) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, const char * cstr) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if (cstr == NULL || ( ! res.concat(cstr, strlen(cstr)) )) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, const __FlashStringHelper * str) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(str) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, char c) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(c) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, unsigned char val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, int val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, unsigned int val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, long val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, unsigned long val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, float val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}


StringSumHelper & operator+ (const StringSumHelper & o, double val) {
	StringSumHelper & res = const_cast<StringSumHelper &>(o);
	if ( ! res.concat(val) ) res.invalidate();
	return res;
}
