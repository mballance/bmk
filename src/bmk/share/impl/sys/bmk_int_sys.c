/*
 * bmk_int_sys.c
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */
#include "bmk_int_sys.h"
#include "bmk_int_scheduler.h"

void bmk_entry(bmk_core_data_t *core_data) {
	if (core_data->coreid == 0) {
		// TODO: handle any global initialization
	}

	core_data_p[core_data->coreid] = core_data;

	// Initialize the per-core scheduler data
	bmk_scheduler_init(&core_data->sched_data);

	// Enable interrupts before calling the entry function
	bmk_sys_enable_interrupts();

	// Finally, call the user-main function
	core_data->main_f(core_data->coreid);

	// Ensure we actually return through this function
	asm("");
}

void bmk_sys_poweroff(void) {
	bmk_sys_emit("[POWEROFF]\n");
	bmk_sys_exit(1);
}

void __attribute__((weak)) bmk_sys_emit(const char *str) {
	// Default is to not do anything
}

void __attribute__((weak)) bmk_sys_exit(int code) {
	// Default is to not do anything
}
