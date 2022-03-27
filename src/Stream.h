/**
 * @file Stream.h
 * @author Daniel Starke
 * @copyright Copyright 2019-2020 Daniel Starke
 * @date 2019-03-10
 * @version 2020-05-17
 */
#ifndef __STREAM_H__
#define __STREAM_H__

#include "Print.h"


enum LookaheadMode{
    SKIP_ALL,
    SKIP_NONE,
    SKIP_WHITESPACE
};


#define NO_IGNORE_CHAR 1


class Stream : public Print {
protected:
	struct MultiTarget {
		const char * str;
		size_t len;
		size_t index;
	};
	
	unsigned long _timeout; /**< in milliseconds */
	unsigned long _startMillis;
public:
	Stream(void);

	virtual int available(void) = 0;
	virtual int read(void) = 0;
	virtual int peek(void) = 0;

	void setTimeout(unsigned long timeout);
	unsigned long getTimeout(void);

	bool find(char * target);
	bool find(uint8_t * target);
	bool find(char * target, size_t length);
	bool find(uint8_t * target, size_t length);
	bool find(char target);

	bool findUntil(char * target, char * terminator);
	bool findUntil(uint8_t * target, char * terminator);
	bool findUntil(char * target, size_t targetLen, char * terminator, size_t termLen);
	bool findUntil(uint8_t * target, size_t targetLen, char * terminator, size_t termLen);

	long parseInt(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);
	float parseFloat(LookaheadMode lookahead = SKIP_ALL, char ignore = NO_IGNORE_CHAR);

	size_t readBytes(char * buffer, size_t length);
	size_t readBytes(uint8_t * buffer, size_t length);

	size_t readBytesUntil(char terminator, char * buffer, size_t length);
	size_t readBytesUntil(char terminator, uint8_t * buffer, size_t length);

	String readString(void);
	String readStringUntil(char terminator);
protected:
	int timedRead(void);
	int timedPeek(void);
	int peekNextDigit(LookaheadMode lookahead, bool detectDecimal);
	long parseInt(char ignore);
	float parseFloat(char ignore);
	int findMulti(struct MultiTarget * targets, int tCount);
};


#undef NO_IGNORE_CHAR


#endif /* __STREAM_H__ */
