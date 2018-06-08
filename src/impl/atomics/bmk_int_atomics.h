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
#ifdef __cplusplus

/**
 * Internal function to initialize a memory location used for locking
 */
void bmk_atomics_init(uint32_t *ptr);

/**
 * Internal function to lock a memory location
 */
void bmk_atomics_lock(uint32_t *ptr);

/**
 * Internal function to unlock a memory location
 */
void bmk_atomics_unlock(uint32_t *ptr);

#ifdef __cplusplus
}
#ifdef __cplusplus

#endif /* SRC_IMPL_ATOMICS_BMK_INT_ATOMICS_H_ */
