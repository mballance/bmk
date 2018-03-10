/*
 * bmk_unittest_smoke.cpp
 *
 *  Created on: Mar 10, 2018
 *      Author: ballance
 */
#include <stdio.h>
#include "gtest/gtest.h"
#include "bmk_unittest_int.h"
#include "bmk.h"
#include "bmk_pthread_thread_services.h"
#include "bmk_pthread.h"

static bool smoke_bmk_main_called = false;

static void smoke_bmk_main(void) {
	smoke_bmk_main_called = true;
}

TEST(smoke,singlecore) {
	set_bmk_main(&smoke_bmk_main);

	bmk_pthread_main(1);

	ASSERT_EQ(smoke_bmk_main_called, true);
}

//*******************************************************************

static int singlecore_1thread_func(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- singlecore_1thread_func()\n");
	return 0;
}

static void singlecore_1thread_main(void) {
	bmk_thread_pthread_t	t;
	uint8_t					*stk = (uint8_t *)malloc(1024);
	bool					called = false;

	fprintf(stdout, "called=%d\n", called);
	fflush(stdout);

	bmk_thread_init(
			&t,
			stk,
			1024,
			&singlecore_1thread_func,
			&called);

	bmk_thread_yield();

	ASSERT_EQ(called, true);
	fprintf(stdout, "called=%d\n", called);
}

TEST(smoke,singlecore_1thread) {
	set_bmk_main(&singlecore_1thread_main);

	bmk_pthread_main(1);
}
