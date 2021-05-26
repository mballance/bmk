/****************************************************************************
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
 *
 *
 * bmk_int_sys.c
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 *
 ****************************************************************************/
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
