/*
 * bmk_int_scheduler.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

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
