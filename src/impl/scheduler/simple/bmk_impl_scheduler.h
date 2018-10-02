/*
 * bmk_impl_scheduler.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_IMPL_SCHEDULER_H
#define INCLUDED_BMK_IMPL_SCHEDULER_H
#include <stdint.h>

struct bmk_thread_s;
struct bmk_event_s;

typedef enum {
	RunStateIdle,
	RunStateRunning,
	RunStateBlocked,
	RunStateDead
} run_state_e;

typedef struct bmk_thread_scheduler_data_s {
	struct bmk_thread_s			*next;
	run_state_e					state;
	struct bmk_event_s			*event;
	uint32_t					refcnt;
} bmk_thread_scheduler_data_t;




#endif /* INCLUDED_BMK_IMPL_SCHEDULER_H */
