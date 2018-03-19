/*
 * bmk_thread_services.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */
#include "bmk_thread_services.h"
#include "bmk_int_context.h"
#include "bmk_int_scheduler.h"

static void bmk_thread_tramp(void *ud) {
	bmk_thread_t *t = (bmk_thread_t *)ud;

	t->main_f(t->main_ud);

	// Doesn't return
	bmk_scheduler_thread_exit(t);
}

void bmk_thread_init(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud) {

	t->main_f = main_f;
	t->main_ud = ud;
	t->procmask = 0; // Pin to core0 initially
//	t->dead = 0; // TODO:

	bmk_context_makecontext(&t->ctxt, stk, stk_sz,
			&bmk_thread_tramp, t);

	bmk_scheduler_thread_new(t);
}

void bmk_thread_init_cpuset(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud,
		bmk_cpuset_t		cpuset) {

	t->main_f = main_f;
	t->main_ud = ud;
	t->procmask = cpuset;
//	t->dead = 0; // TODO:

	bmk_context_makecontext(&t->ctxt, stk, stk_sz,
			&bmk_thread_tramp, t);

	bmk_scheduler_thread_new(t);
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

	// The active thread could be running on a different
	// core than the target one
	while (!t->dead) {
		bmk_pthread_scheduler_reschedule(core_data, 1);
	}

	fprintf(stdout, "<-- bmk_thread_join %d\n", t->dead);
}

bmk_thread_t *bmk_thread_self(void) {
	core_data_t *core_data = bmk_pthread_get_core_data();
	return core_data->active_thread;
}

void bmk_mutex_init(bmk_mutex_t *m) {
	memset(m, 0, sizeof(bmk_mutex_t));
	bmk_atomics_init(&m->lock);
}

void bmk_mutex_lock(bmk_mutex_t *m) {
	// Lock access to mutex fields
	bmk_atomics_lock(&m->lock);
	if (!m->owner) {
		// Now we own it
		m->owner = bmk_thread_self();
	} else {
		bmk_thread_pthread_t *this_t = bmk_thread_self();
		this_t->next = m->waiters;
		m->waiters = this_t;

		// Unlock until we come back
		bmk_atomics_unlock(&m->lock);

		// Suspend the thread
		while (1) {
			bmk_thread_yield(); // TODO
			bmk_atomics_lock(&m->lock);
			if (!m->owner) {
				// Now we own the mutex
				m->owner = this_t;
				break;
			} else {
				// Unlock until we come around again
				bmk_atomics_unlock(&m->lock);
			}
		}
	}

	// Unlock
	bmk_atomics_unlock(&m->lock);
}

void bmk_mutex_unlock(bmk_mutex_pthread_t *m) {
	bmk_atomics_lock(&m->lock);
	m->owner = 0;
	bmk_atomics_unlock(&m->lock);
}

void bmk_cond_init(bmk_cond_pthread_t *c) {
	memset(c, 0, sizeof(bmk_cond_pthread_t));
	bmk_atomics_init(&c->lock);
}

void bmk_cond_wait(bmk_cond_pthread_t *c, bmk_mutex_pthread_t *m) {
	bmk_thread_pthread_t *this_t = bmk_thread_self();
	bmk_atomics_lock(&c->lock);
	this_t->next = c->waiters;
	c->waiters = this_t;
	bmk_atomics_unlock(&c->lock);
	// Wait for the thread to be rescheduled
	bmk_pthread_scheduler_block(this_t);
	bmk_mutex_unlock(m);
}

void bmk_cond_signal(bmk_cond_pthread_t *c) {
	bmk_thread_pthread_t *unblock_thread = 0;

	bmk_atomics_lock(&c->lock);
	if (c->waiters) {
		unblock_thread = c->waiters;
		c->waiters = unblock_thread->next;

		bmk_pthread_scheduler_unblock(unblock_thread);
	}
	bmk_atomics_unlock(&c->lock);
}

void bmk_cond_signal_all(bmk_cond_pthread_t *c) {
	bmk_thread_pthread_t *unblock_thread = 0;

	bmk_atomics_lock(&c->lock);
	while (c->waiters) {
		unblock_thread = c->waiters;
		c->waiters = unblock_thread->next;

		bmk_pthread_scheduler_unblock(unblock_thread);
	}
	bmk_atomics_unlock(&c->lock);
}
