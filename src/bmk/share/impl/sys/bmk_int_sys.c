/*
 * bmk_int_sys.c
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */
#include "bmk_int_sys.h"


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
