/*
 * bmk_pthread_thread_services.c
 *
 *  Created on: Mar 3, 2018
 *      Author: ballance
 */
#include "bmk_thread_services.h"
#include "pthreads/bmk_thread_services_target.h"

static void bmk_pthread_thread_tramp(void *ud) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)ud;

	pt->main_f(pt->main_ud);

	// TODO: On exit, clean up
}

void bmk_thread_init(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)t;

	pt->main_f = main_f;
	pt->main_ud = ud;

	getcontext(&pt->ctxt);
	pt->ctxt.uc_stack.ss_sp = stk;
	pt->ctxt.uc_stack.ss_size = stk_sz;
	pt->ctxt.uc_stack.ss_flags = 0;
	pt->ctxt.uc_link = 0;
	makecontext(&pt->ctxt, &bmk_pthread_thread_tramp, 1, pt);
}

