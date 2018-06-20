/*
 * bmk_int_atomics.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_ATOMICS_BMK_INT_ATOMICS_H_
#define SRC_IMPL_ATOMICS_BMK_INT_ATOMICS_H_
#include <stdint.h>
#include "bmk_impl_atomics.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Internal function to initialize a memory location used for locking
 */
void bmk_atomics_init(bmk_atomic_t *ptr);

/**
 * Internal function to lock a memory location
 */
void bmk_atomics_lock(bmk_atomic_t *ptr);

/**
 * Internal function to unlock a memory location
 */
void bmk_atomics_unlock(bmk_atomic_t *ptr);

#ifdef __cplusplus
}
#endif

#endif /* SRC_IMPL_ATOMICS_BMK_INT_ATOMICS_H_ */
