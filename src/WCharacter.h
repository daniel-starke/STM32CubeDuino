/**
 * @file WCharacter.h
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-11
 * @version 2022-02-19
 */
#ifndef __WCHARACTER_H__
#define __WCHARACTER_H__

#include <ctype.h>


static inline bool isAlphaNumeric(const int) __attribute__((always_inline, unused));
static inline bool isAlphaNumeric(const int c) {
	return isalnum(c) != 0;
}


static inline bool isAlpha(const int) __attribute__((always_inline, unused));
static inline bool isAlpha(const int c) {
	return isalpha(c) != 0;
}


static inline bool isAscii(const int) __attribute__((always_inline, unused));
static inline bool isAscii(const int c) {
	return c <= 0x7F;
}


static inline bool isWhitespace(const int) __attribute__((always_inline, unused));
static inline bool isWhitespace(const int c) {
	return c == ' ' || c == '\t';
}


static inline bool isControl(const int) __attribute__((always_inline, unused));
static inline bool isControl(const int c) {
	return iscntrl(c) != 0;
}


static inline bool isDigit(const int) __attribute__((always_inline, unused));
static inline bool isDigit(const int c) {
	return isdigit(c) != 0;
}


static inline bool isGraph(const int) __attribute__((always_inline, unused));
static inline bool isGraph(const int c) {
	return isgraph(c) != 0;
}


static inline bool isLowerCase(const int) __attribute__((always_inline, unused));
static inline bool isLowerCase(const int c) {
	return islower(c) != 0;
}


static inline bool isPrintable(const int) __attribute__((always_inline, unused));
static inline bool isPrintable(const int c) {
	return isprint(c) != 0;
}


static inline bool isPunct(const int) __attribute__((always_inline, unused));
static inline bool isPunct(const int c) {
	return ispunct(c) != 0;
}


static inline bool isSpace(const int) __attribute__((always_inline, unused));
static inline bool isSpace(const int c) {
	return isspace(c) != 0;
}


static inline bool isUpperCase(const int) __attribute__((always_inline, unused));
static inline bool isUpperCase(const int c) {
	return isupper(c) != 0;
}


static inline bool isHexadecimalDigit(const int) __attribute__((always_inline, unused));
static inline bool isHexadecimalDigit(const int c) {
	return isxdigit(c) != 0;
}


static inline int toAscii(const int) __attribute__((always_inline, unused));
static inline int toAscii(const int c) {
	return c & 0x7F;
}


static inline int toLowerCase(const int) __attribute__((always_inline, unused));
static inline int toLowerCase(const int c) {
	return tolower(c);
}


static inline int toUpperCase(const int) __attribute__((always_inline, unused));
static inline int toUpperCase(const int c) {
	return toupper(c);
}


#endif /* __WCHARACTER_H__ */
