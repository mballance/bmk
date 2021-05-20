/*
 * bmk_int_sys.c
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */
#include "bmk_int_sys.h"

const uint32_t sizeof_bmk_core_data_s = sizeof(bmk_core_data_t);
const uint32_t main_sp_offset = (uint32_t)&((bmk_core_data_t *)0)->main_sp;
const uint32_t main_f_offset = (uint32_t)&((bmk_core_data_t *)0)->main_f;
bmk_core_data_t				core_data[BMK_MAX_CORES];

bmk_core_data_t *bmk_sys_get_core_data(void) {
	return &core_data[bmk_get_procid()];
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
