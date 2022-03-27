/**
 * @file dtostrf.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020-2022 Daniel Starke
 * @date 2020-05-21
 * @version 2022-02-08
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dtostrf.h"


char * dtostrf(double val, signed char width, unsigned char prec, char * sout) {
#ifdef NO_GPL
	(void)val; (void)width; (void)prec; (void)sout;
	return NULL;
#else /* the following code is taken from the original Arduino library and is licensed under the LGPL 2.1 */
#warning This library uses LGPL 2.1 code. Build with NO_GPL to disable relevant parts.
	/* handle negative numbers */
	uint8_t negative = 0;
	if (val < 0.0) {
		negative = 1;
		val = -val;
	}

	/* round correctly so that print(1.999, 2) prints as "2.00" */
	double rounding = 0.5;
	for (int i = 0; i < prec; ++i) {
		rounding /= 10.0;
	}
	val += rounding;

	/* extract the integer part of the number */
	const unsigned long intPart = static_cast<unsigned long>(val);
	double remainder = val - double(intPart);

	if (prec > 0) {
		/* extract digits from the remainder */
		unsigned long decPart = 0;
		double decade = 1.0;
		for (int i = 0; i < prec; i++) {
			decade *= 10.0;
		}
		remainder *= decade;
		decPart = int(remainder);
	
		if (negative != 0) {
			sprintf(sout, "-%ld.%0*ld", intPart, prec, decPart);
		} else {
			sprintf(sout, "%ld.%0*ld", intPart, prec, decPart);
		}
	} else {
		if (negative != 0) {
			sprintf(sout, "-%ld", intPart);
		} else {
			sprintf(sout, "%ld", intPart);
		}
	}
	/* handle minimum field width of the output string */
	/* width is signed value, negative for left adjustment */
	/* range: -128, 127 */
	char fmt[129] = "";
	unsigned int w = width;
	if (width < 0) {
		negative = 1;
		w = -width;
	} else {
		negative = 0;
	}

	if (strlen(sout) < w) {
		memset(fmt, ' ', 128);
		fmt[w - strlen(sout)] = 0;
		if (negative == 0) {
			char * tmp = static_cast<char *>(malloc(strlen(sout) + 1));
			strcpy(tmp, sout);
			strcpy(sout, fmt);
			strcat(sout, tmp);
			free(tmp);
		} else {
			/* left adjustment */
			strcat(sout, fmt);
		}
	}

	return sout;
#endif
}
