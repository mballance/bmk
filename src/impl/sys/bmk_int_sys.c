/*
 * bmk_int_sys.c
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */
#include "bmk_int_sys.h"

bmk_sys_data_t					bmk_sys_data;

void bmk_sys_poweroff(void) {
	bmk_sys_emit("[POWEROFF]\n");
	bmk_sys_exit(1);
}

