/**
 * @file WMath.cpp
 * @author Daniel Starke
 * @copyright Copyright 2019-2020 Daniel Starke
 * @date 2019-03-10
 * @version 2020-06-05
 */
#include <limits.h>
#include "Arduino.h"


/** Value used to calculate the next random number. */
static unsigned long randomNext = 1;



/**
 * Returns a random number between 0 and max - 1.
 * 
 * @param[in] _max - not including maximum number
 * @return random number
 * @remarks Implementation in alignment with avr-libc-2.0.0.
 */
long random(long _max) {
	/* Map invalid zero to another value. */
	if (randomNext == 0) randomNext = 123459876UL;
	/* x = (7^5 * x) mod (2^31 - 1) without overflow bits (2^31 - 1) = 127773 * (7^5) + 2836 */
	long val = static_cast<long>(randomNext);
	const long quot = val / 127773L;
	const long rem = val % 127773L;
	val = (16807L * rem) - (2836L * quot);
	if (val < 0) val += 0x7FFFFFFFL;
	randomNext = static_cast<unsigned long>(val);
	return (val % static_cast<long>((static_cast<unsigned long>(RANDOM_MAX) + 1L))) % _max;
}


/**
 * Returns a random number between min and max - 1.
 * 
 * @param[in] _min - including minimum number
 * @param[in] _max - not including maximum number
 * @return random number
 */
long random(long _min, long _max) {
	if (_min >= _max) return _min;
	return random(_max - _min) + _min;
}


/**
 * Changes the seed for the random number generator.
 * 
 * @param[in] seed - new seed
 */
void randomSeed(unsigned long seed) {
  if (seed != 0) randomNext = seed;
}


/**
 * Re-maps a number from one range to another. That means val is mapped from [fromLow, fromHigh] to
 * [toLow, toHigh].
 * 
 * @param[in] val - value to map
 * @param[in] fromLow - input range lower boundary
 * @param[in] fromHigh - input range upper boundary
 * @param[in] toLow - output range lower boundary
 * @param[in] toHigh - output range upper boundary
 * @return mapped value
 */
long map(long val, long fromLow, long fromHigh, long toLow, long toHigh) {
	return (((val - fromLow) * (toHigh - toLow)) / (fromHigh - fromLow)) + toLow;
}


/**
 * Returns a word from the given value.
 * 
 * @param[in] w - input value
 * @return word of w
 */
uint16_t makeWord(uint16_t w) {
	return w;
}


/**
 * Returns a word of the given values.
 * 
 * @param[in] h - lower order byte
 * @param[in] l - higher order byte
 * @return word of h and l
 */
uint16_t makeWord(uint8_t h, uint8_t l) {
	return static_cast<uint16_t>((h << 8) | l);
}
