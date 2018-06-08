/*
 * bmk_thread_services.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_THREAD_SERVICES_H
#define INCLUDED_BMK_THREAD_SERVICES_H
#include <stdint.h>
#include "bmk_int_context.h"
#include "bmk_thread_types.h"

#ifdef __cplusplus
extern "C" {
#endif


static inline void bmk_cpuset_set(uint32_t cpu, bmk_cpuset_t *cpuset) {
	*cpuset |= (1 << cpu);
}

static inline uint32_t bmk_cpuset_isset(uint32_t cpu, bmk_cpuset_t *cpuset) {
	return (*cpuset & (1 << cpu));
}

static inline void bmk_cpuset_clr(uint32_t cpu, bmk_cpuset_t *cpuset) {
	*cpuset &= ~(1 << cpu);
}

static inline void bmk_cpuset_zero(bmk_cpuset_t *cpuset) {
	*cpuset = 0;
}


/**
 * bmk_thread_init()
 *
 * Initalizes a thread with a stack and thread-main function
 */
void bmk_thread_init(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud);

/**
 * bmk_thread_init_procmask()
 *
 * Initalizes a thread with a stack and thread-main function
 */
void bmk_thread_init_cpuset(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud,
		bmk_cpuset_t		cpuset);

/**
 * Causes the current core to select and run another
 * thread if one exists
 */
void bmk_thread_yield(void);

/**
 * Wait for the specified thread to end
 */
void bmk_thread_join(bmk_thread_t *t);

/**
 * Moves the thread to the runnable list for core CID
 * If the thread is currently running on another core,
 * then the move may take effect after some time
 */
void bmk_thread_run_on_core(
		bmk_thread_t		*t,
		uint32_t			cid
		);

bmk_thread_t *bmk_thread_self(void);

void bmk_mutex_init(bmk_mutex_t *m);

void bmk_mutex_lock(bmk_mutex_t *m);

void bmk_mutex_unlock(bmk_mutex_t *m);

void bmk_cond_init(bmk_cond_t *c);

void bmk_cond_wait(bmk_cond_t *c, bmk_mutex_t *m);

void bmk_cond_signal(bmk_cond_t *c);

void bmk_cond_signal_all(bmk_cond_t *c);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDED_BMK_THREAD_SERVICES_H */
