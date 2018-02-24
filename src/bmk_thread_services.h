/*
 * bmk_thread_services.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_THREAD_SERVICES_H
#define INCLUDED_BMK_THREAD_SERVICES_H
#include <stdint.h>
#include "bmk_thread_types.h"
#include "bmk_thread_services_target.h"

#ifdef __cplusplus
extern "C" {
#endif


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
 * Causes the current core to select and run another
 * thread if one exists
 */
void bmk_thread_yield(void);

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

void bmk_cond_wait(bmk_cond_t *c, bmk_muex_t *m);

void bmk_cond_signal(bmk_cond_t *c);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDED_BMK_THREAD_SERVICES_H */
