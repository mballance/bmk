/*
 * bmk_sys_services.c
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#include "bmk_sys_services.h"
#include "bmk_int_scheduler.h"
#include "bmk_int_sys.h"
#include "bmk_msg_services.h"

static bmk_main_f			prv_level0_main_func = 0;
static bmk_level1_main_f	prv_level1_main_func = 0;
static bmk_main_f			prv_main_func = 0;
bmk_sys_data_t				bmk_sys_data;

uint32_t					c0_ready_key;
// Stack pointers for each core
void						*core_stack[BMK_MAX_CORES];
uint32_t					core_stack_sz[BMK_MAX_CORES];

void bmk_sys_init(void) {
	int i;

	c0_ready_key = 0;
//	for (i=0; i<((BMK_MAX_CORES-1)/32)+1; i++) {
//		bmk_sys_data.core_release_mask[i] = 0;
//	}
//	bmk_sys_data.core_release_mask[i] = 1;

	for (i=0; i<BMK_MAX_CORES; i++) {
		core_stack[i] = 0;
		core_stack_sz[i] = 0;
		bmk_sys_data.core_data[i].procid = i;
		bmk_sys_data.core_data[i].active_thread = 0;
		bmk_sys_data.core_data[i].irq_handler = 0;
	}
}

void __attribute__((weak)) bmk_hardware_init(void) {

}

void __attribute__((weak)) bmk_core0_init(void) {

}

void bmk_set_level0_main_func(bmk_main_f f) {
	prv_level0_main_func = f;
}

void __attribute__((weak)) bmk_level0_main(void) {
	if (prv_level0_main_func) {
		prv_level0_main_func();
	}
}

void bmk_init_core(uint32_t cid, void *stk, uint32_t stk_sz) {
	core_stack[cid] = stk;
	core_stack_sz[cid] = stk_sz;
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
			bmk_scheduler_init();
			bmk_main();
		} else {
			bmk_scheduler_nonprimary();
		}
	}
}

// Assembly-call doesn't seem to teal with weak symbols
void _bmk_level1_main(uint32_t cid) {
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
	bmk_sys_get_core_data()->irq_handler = handler;
}

// Called by the scheduler when a thread is no longer in use
void __attribute__((weak)) bmk_thread_free(bmk_thread_t *t) {
	// By default, do nothing
}
