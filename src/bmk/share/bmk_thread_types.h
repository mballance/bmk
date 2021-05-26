/****************************************************************************
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
 * bmk_thread_types.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 ****************************************************************************/

#ifndef INCLUDED_BMK_THREAD_TYPES_H
#define INCLUDED_BMK_THREAD_TYPES_H
#include <stdint.h>
#include "bmk_int_atomics.h"
#include "bmk_config.h"

typedef int32_t (*bmk_thread_main_f)(void *ud);
typedef struct bmk_cpuset_s {
	uint32_t		mask[((BMK_MAX_CORES-1)/32)+1];
} bmk_cpuset_t;

#include "bmk_int_context.h"
#include "bmk_impl_scheduler.h"

typedef struct bmk_thread_core_s {
} bmk_thread_core_t;

typedef struct bmk_thread_s {
	bmk_context_t					ctxt;
	bmk_thread_main_f				main_f;
	void							*main_ud;
	uint32_t						alive;

	// Data used by the scheduler implementation
	bmk_thread_scheduler_data_t		sched_data;

	// This next pointer is used by the thread-services
	// routines, and must not be used for other
	// purposes while the thread is alive
	struct bmk_thread_s				*next;
} bmk_thread_t;

// TODO:
typedef struct bmk_mutex_s {
	bmk_atomic_t			lock;
	bmk_thread_t			*owner;
	bmk_thread_t			*waiters;
} bmk_mutex_t;

typedef struct bmk_cond_s {
	bmk_atomic_t			lock;
	bmk_thread_t			*waiters;
} bmk_cond_t;

typedef struct bmk_event_s {
	bmk_atomic_t			event;
} bmk_event_t;


#endif /* INCLUDED_BMK_THREAD_TYPES_H */

