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

#define STKSIZE 16384

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
	uint8_t					*stk = (uint8_t *)malloc(STKSIZE);
	bool					called = false;

	fprintf(stdout, "called=%d\n", called);
	fflush(stdout);

	bmk_thread_init(
			&t,
			stk,
			STKSIZE,
			&singlecore_1thread_func,
			&called);

	bmk_thread_yield();

	ASSERT_EQ(called, true);
	fprintf(stdout, "called=%d\n", called);
}

TEST(smoke,singlecore_1thread) {
	set_bmk_main(&singlecore_1thread_main);

	fprintf(stdout, "--> bmk_pthread_main()\n");
	bmk_pthread_main(1);
	fprintf(stdout, "<-- bmk_pthread_main()\n");
}

//*******************************************************************


static int singlecore_2thread_func1(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- singlecore_2thread_func1()\n");
	return 0;
}

static int singlecore_2thread_func2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- singlecore_2thread_func2()\n");
	return 0;
}

static void singlecore_2thread_main(void) {
	bmk_thread_pthread_t	t1, t2;
	uint8_t					*stk1 = (uint8_t *)malloc(STKSIZE);
	uint8_t					*stk2 = (uint8_t *)malloc(STKSIZE);
	bool					called1 = false;
	bool					called2 = false;

	bmk_thread_init(
			&t1,
			stk1,
			STKSIZE,
			&singlecore_2thread_func1,
			&called1);
	bmk_thread_init(
			&t2,
			stk2,
			STKSIZE,
			&singlecore_2thread_func2,
			&called2);

	bmk_thread_yield();
	bmk_thread_yield();
	bmk_thread_yield();
	bmk_thread_yield();

	ASSERT_EQ(called1, true);
	ASSERT_EQ(called2, true);
}

TEST(smoke,singlecore_2thread) {
	fprintf(stdout, "singlecore_2thread\n");
	set_bmk_main(&singlecore_2thread_main);

	bmk_pthread_main(1);
}

