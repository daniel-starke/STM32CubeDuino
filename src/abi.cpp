/**
 * @file abi.cpp
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-05-17
 * @version 2020-05-17
 */
#include <stdlib.h>


extern void systemErrorHandler() __attribute__((weak, noreturn));


void __cxa_pure_virtual(void) {
	systemErrorHandler();
}


void __cxa_deleted_virtual(void) {
	systemErrorHandler();
}


void * operator new(size_t size) {
	return malloc(size);
}


void * operator new[](size_t size) {
	return malloc(size);
}


void * operator new(size_t, void * ptr) noexcept {
	return ptr;
}


void * operator new[](size_t, void * ptr) noexcept {
	return ptr;
}


void operator delete(void *ptr) {
	free(ptr);
}


void operator delete[](void *ptr) {
	free(ptr);
}


void operator delete(void * ptr, size_t /* size */) {
	free(ptr);
}


void operator delete[](void * ptr, size_t /* size */) {
	free(ptr);
}
