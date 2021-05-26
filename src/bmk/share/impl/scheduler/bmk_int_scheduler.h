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
 * bmk_int_scheduler.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 ****************************************************************************/

#ifndef INCLUDED_BMK_INT_SCHEDULER_H
#define INCLUDED_BMK_INT_SCHEDULER_H
#include "bmk_impl_scheduler.h"
#include "bmk_thread_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef struct bmk_thread_t;
typedef struct bmk_scheduler_data_s {
	bmk_scheduler_impl_data_t		impl_data;
} bmk_scheduler_data_t;

/**
 * Internal function called to initialize the scheduler
 * This function will be called as each core starts up
 */
void bmk_scheduler_init(bmk_scheduler_data_t *sched);

bmk_thread_t *bmk_scheduler_active_thread();

/**
 * Internal function to add a new thread to the scheduler
 */
void bmk_scheduler_thread_new(bmk_thread_t *t);

/**
 * Internal function to notify the scheduler that a thread has exited.
 * This function does not return
 */
void bmk_scheduler_thread_exit(bmk_thread_t *t);

void bmk_scheduler_thread_join(bmk_thread_t *t);

void bmk_scheduler_thread_setaffinity(bmk_thread_t *t, bmk_cpuset_t *cpuset);

/**
 * Blocks the active thread.
 * - lock (optional) must be locked on entry and unlocked within _block
 */
void bmk_scheduler_thread_block(
		bmk_atomic_t 		*lock);

void bmk_scheduler_thread_unblock(bmk_thread_t *t);

void bmk_scheduler_reschedule(uint32_t wait);

void bmk_scheduler_event_wait(bmk_event_t *e);

void bmk_scheduler_event_signal(bmk_event_t *e);

/**
 * Main function for a non-primary core
 */
void bmk_scheduler_nonprimary(void);

#ifdef __cplusplus
}
#endif


#endif /* INCLUDED_BMK_INT_SCHEDULER_H */
