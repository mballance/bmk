/*
 * bmk_thread_types.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_THREAD_TYPES_H
#define INCLUDED_BMK_THREAD_TYPES_H
#include <stdint.h>

typedef int32_t (*bmk_thread_main_f)(void *ud);
typedef uint64_t bmk_cpuset_t;

#include "bmk_int_context.h"

typedef struct bmk_thread_core_s {
} bmk_thread_core_t;

typedef struct bmk_thread_s {
	bmk_context_t			ctxt;
	bmk_cpuset_t			procmask;
	bmk_thread_main_f		main_f;
	void					*main_ud;
	uint32_t				alive;
	struct bmk_thread_s		*next;
} bmk_thread_t;

// TODO:
typedef struct bmk_mutex_s {
	uint32_t				lock;
	bmk_thread_t			*owner;
	bmk_thread_t			*waiters;
} bmk_mutex_t;

typedef struct bmk_cond_s {
	uint32_t				lock;
	bmk_thread_t			*waiters;
} bmk_cond_t;


#endif /* INCLUDED_BMK_THREAD_TYPES_H */

