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
 *
 * bmk_int_atomics.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 ****************************************************************************/

#ifndef INCLUDED_BMK_INT_ATOMICS_H
#define INCLUDED_BMK_INT_ATOMICS_H
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

// Performs an atomic swap if [ptr] == oldval
uint32_t bmk_atomics_compare_and_swap(bmk_atomic_t *ptr, uint32_t oldval, uint32_t newval);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_BMK_INT_ATOMICS_H */
