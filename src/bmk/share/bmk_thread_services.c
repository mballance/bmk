/****************************************************************************
 *
 * Copyright 2018-2021 Matthew Ballance
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * bmk_thread_services.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 *
 ****************************************************************************/
#include "bmk_thread_services.h"
#include "bmk_int_atomics.h"
#include "bmk_int_context.h"
#include "bmk_int_scheduler.h"
#include "bmk_int_sys.h"
#include "bmk_int_debug.h"

#undef BMK_THREAD_DEBUG
#ifdef BMK_THREAD_DEBUG
#define bmk_thread_debug(...) bmk_debug(__VA_ARGS__)
#else
#define bmk_thread_debug(...)
#endif


static int32_t bmk_thread_tramp(void *ud) {
	bmk_thread_t *t = (bmk_thread_t *)ud;
	int32_t ret;

	ret = t->main_f(t->main_ud);
	t->alive = 0;

	// Doesn't return
	bmk_scheduler_thread_exit(t);

	return ret;
}

void bmk_thread_init(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud) {

	t->main_f = main_f;
	t->main_ud = ud;
#ifdef UNDEFINED
	bmk_cpuset_zero(&t->procmask);
	bmk_cpuset_set(0, &t->procmask); // Pin to core0 initially
#endif
	t->alive = 1;

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
		bmk_cpuset_t		*cpuset) {
	uint32_t i;

	t->main_f = main_f;
	t->main_ud = ud;
#ifdef UNDEFINED
	for (i=0; i<sizeof(bmk_cpuset_t)/sizeof(uint32_t); i++) {
		t->procmask.mask[i] = cpuset->mask[i];
	}
#endif
	t->alive = 1;

	bmk_context_makecontext(&t->ctxt, stk, stk_sz,
			&bmk_thread_tramp, t);

	bmk_scheduler_thread_new(t);
}

void bmk_thread_yield(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_debug("--> bmk_thread_yield: %d", core_data->coreid);

	bmk_scheduler_reschedule(0);

	bmk_thread_debug("<-- bmk_thread_yield: %d", core_data->coreid);
}

void bmk_thread_join(bmk_thread_t *t) {

	bmk_thread_debug("--> bmk_thread_join: %p %d", t, bmk_get_procid());
	bmk_scheduler_thread_join(t);
	bmk_thread_debug("<-- bmk_thread_join: %p %d", t, bmk_get_procid());
}

void bmk_thread_setaffinity(
		bmk_thread_t		*t,
		bmk_cpuset_t		*cpuset) {
	bmk_scheduler_thread_setaffinity(t, cpuset);
}

bmk_thread_t *bmk_thread_self(void) {
	return bmk_scheduler_active_thread();
}

void bmk_mutex_init(bmk_mutex_t *m) {
	m->owner = 0;
	m->waiters = 0;
	bmk_atomics_init(&m->lock);
}

void bmk_mutex_lock(bmk_mutex_t *m) {
	// Lock access to mutex fields
	bmk_thread_debug("--> bmk_mutex_lock %p %d", m, bmk_get_procid());
	bmk_thread_t *this_t = bmk_thread_self();
	bmk_atomics_lock(&m->lock);
	if (!m->owner) {
		// Now we own it
		m->owner = this_t;
		bmk_thread_debug(" -- bmk_mutex_lock [OWN] m=%p this_t=%p %d", m, m->owner, bmk_get_procid());
	} else {
		bmk_thread_debug("Add thread: this_t=%p next=%p waiters=%p", this_t, this_t->next, m->waiters);


		// Suspend the thread
		while (1) {
			// Add to the end of the list
			// We'll unblock all waiting threads anyway

			// If we aren't currently on a list, add ourselves.
			// We may have been unblocked for a variety of reasons
			if (!this_t->next) {
				this_t->next = m->waiters;
				m->waiters = this_t;
			}

			bmk_thread_debug(" -- bmk_mutex_lock [WAIT] m=%p %d", m, bmk_get_procid());

			// Block the thread until it is unblocked
			bmk_scheduler_thread_block(&m->lock);

			bmk_thread_debug(" -- bmk_mutex_lock post-block owner=%p (this_t=%p)", m->owner, this_t);

			// Wait for a new thread to be selected
//			bmk_scheduler_reschedule(0);

			if (!m->owner) {
				// Now we own the mutex
				m->owner = this_t;
				break;
			}
		}
	}

	// Unlock
	bmk_atomics_unlock(&m->lock);
	bmk_thread_debug("<-- bmk_mutex_lock %p %d", m, bmk_get_procid());
}

void bmk_mutex_unlock(bmk_mutex_t *m) {
	bmk_thread_t *t;
	bmk_thread_debug("--> bmk_mutex_unlock %p %d", m, bmk_get_procid());
	bmk_atomics_lock(&m->lock);
	bmk_thread_debug(" -- bmk_mutex_unlock m=%p %d", m, bmk_get_procid());

	t = m->waiters;
	m->waiters = 0;

	while (t) {
		bmk_thread_t *ub_t = t;
		bmk_thread_debug("About to unblock %p (next=%p)", ub_t, ub_t->next);
		bmk_scheduler_thread_unblock(ub_t);
		t = t->next;

		// Ensure we're removed from the list
		ub_t->next = 0;
	}

	m->owner = 0;
	bmk_atomics_unlock(&m->lock);
	bmk_thread_debug("<-- bmk_mutex_unlock %p %d", m, bmk_get_procid());
}

void bmk_cond_init(bmk_cond_t *c) {
	c->waiters = 0;
	bmk_atomics_init(&c->lock);
}

void bmk_cond_wait(bmk_cond_t *c, bmk_mutex_t *m) {
	bmk_thread_t *this_t = bmk_thread_self();
	bmk_thread_debug("--> bmk_cond_wait cond=%p waiters_p=%p %d", c, &c->waiters, bmk_get_procid());

	bmk_atomics_lock(&c->lock);
	// We're protected by the existing mutex lock
	this_t->next = c->waiters;
	c->waiters = this_t;
	bmk_thread_debug(" -- bmk_cond_wait: %p c->waiters=%p", c, c->waiters);
	bmk_atomics_unlock(&c->lock);

	bmk_mutex_unlock(m);

	// Block this thread
	bmk_thread_debug("--> bmk_cond_wait::block");
	bmk_scheduler_thread_block(0);
	bmk_thread_debug("<-- bmk_cond_wait::block");

	// Re-lock on exit
	bmk_mutex_lock(m);

	bmk_thread_debug("<-- bmk_cond_wait cond=%p %d", c, bmk_get_procid());
}

void bmk_cond_signal(bmk_cond_t *c) {

	bmk_thread_debug("--> bmk_cond_signal cond=%p %d", c, bmk_get_procid());

	bmk_atomics_lock(&c->lock);
	bmk_thread_debug(" -- bmk_cond_signal: %p c->waiters=%p", c, c->waiters);
	if (c->waiters) {
		bmk_thread_t *ub_t = c->waiters;
		c->waiters = c->waiters->next;

		// Ensure the thread is unlinked
		ub_t->next = 0;

		bmk_scheduler_thread_unblock(ub_t);
	}
	bmk_atomics_unlock(&c->lock);

	bmk_thread_debug("<-- bmk_cond_signal cond=%p %d", c, bmk_get_procid());
}

void bmk_cond_signal_all(bmk_cond_t *c) {
	bmk_thread_t *t;
	bmk_thread_debug("--> bmk_cond_signal_all %p waiters_p=%p waiters=%p", c, &c->waiters, c->waiters);

	bmk_atomics_lock(&c->lock);
	t = c->waiters;
	c->waiters = 0;
	while (t) {
		bmk_thread_t *ub_t = t;
		bmk_thread_debug("-- bmk_cond_signal_all: wakeup %p", ub_t);
		bmk_scheduler_thread_unblock(ub_t);
		t = t->next;

		// Ensure this is marked as not on a list
		ub_t->next = 0;
	}
	bmk_atomics_unlock(&c->lock);

	bmk_thread_debug("<-- bmk_cond_signal_all");
}

void bmk_cond_signal_async(bmk_cond_t *c) {

	// Not entirely sure this is safe
	// Do we need to try-wait on something?
	if (c->waiters) {
		bmk_scheduler_thread_unblock(c->waiters);
		c->waiters = 0;
	}
}

void bmk_event_init(bmk_event_t *e) {
	e->event = 0;
}

void bmk_event_wait(bmk_event_t *e) {
	bmk_scheduler_event_wait(e);
}

void bmk_event_signal(bmk_event_t *e) {
	bmk_scheduler_event_signal(e);
}

void __attribute__((weak)) bmk_thread_free(bmk_thread_t *t) {

}

