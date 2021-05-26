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
 * bmk_impl_scheduler.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 *
 ****************************************************************************/

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
	uint32_t					coreid;
} bmk_thread_scheduler_data_t;

typedef struct bmk_scheduler_impl_data_s {
	struct bmk_thread_s			*active;
//	struct bmk_thread_s			main_thread;
} bmk_scheduler_impl_data_t;


#endif /* INCLUDED_BMK_IMPL_SCHEDULER_H */
