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
#include "bmk_impl_sys_pthread.h"
#include <valgrind/valgrind.h>

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
	bmk_thread_t	t;
	uint8_t			*stk = (uint8_t *)malloc(STKSIZE);
	bool			called = false;

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
	bmk_thread_t	t1, t2;
	uint8_t			*stk1 = (uint8_t *)malloc(STKSIZE);
	uint8_t			*stk2 = (uint8_t *)malloc(STKSIZE);
	bool			called1 = false;
	bool				called2 = false;

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

	bmk_thread_join(&t1);
	bmk_thread_join(&t2);

	ASSERT_EQ(called1, true);
	ASSERT_EQ(called2, true);
}

TEST(smoke,singlecore_2thread) {
	fprintf(stdout, "singlecore_2thread\n");
	set_bmk_main(&singlecore_2thread_main);

	bmk_pthread_main(1);
}

//*******************************************************************


static int dualcore_2thread_func1(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- dualcore_2thread_func1()\n");
	return 0;
}

static int dualcore_2thread_func2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- dualcore_2thread_func2()\n");
	return 0;
}

static void dualcore_2thread_main(void) {
	bmk_thread_t	t1, t2;
	uint8_t			*stk1 = (uint8_t *)malloc(STKSIZE);
	uint8_t			*stk2 = (uint8_t *)malloc(STKSIZE);
	bool			called1 = false;
	bool			called2 = false;

	bmk_thread_init(
			&t1,
			stk1,
			STKSIZE,
			&dualcore_2thread_func1,
			&called1);
	bmk_thread_init(
			&t2,
			stk2,
			STKSIZE,
			&dualcore_2thread_func2,
			&called2);

	bmk_thread_yield();
	bmk_thread_yield();
	bmk_thread_yield();
	bmk_thread_yield();

	ASSERT_EQ(called1, true);
	ASSERT_EQ(called2, true);
}

TEST(smoke,dualcore_2thread) {
	fprintf(stdout, "dualcore_2thread\n");
	set_bmk_main(&dualcore_2thread_main);

	fprintf(stdout, "--> bmk_pthread_main(2)\n");
	bmk_pthread_main(2);
	fprintf(stdout, "<-- bmk_pthread_main(2)\n");
}

//*******************************************************************


static int dualcore_2thread_func1_2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- dualcore_2thread_func1()\n");
	return 0;
}

static int dualcore_2thread_func2_2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- dualcore_2thread_func2()\n");
	return 0;
}

static void dualcore_2thread_main_2(void) {
	bmk_thread_t	t1, t2;
	uint8_t			*stk1 = (uint8_t *)malloc(STKSIZE);
	uint8_t			*stk2 = (uint8_t *)malloc(STKSIZE);
	bool			called1 = false;
	bool			called2 = false;

	bmk_thread_init_cpuset(
			&t1,
			stk1,
			STKSIZE,
			&dualcore_2thread_func1_2,
			&called1,
			(1 << 0));
	bmk_thread_init_cpuset(
			&t2,
			stk2,
			STKSIZE,
			&dualcore_2thread_func2_2,
			&called2,
			(1 << 1));

	// Wait for the threads to end
	bmk_thread_join(&t1);
	bmk_thread_join(&t2);

	ASSERT_EQ(called1, true);
	ASSERT_EQ(called2, true);
}

TEST(smoke,dualcore_2thread_2) {
	fprintf(stdout, "dualcore_2thread\n");
	set_bmk_main(&dualcore_2thread_main_2);

	fprintf(stdout, "--> bmk_pthread_main(2)\n");
	bmk_pthread_main(2);
	fprintf(stdout, "<-- bmk_pthread_main(2)\n");
}

//*******************************************************************
static bmk_mutex_t			pingpong_12_req_mutex;
static bool					pingpong_12_req;
static bmk_cond_t			pingpong_12_req_cond;
static bmk_mutex_t			pingpong_21_ack_mutex;
static bool					pingpong_21_ack;
static bmk_cond_t			pingpong_21_ack_cond;
static int					pingpong_count;

static int dualcore_2thread_pingpong_func1_2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "--> dualcore_2thread_pingpong_func1()\n");
	for (int i=0; i<16; i++) {
		fprintf(stdout, "--> thread1 send req\n");
		bmk_mutex_lock(&pingpong_12_req_mutex);
		pingpong_12_req = true;
		bmk_cond_signal(&pingpong_12_req_cond);
		bmk_mutex_unlock(&pingpong_12_req_mutex);
		fprintf(stdout, "<-- thread1 send req\n");

		// Now, wait for a response
		fprintf(stdout, "--> thread1 wait ack\n");
		while (true) {
			bmk_mutex_lock(&pingpong_21_ack_mutex);
			if (pingpong_21_ack) {
				pingpong_21_ack = false;
				pingpong_count++;
				bmk_mutex_unlock(&pingpong_21_ack_mutex);
				break;
			} else {
				bmk_cond_wait(&pingpong_21_ack_cond, &pingpong_21_ack_mutex);
			}
			bmk_mutex_unlock(&pingpong_21_ack_mutex);
		}
		fprintf(stdout, "<-- thread1 wait ack\n");
	}
	fprintf(stdout, "<-- dualcore_2thread_pingpong_func1()\n");
	return 0;
}

static int dualcore_2thread_pingpong_func2_2(void *ud) {
	*((bool *)ud) = true;
	fprintf(stdout, "-- dualcore_2thread_pingpong_func2()\n");
	for (int i=0; i<16; i++) {
		fprintf(stdout, "--> thread2 wait req\n");
		while (true) {
			bmk_mutex_lock(&pingpong_12_req_mutex);
			if (pingpong_12_req) {
				pingpong_12_req = false;
				bmk_mutex_unlock(&pingpong_12_req_mutex);
				break;
			} else {
				bmk_cond_wait(&pingpong_12_req_cond, &pingpong_12_req_mutex);
			}
			bmk_mutex_unlock(&pingpong_12_req_mutex);
		}
		fprintf(stdout, "<-- thread2 wait req\n");

		fprintf(stdout, "--> thread2 send ack\n");
		bmk_mutex_lock(&pingpong_21_ack_mutex);
		pingpong_21_ack = true;
		bmk_cond_signal(&pingpong_21_ack_cond);
		bmk_mutex_unlock(&pingpong_21_ack_mutex);
		fprintf(stdout, "<-- thread2 send ack\n");
	}
	return 0;
}

static void dualcore_2thread_pingpong_main_2(void) {
	bmk_thread_t	t1, t2;
	uint8_t			*stk1 = (uint8_t *)malloc(STKSIZE);
	uint8_t			*stk2 = (uint8_t *)malloc(STKSIZE);
	bool			called1 = false;
	bool			called2 = false;

	VALGRIND_STACK_REGISTER(stk1, stk1+STKSIZE);
	VALGRIND_STACK_REGISTER(stk2, stk2+STKSIZE);

	bmk_thread_init_cpuset(
			&t1,
			stk1,
			STKSIZE,
			&dualcore_2thread_pingpong_func1_2,
			&called1,
			(1 << 0));
	bmk_thread_init_cpuset(
			&t2,
			stk2,
			STKSIZE,
			&dualcore_2thread_pingpong_func2_2,
			&called2,
			(1 << 1));

	// Wait for the threads to end
	bmk_thread_join(&t1);
	bmk_thread_join(&t2);

	ASSERT_EQ(called1, true);
	ASSERT_EQ(called2, true);
	ASSERT_EQ(pingpong_count, 16);
}

TEST(smoke,dualcore_2thread_pingpong) {
	fprintf(stdout, "dualcore_2thread_pingpong\n");
	set_bmk_main(&dualcore_2thread_pingpong_main_2);

	fprintf(stdout, "--> bmk_pthread_main(2)\n");
	bmk_pthread_main(2);
	fprintf(stdout, "<-- bmk_pthread_main(2)\n");
}


