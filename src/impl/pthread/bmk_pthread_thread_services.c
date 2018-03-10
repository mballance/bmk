/*
 * bmk_pthread_thread_services.c
 *
 *  Created on: Mar 3, 2018
 *      Author: ballance
 */
#include "../pthread/bmk_thread_services_target.h"
#include "bmk_thread_services.h"

static void bmk_pthread_thread_tramp(void *ud) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)ud;

	pt->main_f(pt->main_ud);

	// TODO: On exit, clean up
}

void bmk_thread_init(
		bmk_thread_pthread_t	*t,
		void					*stk,
		uint32_t				stk_sz,
		bmk_thread_main_f		main_f,
		void					*ud) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)t;

	pt->main_f = main_f;
	pt->main_ud = ud;

	getcontext(&pt->ctxt);
	pt->ctxt.uc_stack.ss_sp = stk;
	pt->ctxt.uc_stack.ss_size = stk_sz;
	pt->ctxt.uc_stack.ss_flags = 0;
	pt->ctxt.uc_link = 0;
	makecontext(&pt->ctxt, (void (*)())&bmk_pthread_thread_tramp, 1, pt);
}

bmk_thread_pthread_t *bmk_thread_self(void) {
	return 0;

}

void bmk_mutex_init(bmk_mutex_pthread_t *m) {

}

void bmk_mutex_lock(bmk_mutex_pthread_t *m) {

}

void bmk_mutex_unlock(bmk_mutex_pthread_t *m) {

}

void bmk_cond_init(bmk_cond_pthread_t *c) {

}

void bmk_cond_wait(bmk_cond_pthread_t *c, bmk_mutex_pthread_t *m) {

}

void bmk_cond_signal(bmk_cond_pthread_t *c) {

}
