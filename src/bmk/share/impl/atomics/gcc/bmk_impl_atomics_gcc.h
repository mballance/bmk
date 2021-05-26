/****************************************************************************
 *
 * Copyright 2018-2021 Matthew Ballance
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * bmk_impl_atomics_gcc.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 ****************************************************************************/

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
	// Spin as long as the previous value was non-zero
	// Once we get a previous value of zero, we know
	// we've successfully locked
	while (__sync_lock_test_and_set(ptr, 1)) { ; }
}

/**
 * Internal function to unlock a memory location
 */
static inline void bmk_atomics_unlock(bmk_atomic_t *ptr) {
	__sync_lock_test_and_set(ptr, 0);
}

static inline uint32_t bmk_atomics_compare_and_swap(bmk_atomic_t *ptr, uint32_t oldval, uint32_t newval) {
	return __sync_bool_compare_and_swap(ptr, oldval, newval);
}

#endif /* INCLUDED_BMK_IMPL_ATOMICS_GCC_H */
