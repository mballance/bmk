/*
 * bmk_sys_services.c
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#include "bmk_sys_services.h"
#include "bmk_int_scheduler.h"
#include "bmk_int_sys.h"

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
		bmk_sys_data.core_data[i].procid = i;
		bmk_sys_data.core_data[i].active_thread = 0;
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

void bmk_set_bmk_main_func(bmk_main_f func) {
	prv_main_func = func;
}

void __attribute__((weak)) bmk_main(void)  {
	if (prv_main_func) {
		prv_main_func();
	}
}

