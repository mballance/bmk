/*
 * bmk_pthread_scheduler.c
 *
 *  Created on: Mar 10, 2018
 *      Author: ballance
 */
#include "bmk_pthread_scheduler.h"
#include <stdio.h>
#include <string.h>

static scheduler_t			prv_scheduler;

static bmk_thread_pthread_t *select_next_thread(uint32_t procid);

scheduler_t *bmk_pthread_get_scheduler(void) {
	return &prv_scheduler;
}

void bmk_pthread_scheduler_add2ready(bmk_thread_pthread_t *t) {
	pthread_mutex_lock(&prv_scheduler.mutex);
	fprintf(stdout, "Note: add2ready ready=%p t=%p\n",
			prv_scheduler.ready_thread_list, t);
	t->next = prv_scheduler.ready_thread_list;
	prv_scheduler.ready_thread_list = t;

	// Notify any waiting threads that something has changed
	pthread_cond_broadcast(&prv_scheduler.cond);

	pthread_mutex_unlock(&prv_scheduler.mutex);
}

void bmk_pthread_scheduler_block(bmk_thread_pthread_t *t) {

}

void bmk_pthread_scheduler_unblock(bmk_thread_pthread_t *t) {

}

void bmk_pthread_scheduler_thread_exit(bmk_thread_pthread_t *t) {
	bmk_thread_pthread_t *next = 0;
	// TODO: free data structure associated with thread
	fprintf(stdout, "TODO: bmk_pthread_scheduler_thread_exit()\n");
	fflush(stdout);

	// Notify listeners of a change
	pthread_mutex_lock(&prv_scheduler.mutex);
	t->dead = 1;
	pthread_cond_broadcast(&prv_scheduler.cond);
	pthread_mutex_unlock(&prv_scheduler.mutex);

	// TODO: select next thread to run
	fprintf(stdout, "--> select_next_thread\n");
	fflush(stdout);
	next = select_next_thread(bmk_pthread_get_core_data()->procid);
	fprintf(stdout, "<-- select_next_thread next=%p\n", next);
	fflush(stdout);
	swapcontext(&t->ctxt, &next->ctxt);
}

void bmk_pthread_scheduler_nonprimary() {
	while (1) {
		fprintf(stdout, "--> nonprimary reschedule\n");
		bmk_pthread_scheduler_reschedule(bmk_get_core_data(), 1);
		fprintf(stdout, "<-- nonprimary reschedule\n");
	}
}

void bmk_pthread_scheduler_reschedule(core_data_t *core, uint32_t wait) {
	bmk_thread_pthread_t *t = 0, *tl = 0;
	bmk_thread_pthread_t *active = core->active_thread;
	uint32_t new_thread = 0;

	fprintf(stdout, "--> bmk_pthread_scheduler_reschedule procid=%d wait=%d\n",
			core->procid, wait);

	while (1) {
	pthread_mutex_lock(&prv_scheduler.mutex);
	if (!prv_scheduler.ready_thread_list) {
		// We already know we're the sole remaining thread
		pthread_mutex_unlock(&prv_scheduler.mutex);
//		return;
	} else {
		// Add the current thread to the end of the list
		t = prv_scheduler.ready_thread_list;
		fprintf(stdout, "Note: reschedule ready=%p active=%p\n",
			prv_scheduler.ready_thread_list,
			core->active_thread);

		// Add the current thread to the end of the list
		bmk_thread_pthread_t *next = 0;
		// search to the end
		while (t->next) {
			t = t->next;
		}
		// t now points to the final entry in the list
		t->next = core->active_thread;
		t->next->next = 0;

		pthread_mutex_unlock(&prv_scheduler.mutex);

		t = select_next_thread(core->procid);

		if (t != core->active_thread) {
			// Swap to the new thread
			new_thread = 1;
		}
	}

	if (new_thread) {
		bmk_thread_pthread_t *old = core->active_thread;
		// If there's a new active thread, notify others
		// TODO: notify change in active list
		pthread_mutex_lock(&prv_scheduler.mutex);
		pthread_cond_broadcast(&prv_scheduler.cond);
		pthread_mutex_unlock(&prv_scheduler.mutex);

		core->active_thread = t;
		fprintf(stdout, "Note: swap %p => %p\n",
				old, t);
		swapcontext(
				&old->ctxt,
				&t->ctxt);
		break; // return after a successful swap
	} else if (wait) {
		// Wait for the scheduler state to change
		fprintf(stdout, "--> wait for scheduler state change %d\n", wait);
		// TODO: wait for a change in the active list
		pthread_mutex_lock(&prv_scheduler.mutex);
		pthread_cond_wait(&prv_scheduler.cond, &prv_scheduler.mutex);
		pthread_mutex_unlock(&prv_scheduler.mutex);
		fprintf(stdout, "<-- wait for scheduler state change\n");
	} else {
		//
		break;
	}
	}

	fprintf(stdout, "<-- bmk_pthread_scheduler_reschedule procid=%d wait=%d\n",
			core->procid, wait);
}

static bmk_thread_pthread_t *select_next_thread(uint32_t procid) {
	bmk_thread_pthread_t *t = 0, *tl = 0;
	// TODO: lock the scheduler list
	pthread_mutex_lock(&prv_scheduler.mutex);

	// Now, search for the next thread
	t = prv_scheduler.ready_thread_list;
	while (t) {
		fprintf(stdout, "t=%p procmask=%d procid=%d\n",
				t, t->procmask, procid);
		if ((t->procmask == 0 && procid == 0) ||
				(t->procmask & (1 << procid))) {
			break;
		}
		tl = t; // track the previous entry
		t = t->next;
	}

	// remove the new thread from the active list
	if (tl) {
		tl->next = t->next;
	} else {
		// No previous, so update the 'head' link
		prv_scheduler.ready_thread_list = t->next;
	}

	fprintf(stdout, "POST: target=%p ready-list is:\n", t);
	tl = prv_scheduler.ready_thread_list;
	while (tl) {
		fprintf(stdout, "  thread=%p\n", tl);
		tl = tl->next;
	}

	// TODO: unlock the scheduler list
	pthread_mutex_unlock(&prv_scheduler.mutex);

	return t;
}

void bmk_pthread_scheduler_init(void) {
	memset(&prv_scheduler, 0, sizeof(bmk_thread_pthread_t));
	pthread_mutex_init(&prv_scheduler.mutex, 0);
	pthread_cond_init(&prv_scheduler.cond, 0);
}


