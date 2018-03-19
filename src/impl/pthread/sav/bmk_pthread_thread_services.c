/*
 * bmk_pthread_thread_services.c
 *
 *  Created on: Mar 3, 2018
 *      Author: ballance
 */
#include "bmk_thread_services.h"
#include "bmk_pthread_thread_services.h"
#include "bmk_pthread.h"
#include <string.h>
#include <stdio.h>

static void bmk_pthread_thread_tramp(void *ud) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)ud;

	fprintf(stdout, "thread_tramp --> main\n");
	fflush(stdout);
	pt->main_f(pt->main_ud);
	fprintf(stdout, "thread_tramp <-- main\n");
	fflush(stdout);

	bmk_pthread_scheduler_thread_exit(pt);
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
	pt->procmask = 0; // Pin to core0 initially
	pt->dead = 0;

	getcontext(&pt->ctxt);
	pt->ctxt.uc_stack.ss_sp = stk;
	pt->ctxt.uc_stack.ss_size = stk_sz;
	pt->ctxt.uc_stack.ss_flags = 0;
	pt->ctxt.uc_link = 0;
	makecontext(&pt->ctxt, (void (*)())&bmk_pthread_thread_tramp, 1, pt);

	// TODO: add new thread to the scheduler's ready list
	bmk_pthread_scheduler_add2ready(pt);
}

void bmk_thread_init_cpuset(
		bmk_thread_pthread_t	*t,
		void					*stk,
		uint32_t				stk_sz,
		bmk_thread_main_f		main_f,
		void					*ud,
		bmk_cpuset_t			cpuset) {
	bmk_thread_pthread_t *pt = (bmk_thread_pthread_t *)t;

	pt->main_f = main_f;
	pt->main_ud = ud;
	pt->procmask = cpuset;
	pt->dead = 0;

	getcontext(&pt->ctxt);
	pt->ctxt.uc_stack.ss_sp = stk;
	pt->ctxt.uc_stack.ss_size = stk_sz;
	pt->ctxt.uc_stack.ss_flags = 0;
	pt->ctxt.uc_link = 0;
	makecontext(&pt->ctxt, (void (*)())&bmk_pthread_thread_tramp, 1, pt);

	// TODO: add new thread to the scheduler's ready list
	bmk_pthread_scheduler_add2ready(pt);
}

void bmk_thread_yield(void) {
	core_data_t *core_data = bmk_pthread_get_core_data();
	fprintf(stdout, "--> bmk_thread_yield: %d\n", core_data->procid);

	bmk_pthread_scheduler_reschedule(core_data, 0);

	fprintf(stdout, "<-- bmk_thread_yield: %d\n", core_data->procid);
}

void bmk_thread_join(bmk_thread_pthread_t *t) {
	core_data_t *core_data = bmk_pthread_get_core_data();
	fprintf(stdout, "--> bmk_thread_join %d\n", t->dead);

	while (!t->dead) {
		bmk_pthread_scheduler_reschedule(core_data, 1);
	}

	fprintf(stdout, "<-- bmk_thread_join %d\n", t->dead);
}

bmk_thread_pthread_t *bmk_thread_self(void) {
	core_data_t *core_data = bmk_pthread_get_core_data();
	return core_data->active_thread;
}

void bmk_mutex_init(bmk_mutex_pthread_t *m) {
	memset(m, 0, sizeof(bmk_mutex_pthread_t));
}

void bmk_mutex_lock(bmk_mutex_pthread_t *m) {
	// Lock access to mutex fields
	while (!__sync_bool_compare_and_swap(&m->lock, 0, 1)) { ; }
	if (!m->owner) {
		// Now we own it
		m->owner = bmk_thread_self();
	} else {
		bmk_thread_pthread_t *this_t = bmk_thread_self();
		this_t->next = m->waiters;
		m->waiters = this_t;

		// Unlock until we come back
		while (!__sync_bool_compare_and_swap(&m->lock, 1, 0)) { ; }

		// Suspend the thread
		while (1) {
			bmk_thread_yield(); // TODO
			while (!__sync_bool_compare_and_swap(&m->lock, 0, 1)) { ; }
			if (!m->owner) {
				// Now we own the mutex
				m->owner = this_t;
				break;
			} else {
				// Unlock until we come around again
				while (!__sync_bool_compare_and_swap(&m->lock, 1, 0)) { ; }
			}
		}
	}

	// Unlock
	while (!__sync_bool_compare_and_swap(&m->lock, 1, 0)) { ; }
}

void bmk_mutex_unlock(bmk_mutex_pthread_t *m) {
	while (!__sync_bool_compare_and_swap(&m->lock, 0, 1)) { ; }
	m->owner = 0;
	while (!__sync_bool_compare_and_swap(&m->lock, 1, 0)) { ; }
}

void bmk_cond_init(bmk_cond_pthread_t *c) {
	memset(c, 0, sizeof(bmk_cond_pthread_t));
}

void bmk_cond_wait(bmk_cond_pthread_t *c, bmk_mutex_pthread_t *m) {
	bmk_thread_pthread_t *this_t = bmk_thread_self();
	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
	this_t->next = c->waiters;
	c->waiters = this_t;
	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
	// Wait for the thread to be rescheduled
	bmk_pthread_scheduler_block(this_t);
	bmk_mutex_unlock(m);
}

void bmk_cond_signal(bmk_cond_pthread_t *c) {
	bmk_thread_pthread_t *unblock_thread = 0;

	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
	if (c->waiters) {
		unblock_thread = c->waiters;
		c->waiters = unblock_thread->next;

		bmk_pthread_scheduler_unblock(unblock_thread);
	}
	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
}

void bmk_cond_signal_all(bmk_cond_pthread_t *c) {
	bmk_thread_pthread_t *unblock_thread = 0;

	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
	while (c->waiters) {
		unblock_thread = c->waiters;
		c->waiters = unblock_thread->next;

		bmk_pthread_scheduler_unblock(unblock_thread);
	}
	while (!__sync_bool_compare_and_swap(&c->lock, 0, 1)) { ; }
}

