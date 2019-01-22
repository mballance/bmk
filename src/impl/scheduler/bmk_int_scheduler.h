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

/**
 * Internal function called to initialize the scheduler
 * This function must be called from the primary core
 */
void bmk_scheduler_init(void);

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
 * Blocks the active thread
 */
void bmk_scheduler_thread_block(bmk_atomic_t *lock);

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
