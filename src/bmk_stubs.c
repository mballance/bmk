/*
 * bmk_stubs.c
 *
 *  Created on: Jun 20, 2018
 *      Author: ballance
 */
#include "bmk_stubs_types.h"

bmk_stubs_t			*__bmk_stubs;


void bmk_stubs_init(bmk_stubs_t *stubs) {
	__bmk_stubs = stubs;
}

