/**
 * @file new.h
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-05-17
 * @version 2020-10-06
 */
#ifdef __cplusplus
#ifndef __NEW_H__
#define __NEW_H__

#include <stdlib.h>


void * operator new(size_t size);
void * operator new[](size_t size);
#ifndef STM32CUBEDUINO_LEGACY_API
void * operator new(size_t, void * ptr) noexcept;
void * operator new[](size_t, void * ptr) noexcept;
#endif /* STM32CUBEDUINO_LEGACY_API */
void operator delete(void * ptr);
void operator delete[](void * ptr);
void operator delete(void * ptr, size_t /* size */);
void operator delete[](void * ptr, size_t /* size */);
extern "C" void __cxa_pure_virtual(void) __attribute__((__noreturn__));
extern "C" void __cxa_deleted_virtual(void) __attribute__((__noreturn__));


#endif /* __NEW_H__ */
#endif /* __cplusplus */
