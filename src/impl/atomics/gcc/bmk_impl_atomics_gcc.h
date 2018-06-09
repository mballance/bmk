/*
 * bmk_impl_atomics_gcc.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_ATOMICS_GCC_BMK_IMPL_ATOMICS_GCC_H_
#define SRC_IMPL_ATOMICS_GCC_BMK_IMPL_ATOMICS_GCC_H_
#include <stdint.h>
#include <stdio.h>

/**
 * Internal function to initialize a memory location used for locking
 */
static inline void bmk_atomics_init(uint32_t *ptr) {
	*ptr = 0;
}

/**
 * Internal function to lock a memory location
 */
static inline void bmk_atomics_lock(uint32_t *ptr) {
//	fprintf(stdout, "--> bmk_atomics_lock(%p)\n", ptr);
//	fflush(stdout);
	while (!__sync_bool_compare_and_swap(ptr, 0, 1)) { ; }
//	fprintf(stdout, "<-- bmk_atomics_lock(%p)\n", ptr);
//	fflush(stdout);
}

/**
 * Internal function to unlock a memory location
 */
static inline void bmk_atomics_unlock(uint32_t *ptr) {
//	fprintf(stdout, "--> bmk_atomics_unlock(%p)\n", ptr);
//	fflush(stdout);
	while (!__sync_bool_compare_and_swap(ptr, 1, 0)) { ; }
//	fprintf(stdout, "<-- bmk_atomics_unlock(%p)\n", ptr);
//	fflush(stdout);
}


#endif /* SRC_IMPL_ATOMICS_GCC_BMK_IMPL_ATOMICS_GCC_H_ */
