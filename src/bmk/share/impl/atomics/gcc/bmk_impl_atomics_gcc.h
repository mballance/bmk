/*
 * bmk_impl_atomics_gcc.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_IMPL_ATOMICS_GCC_H
#define INCLUDED_BMK_IMPL_ATOMICS_GCC_H
#include <stdint.h>
typedef uint32_t bmk_atomic_t;
#include "bmk_int_atomics.h"

/**
 * Internal function to initialize a memory location used for locking
 */
static inline void bmk_atomics_init(bmk_atomic_t *ptr) {
	*ptr = 0;
}

/**
 * Internal function to lock a memory location
 */
static inline void bmk_atomics_lock(bmk_atomic_t *ptr) {
//	fprintf(stdout, "--> bmk_atomics_lock(%p)\n", ptr);
//	fflush(stdout);
	while (!__sync_bool_compare_and_swap(ptr, 0, 1)) { ; }
//	fprintf(stdout, "<-- bmk_atomics_lock(%p)\n", ptr);
//	fflush(stdout);
}

/**
 * Internal function to unlock a memory location
 */
static inline void bmk_atomics_unlock(bmk_atomic_t *ptr) {
//	fprintf(stdout, "--> bmk_atomics_unlock(%p)\n", ptr);
//	fflush(stdout);
//	while (!__sync_bool_compare_and_swap(ptr, 1, 0)) { ; }
	*ptr = 0;
//	fprintf(stdout, "<-- bmk_atomics_unlock(%p)\n", ptr);
//	fflush(stdout);
}

static inline uint32_t bmk_atomics_compare_and_swap(bmk_atomic_t *ptr, uint32_t oldval, uint32_t newval) {
	return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

#endif /* INCLUDED_BMK_IMPL_ATOMICS_GCC_H */
