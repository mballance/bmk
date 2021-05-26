/*****************************************************************************
 *
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
 * bmk_sys_services.c
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 *
 *****************************************************************************/

#include "bmk_sys_services.h"
#include "bmk_int_scheduler.h"
#include "bmk_int_sys.h"
#include "bmk_msg_services.h"

static bmk_main_f			prv_level0_main_func = 0;
static bmk_level1_main_f	prv_level1_main_func = 0;
static bmk_main_f			prv_main_func = 0;
//bmk_sys_data_t				bmk_sys_data;

const uint32_t sizeof_bmk_core_data_s = sizeof(bmk_core_data_t);
const uint32_t main_sp_offset = (uint32_t)&((bmk_core_data_t *)0)->main_sp;
const uint32_t main_f_offset = (uint32_t)&((bmk_core_data_t *)0)->main_f;
const uint32_t coreid_offset = (uint32_t)&((bmk_core_data_t *)0)->coreid;
bmk_core_data_t				core_data[BMK_MAX_CORES] = {0};
bmk_core_data_t				*core_data_p[BMK_MAX_CORES] = {0};

void bmk_sys_init(void) {
	int i;

//	for (i=0; i<((BMK_MAX_CORES-1)/32)+1; i++) {
//		bmk_sys_data.core_release_mask[i] = 0;
//	}
//	bmk_sys_data.core_release_mask[i] = 1;

}

void __attribute__((weak)) bmk_hardware_init(void) {

}

void __attribute__((weak)) bmk_core0_init(void) {

}

bmk_core_data_t *bmk_sys_get_core_data(void) {
	return core_data_p[bmk_get_procid()];
}

void bmk_set_level0_main_func(bmk_main_f f) {
	prv_level0_main_func = f;
}

void __attribute__((weak)) bmk_level0_main(void) {
	if (prv_level0_main_func) {
		prv_level0_main_func();
	}
}

void bmk_init_core(
		uint32_t cid,
		void (*main_f)(uint32_t),
		void *stk, uint32_t stk_sz) {
	bmk_cpuset_t mask;

	bmk_cpuset_zero(&mask);
	core_data[cid].main_f = main_f;
	core_data[cid].main_sp = stk + stk_sz - sizeof(void *);

	bmk_cpuset_set(cid, &mask);

	// Notify core that it can run
	bmk_sys_send_proc_event(&mask);
}

void bmk_set_level1_main_func(bmk_level1_main_f f) {
	prv_level1_main_func = f;
}

void __attribute__((weak)) bmk_level1_main(uint32_t cid) {
	// Call the user's level1 function if desired
	// Otherwise, start up the scheduler
	if (prv_level1_main_func) {
		prv_level1_main_func(cid);
	} else {
		if (cid == 0) {
			bmk_main();
		} else {
			bmk_scheduler_nonprimary();
		}
	}
}

// Assembly-call doesn't seem to teal with weak symbols
void _bmk_level1_main(uint32_t cid) {
	if (cid == 0) {
		// Initialize the scheduler before unlocking non-primary cores
		// TODO:
		bmk_scheduler_init(0);

	}

	bmk_level1_main(cid);
}
//	bmk_core_data_t *core = bmk_sys_get_core_data();
//	bmk_context_getcontext(&core->main_thread.ctxt);
//
//	// Initially pin this thread to the relevant processor
//	core->main_thread.procmask = (1 << bmk_get_procid());
//	// Register this thread with the schedule as a running thread
//	bmk_scheduler_thread_new(&core->main_thread, 1);
////	core->active_thread = &core->main_thread;
//
//	bmk_level1_main(cid);
//}

void bmk_set_bmk_main_func(bmk_main_f func) {
	prv_main_func = func;
}

void __attribute__((weak)) bmk_main(void)  {
	if (prv_main_func) {
		prv_main_func();
	}
}

void bmk_sys_set_irq_handler(bmk_irq_f handler) {
	// TODO:
//	bmk_sys_get_core_data()->irq_handler = handler;
}

// Called by the scheduler when a thread is no longer in use
void __attribute__((weak)) bmk_thread_free(bmk_thread_t *t) {
	// By default, do nothing
}
