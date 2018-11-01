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
#ifdef DEBUG_SCHEDULER
#define bmk_scheduler_debug(...) bmk_debug(__VA_ARGS__)
#else
#define bmk_scheduler_debug(...)
#endif

// #ifdef DEBUG_SCHEDULER
// #define scheduler_debug(...) bmk_scheduler_debug(__)

typedef struct bmk_scheduler_simple_data_s {
	bmk_atomic_t			lock;
	uint32_t				changed;
	bmk_thread_t			*thread_l;
	bmk_event_t				thread_exit_ev;

} bmk_scheduler_simple_data_t;

static bmk_scheduler_simple_data_t		prv_data = {0};

static bmk_thread_t *bmk_simple_scheduler_get_next_thread(
		bmk_core_data_t 	*core_data,
		bmk_thread_t		*start) {
	bmk_thread_t *t, *next_t = 0, *prev_t;
	bmk_scheduler_simple_data_t		*sched = &prv_data;

	bmk_atomics_lock(&prv_data.lock);
	bmk_scheduler_debug("--> bmk_simple_scheduler_get_next_thread start=%p procid=%d",
			start, core_data->procid);

#ifdef DEBUG_SCHEDULER
	{
		bmk_thread_t *thread_t = prv_data.thread_l;
		while (thread_t) {
		  bmk_scheduler_debug("  thread_t=%p state=%d procmask=0x%08x (procid=%d)",
				  thread_t, thread_t->sched_data.state, thread_t->procmask, core_data->procid);
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
		bmk_scheduler_debug("  t=%p state=%d procmask=0x%08x procid=%d next=%p",
				t, t->sched_data.state, t->procmask, core_data->procid,
				t->sched_data.next);
		if (t->sched_data.state == RunStateDead) {
			bmk_thread_t *dead_t = t;

			// Need to remove this thread from the run queue
//			bmk_info_low("-- remove dead thread %p", t);
			bmk_debug("-- remove dead thread %p (prev=%p next=%p)",
					t, prev_t, t->sched_data.next);
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
//				prev_t = t;
				t = t->sched_data.next;
			} else {
				prev_t = 0;
				t = prv_data.thread_l;
			}

			// Notify that 't' is now free
			bmk_thread_free(dead_t);

			if (t == start) {
				bmk_scheduler_debug("-- dead-thread wrap around");
				break;
			}
		} else {
			// Non-dead thread
			if (t->sched_data.state == RunStateIdle &&
					bmk_cpuset_isset(core_data->procid, &t->procmask)) {
				next_t = t;
				break;
			} else if (t->sched_data.state == RunStateBlocked &&
					t->sched_data.event &&
					bmk_atomics_compare_and_swap(&t->sched_data.event->event, 1, 0)) {
				//			bmk_scheduler_debug("-- Notice wakeup");
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
//					bmk_scheduler_debug("-- wrapped around");
					break;
				}
			}
		}
	}

	bmk_scheduler_debug("next_t = %p", next_t);
	bmk_scheduler_debug("<-- bmk_simple_scheduler_get_next_thread %p procid=%d", next_t, core_data->procid);

	bmk_atomics_unlock(&prv_data.lock);

	return next_t;
}

void bmk_scheduler_init(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_scheduler_debug("--> bmk_scheduler_init()");
	bmk_atomics_init(&prv_data.lock);
	bmk_event_init(&prv_data.thread_exit_ev);

	bmk_atomics_lock(&prv_data.lock);
	bmk_context_getcontext(&core_data->main_thread.ctxt);
	prv_data.thread_l = 0;
	// Pin this thread to the active core
	bmk_cpuset_zero(&core_data->main_thread.procmask);
	bmk_cpuset_set(core_data->procid, &core_data->main_thread.procmask);
	core_data->main_thread.sched_data.state = RunStateRunning;
	core_data->main_thread.sched_data.next = prv_data.thread_l;
	prv_data.thread_l = &core_data->main_thread;
	core_data->active_thread = &core_data->main_thread;
	bmk_atomics_unlock(&prv_data.lock);

	bmk_scheduler_debug("<-- bmk_scheduler_init()");
}

void bmk_scheduler_thread_new(bmk_thread_t *t) {
	bmk_scheduler_debug("--> bmk_scheduler_thread_new(%p)", t);
	bmk_atomics_lock(&prv_data.lock);
	t->sched_data.next = prv_data.thread_l;
	prv_data.thread_l = t;
	t->sched_data.state = RunStateIdle;
	t->sched_data.event = 0; // no events
	t->sched_data.refcnt = 1; // We're holding onto this thread
	bmk_atomics_unlock(&prv_data.lock);

	// Send events to processors this thread could run on
	bmk_sys_send_proc_event(&t->procmask);

	bmk_scheduler_debug("<-- bmk_scheduler_thread_new()");
}

void bmk_scheduler_thread_exit(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;
	bmk_scheduler_debug("--> bmk_scheduler_thread_exit(%p) procid=%d",
			active_t, core_data->procid);
//	bmk_atomics_lock(&prv_data.lock);

	// Mark this thread dead
	bmk_atomics_lock(&prv_data.lock);
	active_t->sched_data.state = RunStateDead;
	bmk_atomics_unlock(&prv_data.lock);

	// Notify any waiters
	bmk_event_signal(&prv_data.thread_exit_ev);

//	{
//		uint32_t i;
//		bmk_thread_t *tt = prv_data.thread_l;
//		bmk_info_low("bmk_scheduler_thread_exit: %p", t);
//
//		for (i=0; i<10 && tt; i++) {
//			bmk_info_low("  t=%p state=%d", tt, tt->sched_data.state);
//			tt = tt->sched_data.next;
//		}
//	}

	// Don't return
	bmk_scheduler_reschedule(1);

	bmk_error("thread exit reschedule returned");

//	bmk_scheduler_debug("<-- bmk_scheduler_thread_exit()");
}

void bmk_scheduler_thread_join(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_scheduler_debug("--> bmk_scheduler_thread_join t=%p procid=%d",
			t, core_data->procid);

	// We're now also hanging onto this thread
	t->sched_data.refcnt++;
	bmk_info_low("--> bmk_scheduler_join: %p", t);

	while (t->sched_data.state != RunStateDead) {
		bmk_event_wait(&prv_data.thread_exit_ev);
        bmk_info_low(" -- bmk_scheduler_join: wakeup %p %d", t, t->sched_data.state);
		bmk_scheduler_debug("  join: %p t.state=%d", t, t->sched_data.state);
	}

	bmk_info_low("<-- bmk_scheduler_join: %p", t);

	// Free this thread
	if (--t->sched_data.refcnt == 0) {
		// TODO: free this thread
	}

	bmk_scheduler_debug("<-- bmk_scheduler_thread_join t=%p procid=%d",
			t, core_data->procid);
}

/**
 * Blocks the active thread and waits for it to be awakened
 *
 * If lock is non-null, it is expected to be locked on
 * entry, unlocked while the thread is blocked, and
 * re-locked on exit
 */
void bmk_scheduler_thread_block(bmk_atomic_t *lock) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t=core_data->active_thread, *next_t;

	// The active thread is now blocked
	// Select another thread and swap to that one
	bmk_scheduler_debug("--> bmk_scheduler_thread_block proc=%d thread=%p",
			core_data->procid, active_t);

	// Thread cannot run
	bmk_atomics_lock(&prv_data.lock);
	active_t->sched_data.state = RunStateBlocked;
	bmk_atomics_unlock(&prv_data.lock);

	while (1) {
		if (lock) {
			bmk_atomics_unlock(lock);
		}
		bmk_scheduler_reschedule(1);

		if (lock) {
			bmk_atomics_lock(lock);
		}
		bmk_atomics_lock(&prv_data.lock);
		if (active_t->sched_data.state != RunStateBlocked) {
			bmk_atomics_unlock(&prv_data.lock);
			break;
		} else {
			bmk_atomics_unlock(&prv_data.lock);
		}
	}

	bmk_scheduler_debug("<-- bmk_scheduler_thread_block proc=%d thread=%p",
			core_data->procid, active_t);
}

void bmk_scheduler_thread_unblock(bmk_thread_t *t) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_scheduler_debug("--> bmk_scheduler_thread_unblock procid=%d thread=%p",
			core_data->procid, t);
	// Add back to the active list

	bmk_atomics_lock(&prv_data.lock);
	t->sched_data.state = RunStateIdle;
	bmk_atomics_unlock(&prv_data.lock);

	bmk_sys_send_proc_event(0);
	bmk_scheduler_debug("<-- bmk_scheduler_thread_unblock procid=%d thread=%p",
			core_data->procid, t);
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

	bmk_scheduler_debug("--> bmk_scheduler_reschedule active=%p procid=%d", active_t, core_data->procid);

	// Pass in null, since this thread will eventually be removed
	// If the thread is currently blocked, don't worry about
	// rescheduling the current thread
	bmk_atomics_lock(&prv_data.lock);
	if (active_t->sched_data.state == RunStateDead ||
			active_t->sched_data.state == RunStateBlocked) {
		active_t = 0;
	}
	bmk_atomics_unlock(&prv_data.lock);

	do {
		next_t = bmk_simple_scheduler_get_next_thread(core_data, active_t);

		if (wait && (!next_t || next_t == active_t)) {
			bmk_scheduler_debug("--> reschedule: bmk_sys_wait_proc_event procid=%d", core_data->procid);
			bmk_sys_wait_proc_event();
			bmk_scheduler_debug("<-- reschedule: bmk_sys_wait_proc_event procid=%d", core_data->procid);
		}
	} while ((!next_t || next_t == active_t) && wait);


	if (next_t) {
		bmk_thread_t *curr = core_data->active_thread;
		// Notify all processors that the schedule list has changed
		// TODO: not sure this impacts other threads
//		bmk_sys_send_proc_event(0);

		bmk_scheduler_debug("SWAP: %p => %p procid=%d",
				core_data->active_thread, next_t, core_data->procid);
		bmk_atomics_lock(&prv_data.lock);
		if (curr->sched_data.state == RunStateRunning) {
			// Move back to idle state if we were in the running state
			curr->sched_data.state = RunStateIdle;
		}
		next_t->sched_data.state = RunStateRunning;
		core_data->active_thread = next_t;
		bmk_atomics_unlock(&prv_data.lock);
		bmk_context_swapcontext(&curr->ctxt, &next_t->ctxt);
	}

	bmk_scheduler_debug("<-- bmk_scheduler_reschedule procid=%d", core_data->procid);
}

void bmk_scheduler_event_wait(bmk_event_t *e) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *active_t = core_data->active_thread;

	bmk_scheduler_debug("--> bmk_scheduler_event_wait");

	// First, check to see if the event has already been notified
	if (bmk_atomics_compare_and_swap(&e->event, 1, 0)) {
    	bmk_scheduler_debug("-- already set");
		return;
	}

	// Now, suspend the thread
	bmk_atomics_lock(&prv_data.lock);
	active_t->sched_data.event = e;
	active_t->sched_data.state = RunStateBlocked;
	bmk_atomics_unlock(&prv_data.lock);

	// Block this thread until an event awakens it
	bmk_scheduler_thread_block(0);

	// This thread is no longer waiting
	active_t->sched_data.event = 0;
	bmk_scheduler_debug("<-- bmk_scheduler_event_wait");
}

void bmk_scheduler_event_signal(bmk_event_t *e) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	uint8_t mask = (1 << core_data->procid);
	e->event = 1;

	// Notify all processors
	bmk_sys_send_proc_event(0);
}

void bmk_scheduler_nonprimary(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	bmk_thread_t *this_t = core_data->active_thread;
	uint32_t i, num_spins=100;
	bmk_scheduler_debug("--> bmk_scheduler_nonprimary()");

	bmk_scheduler_debug("--> bmk_scheduler_nonprimary::init()");
	bmk_context_getcontext(&core_data->main_thread.ctxt);

	// Pin this thread to the active core
	bmk_atomics_lock(&prv_data.lock);
	bmk_cpuset_zero(&core_data->main_thread.procmask);
	bmk_cpuset_set(core_data->procid, &core_data->main_thread.procmask);
	core_data->main_thread.sched_data.state = RunStateRunning;
	core_data->main_thread.sched_data.next = prv_data.thread_l;
	prv_data.thread_l = &core_data->main_thread;
	core_data->active_thread = &core_data->main_thread;
	bmk_atomics_unlock(&prv_data.lock);

	// This should act as 'slave'
	for (i=0; i<num_spins; i++) { // TODO: need an 'exit now' signal
		bmk_thread_t *next_t;

		bmk_scheduler_debug("--> bmk_scheduler_nonprimary::reschedule()");
		next_t = bmk_simple_scheduler_get_next_thread(
			core_data,
			core_data->active_thread);

		if (next_t && next_t != core_data->active_thread) {
			bmk_thread_t *curr = core_data->active_thread;
			// Swap to new thread
			bmk_atomics_lock(&prv_data.lock);
			if (curr->sched_data.state == RunStateRunning) {
				// Move back to idle state if we were in the running state
				curr->sched_data.state = RunStateIdle;
			}
			next_t->sched_data.state = RunStateRunning;
			core_data->active_thread = next_t;
			bmk_atomics_unlock(&prv_data.lock);
			bmk_context_swapcontext(&curr->ctxt, &next_t->ctxt);
		} else {
			// Wait for a core event
			bmk_sys_wait_proc_event();
		}
		bmk_scheduler_debug("<-- bmk_scheduler_nonprimary::reschedule()");
	}

	bmk_scheduler_debug("<-- bmk_scheduler_nonprimary()");
}


