/*
 * bmk_thread_services_target.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_THREAD_SERVICES_TARGET_PTHREAD_H
#define INCLUDED_BMK_THREAD_SERVICES_TARGET_PTHREAD_H
#include "bmk_thread_types.h"
#include <pthread.h>
#include <ucontext.h>

typedef struct bmk_thread_pthread_s {
	// We want our own user threads for this
	ucontext_t					ctxt;
	bmk_thread_main_f			main_f;
	void						*main_ud;
	struct bmk_thread_core_s	*next; //
} bmk_thread_pthread_t;

typedef struct bmk_mutex_pthread_s {

} bmk_mutex_pthread_t;

typedef struct bmk_cond_pthread_s {

} bmk_cond_pthread_t;

typedef bmk_thread_pthread_t bmk_thread_t;
typedef bmk_mutex_pthread_t bmk_mutex_t;
typedef bmk_cond_pthread_t bmk_cond_t;


#endif /* INCLUDED_BMK_THREAD_SERVICES_TARGET_RISCV_H */
