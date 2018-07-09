/*
 * bmk_impl_scheduler_simple.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */
#include <string.h>
#include "bmk_thread_types.h"
#include "bmk_int_atomics.h"
#include "bmk_int_sys.h"
#include "bmk_int_debug.h"

typedef struct bmk_scheduler_simple_data_s {
	bmk_atomic_t			lock;
	uint32_t				changed;
	bmk_thread_t			*active_thread_l;

} bmk_scheduler_simple_data_t;

static bmk_scheduler_simple_data_t		prv_data = {0};

static bmk_thread_t *bmk_simple_scheduler_get_next_thread(bmk_core_data_t *core_data) {
	bmk_thread_t *t, *next_t = 0, *last_t = 0;

	bmk_debug("--> bmk_simple_scheduler_get_next_thread\n");

	// Now, find the next thread
	t = prv_data.active_thread_l;
	while (t) {
		bmk_debug("  t=%p procmask=0x%08x procid=%d\n", t, t->procmask, core_data->procid);
		if (t->procmask & (1 << core_data->procid)) {
			next_t = t;

			if (last_t) {
				last_t->next = t->next;
			} else {
				// This is the first iteration
				prv_data.active_thread_l = t->next;
			}
			break;
		}
		last_t = t;
		t = t->next;
	}

	bmk_debug("next_t = %p\n", next_t);
	bmk_debug("<-- bmk_simple_scheduler_get_next_thread %p\n", next_t);

	return next_t;
}

static void bmk_simple_scheduler_queue_active_thread(bmk_core_data_t *core_data) {
	bmk_thread_t *active_t = core_data->active_thread;

	if (prv_data.active_thread_l) {
		bmk_thread_t *t = prv_data.active_thread_l;

		while (t->next) {
			bmk_debug("t=%p next=%p\n", t, t->next);
			t = t->next;
		}

		bmk_debug("head t=%p\n", t);

		// t wasn't null, but the next is
		t->next = active_t;
	} else {
		prv_data.active_thread_l = active_t;
	}
	active_t->next = 0;

}

void bmk_scheduler_init(void) {
	bmk_debug("--> bmk_scheduler_init()\n");
	bmk_atomics_init(&prv_data.lock);
	bmk_debug("<-- bmk_scheduler_init()\n");
}

void bmk_scheduler_thread_new(bmk_thread_t *t) {
	bmk_debug("--> bmk_scheduler_thread_new(%p)\n", t);
	bmk_atomics_lock(&prv_data.lock);
	t->next = prv_data.active_thread_l;
	prv_data.active_thread_l = t;
	bmk_atomics_unlock(&prv_data.lock);
	bmk_debug("<-- bmk_scheduler_thread_new()\n");
}

void bmk_scheduler_thread_exit(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;
	bmk_debug("--> bmk_scheduler_thread_exit()\n");
	bmk_atomics_lock(&prv_data.lock);

	next_t = bmk_simple_scheduler_get_next_thread(core_data);
	core_data->active_thread = next_t;

	bmk_atomics_unlock(&prv_data.lock);

	bmk_debug("EXIT SWAP: %p => %p\n", active_t, core_data->active_thread);
	bmk_context_swapcontext(&active_t->ctxt, &core_data->active_thread->ctxt);

	bmk_debug("<-- bmk_scheduler_thread_exit()\n");
}

void bmk_scheduler_thread_block(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;

	// The active thread is now blocked
	// Select another thread and swap to that one
	bmk_debug("--> bmk_scheduler_thread_block proc=%d thread=%p\n",
			core_data->procid, t);
	bmk_atomics_lock(&prv_data.lock);

	next_t = bmk_simple_scheduler_get_next_thread(core_data);
	core_data->active_thread = next_t;

	bmk_atomics_unlock(&prv_data.lock);

	bmk_debug("EXIT SWAP: %p => %p\n", active_t, core_data->active_thread);
	bmk_context_swapcontext(&active_t->ctxt, &core_data->active_thread->ctxt);

	bmk_debug("<-- bmk_scheduler_thread_block proc=%d thread=%p\n",
			core_data->procid, t);
}

void bmk_scheduler_thread_unblock(bmk_thread_t *t) {
	bmk_debug("--> bmk_scheduler_thread_unblock\n");
	// Add back to the active list
	bmk_atomics_lock(&prv_data.lock);
	t->next = prv_data.active_thread_l;
	prv_data.active_thread_l = t;
	bmk_atomics_unlock(&prv_data.lock);
	bmk_sys_send_proc_event(0, 0);
	bmk_debug("<-- bmk_scheduler_thread_unblock\n");
}

// Reschedule operates for a given core
void bmk_scheduler_reschedule(uint32_t wait) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t = core_data->active_thread;

	if (!active_t) {
		bmk_debug("Error: core %d doesn't have an active thread\n",
				core_data->procid);
		return;
	}


	bmk_debug("--> bmk_scheduler_reschedule active=%p\n", active_t);
	bmk_atomics_lock(&prv_data.lock);

	// Add the active thread to the back of the list
//	if (t) {
//		while (t->next) {
//			bmk_debug("t=%p next=%p\n", t, t->next);
//			t = t->next;
//		}
//
//		bmk_debug("head t=%p\n", t);
//	}

	bmk_simple_scheduler_queue_active_thread(core_data);
//	core_data->active_thread = 0;

	{
		bmk_thread_t *t = prv_data.active_thread_l;
		while (t) {
			bmk_debug("  t=%p mask=0x%08x\n", t, t->procmask);
			t=t->next;
		}
	}

	core_data->active_thread = bmk_simple_scheduler_get_next_thread(core_data);

	bmk_debug("  next_thread=%p (active_t=%p)\n", core_data->active_thread, active_t);

	if (!core_data->active_thread) {
		bmk_debug("Error: get_next_thread returned 0\n");
	}


	if (active_t != core_data->active_thread) {
		bmk_atomics_unlock(&prv_data.lock);

		// Notify all processors that the schedule list has changed
		bmk_sys_send_proc_event(0, 0);

		bmk_debug("SWAP: %p => %p\n", active_t, core_data->active_thread);
		bmk_context_swapcontext(&active_t->ctxt, &core_data->active_thread->ctxt);
	} else {
		bmk_atomics_unlock(&prv_data.lock);
		if (wait) {
			bmk_debug("--> reschedule: bmk_sys_wait_proc_event\n");
			bmk_sys_wait_proc_event();
			bmk_debug("<-- reschedule: bmk_sys_wait_proc_event\n");
		}
	}

	bmk_debug("<-- bmk_scheduler_reschedule\n");
}


void bmk_scheduler_nonprimary(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *this_t = core_data->active_thread;
	bmk_debug("--> bmk_scheduler_nonprimary()\n");

	// This should act as 'slave'
	while (1) {
//		uint32_t main_thread_active = 1;

		bmk_simple_scheduler_queue_active_thread(core_data);

		core_data->active_thread = bmk_simple_scheduler_get_next_thread(core_data);

		if (this_t == core_data->active_thread) {
			bmk_debug("--> bmk_scheduler_nonprimary::wait_proc_event\n");
			bmk_sys_wait_proc_event();
			bmk_debug("<-- bmk_scheduler_nonprimary::wait_proc_event\n");
		} else {
			bmk_debug("--> bmk_scheduler_nonprimary::SWAP\n");
			bmk_context_swapcontext(
					&this_t->ctxt,
					&core_data->active_thread->ctxt);
			bmk_debug("<-- bmk_scheduler_nonprimary::SWAP\n");
		}

		if (!bmk_sys_main_core_active()) {
			break;
		}

//		bmk_sys_wait_proc_event();
//
//		if (!bmk_sys_main_core_active()) {
//			break;
//		}
	}

	bmk_debug("<-- bmk_scheduler_nonprimary()\n");
}


