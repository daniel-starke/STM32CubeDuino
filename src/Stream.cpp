/**
 * @file Stream.cpp
 * @author Daniel Starke
 * @copyright Copyright 2019-2022 Daniel Starke
 * @date 2019-03-10
 * @version 2022-04-17
 */
#include "Arduino.h"
#include "Stream.h"


Stream::Stream(void):
	_timeout(1000)
{}


void Stream::setTimeout(unsigned long timeout) {
	this->_timeout = timeout;
}


unsigned long Stream::getTimeout(void) {
	return this->_timeout;
}


bool Stream::find(char * target) {
	return this->findUntil(target, strlen(target), NULL, 0);
}


bool Stream::find(uint8_t * target) {
	return this->find(reinterpret_cast<char *>(target));
}


bool Stream::find(char * target, size_t length) {
	return this->findUntil(target, length, NULL, 0);
}


bool Stream::find(uint8_t * target, size_t length) {
	return this->find(reinterpret_cast<char *>(target), length);
}


bool Stream::find(char target) {
	return this->find(&target, 1);
}


bool Stream::findUntil(char * target, char * terminator) {
#ifdef NO_GPL
	(void)target; (void)terminator;
	abort();
	return false;
#else /* the following code is taken from the original Arduino library and licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	return this->findUntil(target, strlen(target), terminator, strlen(terminator));
#endif
}


bool Stream::findUntil(uint8_t * target, char * terminator) {
#ifdef NO_GPL
	(void)target; (void)terminator;
	abort();
	return false;
#else /* the following code is taken from the original Arduino library and licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	return this->findUntil(reinterpret_cast<char *>(target), terminator);
#endif
}


bool Stream::findUntil(char * target, size_t targetLen, char * terminator, size_t termLen) {
#ifdef NO_GPL
	(void)target; (void)targetLen; (void)terminator; (void)termLen;
	abort();
	return false;
#else /* the following code is taken from the original Arduino library and licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	if (terminator == NULL) {
		MultiTarget targets[1] = {
			{target, targetLen, 0}
		};
		return this->findMulti(targets, 1) == 0;
	} else {
		MultiTarget targets[2] = {
			{target, targetLen, 0},
			{terminator, termLen, 0}
		};
		return this->findMulti(targets, 2) == 0;
	}
#endif
}


bool Stream::findUntil(uint8_t * target, size_t targetLen, char * terminator, size_t termLen) {
#ifdef NO_GPL
	(void)target; (void)targetLen; (void)terminator; (void)termLen;
	abort();
	return false;
#else /* the following code is taken from the original Arduino library and licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	return this->findUntil(reinterpret_cast<char *>(target), targetLen, terminator, termLen);
#endif
}


long Stream::parseInt(LookaheadMode lookahead, char ignore) {
	int c = this->peekNextDigit(lookahead, false);
	if (c < 0) return 0; /* timeout */

	bool sign = false;
	bool empty = true;
	long val = 0;
	for (;;) {
		if (c == ignore) {
			/* ignored */
		} else if (empty && c == '-') {
			sign = !sign;
		} else if (c >= '0' && c <= '9') {
			val = (val * 10) + (c - '0');
			empty = false;
		} else {
			break;
		}
		this->read(); /* consume character */
		c = this->timedPeek();
	};

	return sign ? -val : val;
}


float Stream::parseFloat(LookaheadMode lookahead, char ignore) {
	int c = this->peekNextDigit(lookahead, true);
	if (c < 0) return 0; /* timeout */

	bool sign = false;
	bool isFrac = false;
	bool empty = true;
	long val = 0;
	float frac = 1.0f;
	for (;;) {
		if (c == ignore) {
			/* ignore */
		} else if (empty && c == '-') {
			sign = !sign;
		} else if (( ! isFrac ) && c == '.') {
			isFrac = true;
			empty = false;
		} else if (c >= '0' && c <= '9') {
			val = (val * 10) + (c - '0');
			if ( isFrac ) frac *= 0.1f;
			empty = false;
		} else {
			break;
		}
		this->read(); /* consume */
		c = this->timedPeek();
	}

	if ( sign ) val = -val;
	if ( isFrac ) return static_cast<float>(val) * frac;
	return static_cast<float>(val);
}


size_t Stream::readBytes(char * buffer, size_t length) {
	size_t count = 0;
	for (; count < length; count++) {
		const int c = this->timedRead();
		if (c < 0) break;
		*buffer++ = static_cast<char>(c);
	}
	return count;
}


size_t Stream::readBytes(uint8_t * buffer, size_t length) {
	return this->readBytes(reinterpret_cast<char *>(buffer), length);
}


size_t Stream::readBytesUntil(char terminator, char * buffer, size_t length) {
	if (length < 1) return 0;
	size_t index = 0;
	for (; index < length; index++) {
		const int c = this->timedRead();
		if (c < 0 || c == terminator) break;
		*buffer++ = static_cast<char>(c);
	}
	return index;
}


size_t Stream::readBytesUntil(char terminator, uint8_t * buffer, size_t length) {
	return this->readBytesUntil(terminator, reinterpret_cast<char *>(buffer), length);
}


#ifndef STM32CUBEDUINO_DISABLE_STRING
String Stream::readString(void) {
	String res;
	for (int c = this->timedRead(); c >= 0; c = this->timedRead()) {
		res += static_cast<char>(c);
	}
	return res;
}


String Stream::readStringUntil(char terminator) {
	String res;
	for (int c = this->timedRead(); c >= 0 && c != terminator; c = this->timedRead()) {
		res += static_cast<char>(c);
	}
	return res;
}
#endif /* not STM32CUBEDUINO_DISABLE_STRING */


int Stream::timedRead(void) {
	this->_startMillis = millis();
	do {
		const int c = this->read();
		if (c >= 0) return c;
	} while ((millis() - this->_startMillis) < this->_timeout);
	return -1;
}


int Stream::timedPeek(void) {
	this->_startMillis = millis();
	do {
		const int c = this->peek();
		if (c >= 0) return c;
	} while((millis() - this->_startMillis) < this->_timeout);
	return -1;
}


int Stream::peekNextDigit(LookaheadMode lookahead, bool detectDecimal) {
	for (;;) {
		const int c = this->timedPeek();
		if (c < 0 || c == '-' || (c >= '0' && c <= '9') || (detectDecimal && c == '.')) return c;

		switch (lookahead) {
		case SKIP_NONE:
			return -1;
		case SKIP_WHITESPACE:
			switch (c) {
			case '\t':
			case '\n':
			case '\r':
			case ' ':
				break;
			default:
				return -1;
			}
		case SKIP_ALL:
			break;
		}
		this->read(); /* discard */
	}
}


long Stream::parseInt(char ignore) {
	return this->parseInt(SKIP_ALL, ignore);
}


float Stream::parseFloat(char ignore) {
	return this->parseFloat(SKIP_ALL, ignore);
}


int Stream::findMulti(MultiTarget * targets, int tCount) {
#ifdef NO_GPL
	(void)targets; (void)tCount;
	abort();
	return -1;
#else /* the following code is taken from the original Arduino library and licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	/* validate target lengths */
	for (const MultiTarget * target = targets; target < (targets + tCount); target++) {
		if (target->len <= 0) return static_cast<int>(target - targets);
	}

	for (;;) {
		const int c = this->timedRead();
		if (c < 0) return -1;

		for (MultiTarget * target = targets; target < (targets + tCount); target++) {
			/* check matches */
			if (c == target->str[target->index]) {
				target->index++;
				if (target->index == target->len) return static_cast<int>(target - targets);
				continue;
			}

			if (target->index == 0) continue;
			
			/* rewind match for alternative sub matches */
			const int origIndex = static_cast<int>(target->index);
			do {
				target->index--;
				if (c != target->str[target->index]) continue;
				if (target->index == 0) {
					target->index++;
					break;
				}

				const int diff = origIndex - static_cast<int>(target->index);
				size_t i = 0;
				for (; i < target->index && target->str[i] == target->str[i + diff]; i++);
				if (i == target->index) {
					target->index++;
					break;
				}
			} while (target->index > 0);
		}
	}

	return -1; /* unreachable */
#endif
}
