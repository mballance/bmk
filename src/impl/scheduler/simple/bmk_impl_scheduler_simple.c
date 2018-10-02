/*
 * bmk_impl_scheduler_simple.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */
#include <string.h>
#include "bmk_thread_types.h"
#include "bmk_thread_services.h"
#include "bmk_int_atomics.h"
#include "bmk_int_sys.h"
#include "bmk_int_debug.h"
#include "bmk_int_scheduler.h"

#undef DEBUG_SCHEDULER

// #ifdef DEBUG_SCHEDULER
// #define scheduler_debug(...) bmk_debug(__)

typedef struct bmk_scheduler_simple_data_s {
	bmk_atomic_t			lock;
	uint32_t				changed;
	bmk_thread_t			*thread_l;

} bmk_scheduler_simple_data_t;

static bmk_scheduler_simple_data_t		prv_data = {0};

static bmk_thread_t *bmk_simple_scheduler_get_next_thread(
		bmk_core_data_t 	*core_data,
		bmk_thread_t		*start) {
	bmk_thread_t *t, *next_t = 0, *prev_t;

	bmk_debug("--> bmk_simple_scheduler_get_next_thread start=%p");
#ifdef DEBUG_SCHEDULER
	{
		bmk_thread_t *thread_t = prv_data.thread_l;
		while (thread_t) {
		  bmk_debug("  thread_t=%p state=%d procmask=0x%08x",
				  thread_t, thread_t->sched_data.state, thread_t->procmask);
		  thread_t = thread_t->sched_data.next;
		}
	}
#endif

	// start should be the currently-active thread
	if (start && start->sched_data.next) {
		prev_t = start;
		start = start->sched_data.next;
	} else {
		prev_t = 0;
		start = prv_data.thread_l;
	}

	// Now, find the next thread
	t = start;
	while (t) {
		bmk_debug("  t=%p state=%d procmask=0x%08x procid=%d",
				t, t->sched_data.state, t->procmask, core_data->procid);
		if (t->sched_data.state == RunStateDead) {
			bmk_thread_t *dead_t = t;

			// Need to remove this thread from the run queue
//			bmk_info_low("-- remove dead thread %p", t);
			if (prev_t) {
				// unlink and relink
				prev_t->sched_data.next = t->sched_data.next;
			} else {
				// prev_t==0 means we're working with head
				prv_data.thread_l = t->sched_data.next;
			}

			// Prev doesn't change unless we're wrapping around.
			// We'll use the typical calculation for next
			if (t->sched_data.next) {
				prev_t = t;
				t = t->sched_data.next;
			} else {
				prev_t = 0;
				t = prv_data.thread_l;
			}

			// Notify that 't' is now free
			bmk_thread_free(dead_t);

			if (t == start) {
				bmk_debug("-- dead-thread wrap around");
				break;
			}
		} else {
			// Non-dead thread
			if (t->sched_data.state == RunStateIdle &&
				(t->procmask & (1 << core_data->procid))) {
				next_t = t;
				break;
			} else if (t->sched_data.state == RunStateBlocked &&
					t->sched_data.event &&
					bmk_atomics_compare_and_swap(&t->sched_data.event->event, 1, 0)) {
				//			bmk_debug("-- Notice wakeup");
//				bmk_info_low("-- Notice wakeup");
				//			t->sched_data.state = RunStateIdle;
				next_t = t;
				break;
			} else {

				if (t->sched_data.next) {
					prev_t = t;
					t = t->sched_data.next;
				} else {
					prev_t = 0;
					t = prv_data.thread_l;
				}

				// If we make it back around to where we started,
				// then exit
				if (t == start) {
//					bmk_debug("-- wrapped around");
					break;
				}
			}
		}
	}

	bmk_debug("next_t = %p", next_t);
	bmk_debug("<-- bmk_simple_scheduler_get_next_thread %p", next_t);

	return next_t;
}

void bmk_scheduler_init(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_debug("--> bmk_scheduler_init()");
	bmk_atomics_init(&prv_data.lock);

	bmk_atomics_lock(&prv_data.lock);
	bmk_context_getcontext(&core_data->main_thread.ctxt);
	prv_data.thread_l = 0;
	// Pin this thread to the active core
	core_data->main_thread.procmask = (1 << core_data->procid);
	core_data->main_thread.sched_data.state = RunStateRunning;
	core_data->main_thread.sched_data.next = prv_data.thread_l;
	prv_data.thread_l = &core_data->main_thread;
	core_data->active_thread = &core_data->main_thread;
	bmk_atomics_unlock(&prv_data.lock);

	bmk_debug("<-- bmk_scheduler_init()");
}

void bmk_scheduler_thread_new(bmk_thread_t *t) {
	bmk_debug("--> bmk_scheduler_thread_new(%p)", t);
	bmk_atomics_lock(&prv_data.lock);
	t->sched_data.next = prv_data.thread_l;
	prv_data.thread_l = t;
	t->sched_data.state = RunStateIdle;
	t->sched_data.event = 0; // no events
	t->sched_data.refcnt = 1; // We're holding onto this thread
	bmk_atomics_unlock(&prv_data.lock);
	bmk_debug("<-- bmk_scheduler_thread_new()");
}

void bmk_scheduler_thread_exit(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;
	bmk_debug("--> bmk_scheduler_thread_exit(%p)", active_t);
//	bmk_atomics_lock(&prv_data.lock);

	// Mark this thread dead
	active_t->sched_data.state = RunStateDead;

	// Don't return
	bmk_scheduler_reschedule(1);

	bmk_error("thread exit reschedule returned");

//	bmk_debug("<-- bmk_scheduler_thread_exit()");
}

void bmk_scheduler_thread_join(bmk_thread_t *t) {
	bmk_debug("--> bmk_scheduler_thread_join t=%p", t);

	// We're now also hanging onto this thread
	t->sched_data.refcnt++;

	while (t->sched_data.state != RunStateDead) {
		bmk_scheduler_reschedule(0);
		bmk_debug("  join: %p t.state=%d", t, t->sched_data.state);
	}

	// Free this thread
	if (--t->sched_data.refcnt == 0) {
		// TODO: free this thread
	}

	bmk_debug("<-- bmk_scheduler_thread_join t=%p", t);
}

void bmk_scheduler_thread_block(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;

	// The active thread is now blocked
	// Select another thread and swap to that one
	bmk_debug("--> bmk_scheduler_thread_block proc=%d thread=%p",
			core_data->procid, t);

	// Thread cannot run
	t->sched_data.state = RunStateBlocked;

	// Why don't we block?

//	bmk_atomics_lock(&prv_data.lock);

//	bmk_scheduler_reschedule(1);

//	next_t = bmk_simple_scheduler_get_next_thread(core_data, t);
//	core_data->active_thread = next_t;

//	bmk_atomics_unlock(&prv_data.lock);

//	bmk_debug("BLOCK SWAP: %p => %p", active_t, core_data->active_thread);
//	bmk_context_swapcontext(&active_t->ctxt, &core_data->active_thread->ctxt);

	bmk_debug("<-- bmk_scheduler_thread_block proc=%d thread=%p",
			core_data->procid, t);
}

void bmk_scheduler_thread_unblock(bmk_thread_t *t) {
	bmk_debug("--> bmk_scheduler_thread_unblock");
	// Add back to the active list
	t->sched_data.state = RunStateIdle;
//	bmk_atomics_lock(&prv_data.lock);
//	t->next = prv_data.active_thread_l;
//	prv_data.active_thread_l = t;
//	bmk_atomics_unlock(&prv_data.lock);
	bmk_sys_send_proc_event(0, 0);
	bmk_debug("<-- bmk_scheduler_thread_unblock");
}

// Reschedule operates for a given core
void bmk_scheduler_reschedule(uint32_t wait) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t = core_data->active_thread;
	bmk_thread_t *next_t = 0;

	if (!active_t) {
		bmk_fatal("core %d doesn't have an active thread",
				core_data->procid);
		return;
	}

	bmk_debug("--> bmk_scheduler_reschedule active=%p", active_t);

	// Pass in null, since this thread will eventually be removed
	if (active_t->sched_data.state == RunStateDead) {
		active_t = 0;
	}

	do {
		next_t = bmk_simple_scheduler_get_next_thread(core_data, active_t);

		if (wait && (!next_t || next_t == active_t)) {
			bmk_debug("--> reschedule: bmk_sys_wait_proc_event");
			bmk_sys_wait_proc_event();
			bmk_debug("<-- reschedule: bmk_sys_wait_proc_event");
		}
	} while ((!next_t || next_t == active_t) && wait);


	if (next_t) {
		bmk_thread_t *curr = core_data->active_thread;
		// Notify all processors that the schedule list has changed
		bmk_sys_send_proc_event(0, 0);

		bmk_debug("SWAP: %p => %p", core_data->active_thread, next_t);
		if (curr->sched_data.state == RunStateRunning) {
			// Move back to idle state if we were in the running state
			curr->sched_data.state = RunStateIdle;
		}
		next_t->sched_data.state = RunStateRunning;
		core_data->active_thread = next_t;
		bmk_context_swapcontext(&curr->ctxt, &next_t->ctxt);
	}

	bmk_debug("<-- bmk_scheduler_reschedule");
}

void bmk_scheduler_event_wait(bmk_event_t *e) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t = core_data->active_thread;

	bmk_debug("--> bmk_scheduler_event_wait");

	// First, check to see if the event has already been notified
	if (bmk_atomics_compare_and_swap(&e->event, 1, 0)) {
    	bmk_debug("-- already set");
		return;
	}

	// Now, suspend the thread
	active_t->sched_data.event = e;
	active_t->sched_data.state = RunStateBlocked;

	// TODO: wait, at least for a little while
	bmk_scheduler_reschedule(0);

	// This thread is no longer waiting
	active_t->sched_data.event = 0;
	bmk_debug("<-- bmk_scheduler_event_wait");
}

void bmk_scheduler_event_signal(bmk_event_t *e) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	uint8_t mask = (1 << core_data->procid);
	e->event = 1;

//	bmk_sys_send_proc_event(&mask, 1);
}

void bmk_scheduler_nonprimary(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *this_t = core_data->active_thread;
	bmk_debug("--> bmk_scheduler_nonprimary()");

	// This should act as 'slave'
//	while (1) {
////		uint32_t main_thread_active = 1;
//
//		bmk_simple_scheduler_queue_active_thread(core_data);
//
//		core_data->active_thread = bmk_simple_scheduler_get_next_thread(core_data);
//
//		if (this_t == core_data->active_thread) {
//			bmk_debug("--> bmk_scheduler_nonprimary::wait_proc_event");
//			bmk_sys_wait_proc_event();
//			bmk_debug("<-- bmk_scheduler_nonprimary::wait_proc_event\n");
//		} else {
//			bmk_debug("--> bmk_scheduler_nonprimary::SWAP\n");
//			bmk_context_swapcontext(
//					&this_t->ctxt,
//					&core_data->active_thread->ctxt);
//			bmk_debug("<-- bmk_scheduler_nonprimary::SWAP\n");
//		}
//
//		if (!bmk_sys_main_core_active()) {
//			break;
//		}
//
////		bmk_sys_wait_proc_event();
////
////		if (!bmk_sys_main_core_active()) {
////			break;
////		}
//	}

	bmk_debug("<-- bmk_scheduler_nonprimary()");
}


