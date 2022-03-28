/**
 * @file main.c
 * @author Daniel Starke
 * @copyright Copyright 2022 Daniel Starke
 * @date 2022-03-28
 * @version 2022-03-28
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "serial.h"

#define BUFFER_SIZE 2097152

/**
 * Main entry point.
 * Expects a serial interface as single argument.
 * 
 * @param[in] argc - argument count 
 * @param[in] args - argument list
 */
int main(int argc, char ** args) {
	int ec = EXIT_FAILURE;
	if (argc <= 1) return ec;
	uint8_t * buffer = NULL;
	tSerial * ser = ser_create(args[1], 115200, SFR_8N1, SFC_NONE);
	if (ser == NULL) goto onError;
	buffer = malloc(BUFFER_SIZE);
	if (buffer == NULL) goto onError;
	
	/* give the device some time for channel initialization */
	sleep(1);
	
	/* transmission speed */
	{
		for (uint8_t i = 0; i < 64; i++) buffer[(size_t)i] = i;
		for (size_t i = 0; i < 16384; i++) {
			for (size_t rem = 64, try = 0; rem > 0; try++) {
				if (try > 5) {
					fprintf(stderr, "Error: Failed to send data on %s.\n", args[1]);
					goto onError;
				}
				const ssize_t sent = ser_write(ser, buffer + 64 - rem, rem, 100);
				if (sent > 0) rem = (size_t)(rem - (size_t)sent);
			}
		}
	}
	
	/* reception speed */
	{
		size_t len = 0;
		do {
			const ssize_t i = ser_read(ser, buffer + len, BUFFER_SIZE - len, 10);
			if (i > 0) len += (size_t)i;
			buffer[len] = 0;
		} while (len < BUFFER_SIZE && strstr((char *)buffer, "done\r\n") == NULL);
		printf("<<received %i bytes>>\n%.*s\n", (int)len, (int)len, (char *)buffer);
	}
	
	ec = EXIT_SUCCESS;
onError:
	if (buffer != NULL) free(buffer);
	if (ser != NULL) ser_delete(ser);
	return ec;
}
