/**
 * @file atomic.h
 * @author Daniel Starke
 * @copyright Copyright 2020 Daniel Starke
 * @date 2020-07-12
 * @version 2020-07-12
 */
#ifndef __UTIL_ATOMIC_H__
#define __UTIL_ATOMIC_H__

#include <stdint.h>
#include <board.hpp>


#ifndef __DOXYGEN__
/* Internal helper functions. */
extern "C" {
/**
 * Returns the interrupt state.
 * 
 * @return 0 if no interrupts are currently enabled, else 1
 */
static __inline__ uint8_t __iSREG(void) {
	__DSB();
	return uint8_t(__get_PRIMASK() & 0x1);
}


/**
 * Enables interrupts and returns 1.
 * 
 * @return 1
 */
static __inline__ uint8_t __iSeiRetVal(void) {
	__enable_irq();
	__ISB();
	return 1;
}


/**
 * Disables interrupts and returns 1.
 * 
 * @return 1
 */
static __inline__ uint8_t __iCliRetVal(void) {
	__disable_irq();
	return 1;
}

/**
 * Unconditionally re-enabled interrupts.
 * 
 * @param[in] wasDisabled - previous interrupt enabled state
 */
static __inline__ void __iSeiParam(const uint8_t * wasDisabled) {
	__enable_irq();
	__ISB();
	(void)wasDisabled;
}

/**
 * Unconditionally re-disabled interrupts.
 * 
 * @param[in] wasDisabled - previous interrupt enabled state
 */
static __inline__ void __iCliParam(const uint8_t * wasDisabled) {
	__disable_irq();
	__DSB();
	(void)wasDisabled;
}

/**
 * Conditionally re-enables interrupts if they were previously enabled.
 * 
 * @param[in] wasDisabled - previous interrupt enabled state
 */
static __inline__ void __iRestore(const  uint8_t * wasDisabled) {
	if (*wasDisabled == 0) {
		__enable_irq();
		__ISB();
	}
}
} /* extern "C" */
#endif /* !__DOXYGEN__ */


#define ATOMIC_BLOCK(type)     for (type, __ToDo = __iCliRetVal(); __ToDo; __ToDo = 0)
#define NONATOMIC_BLOCK(type)  for (type, __ToDo = __iSeiRetVal(); __ToDo; __ToDo = 0)
#define ATOMIC_RESTORESTATE    uint8_t sreg_save __attribute__((__cleanup__(__iRestore)))  = __iSREG()
#define ATOMIC_FORCEON         uint8_t sreg_save __attribute__((__cleanup__(__iSeiParam))) = 0
#define NONATOMIC_RESTORESTATE uint8_t sreg_save __attribute__((__cleanup__(__iRestore)))  = __iSREG()
#define NONATOMIC_FORCEOFF     uint8_t sreg_save __attribute__((__cleanup__(__iCliParam))) = 0


#endif /* __UTIL_ATOMIC_H__ */
