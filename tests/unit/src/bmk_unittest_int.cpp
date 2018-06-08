/*
 * bmk_unittest_int.cpp
 *
 *  Created on: Mar 10, 2018
 *      Author: ballance
 */
#include "bmk.h"
#include "bmk_pthread.h"

static void (*prv_bmk_main)(void) = 0;

void set_bmk_main(void (*f)(void)) {
	prv_bmk_main = f;
}

void bmk_main(void) {
	if (prv_bmk_main) {
		prv_bmk_main();
	} else {

	}
}

