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

scheduler_t *bmk_pthread_get_scheduler(void) {
	return &prv_scheduler;
}

void bmk_pthread_scheduler_add2ready(bmk_thread_pthread_t *t) {
	pthread_mutex_lock(&prv_scheduler.mutex);
	fprintf(stdout, "Note: add2ready ready=%p t=%p\n",
			prv_scheduler.ready_thread_list, t);
	t->next = prv_scheduler.ready_thread_list;
	prv_scheduler.ready_thread_list = t;
	pthread_mutex_unlock(&prv_scheduler.mutex);
}

void bmk_pthread_scheduler_reschedule(core_data_t *core) {
	bmk_thread_pthread_t *t = 0, *tl = 0;

	pthread_mutex_lock(&prv_scheduler.mutex);
	// Add the current thread to the end of the list
	t = prv_scheduler.ready_thread_list;

	fprintf(stdout, "Note: reschedule ready=%p active=%p\n",
			prv_scheduler.ready_thread_list,
			core->active_thread);

	// Add the current thread to the end of the list
	if (t) {
		bmk_thread_pthread_t *next = 0;
		// search to the end
		while (t->next) {
			t = t->next;
		}

		// t now points to the final entry in the list
		t->next = core->active_thread;
		t->next->next = 0;

		// Now, search for the next thread
		t = prv_scheduler.ready_thread_list;
		while (t) {
			fprintf(stdout, "t=%p procmask=%d procid=%d\n",
					t, t->procmask, core->procid);
			if ((t->procmask == 0 && core->procid == 0) ||
					(t->procmask & (1 << core->procid))) {
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

		if (t != core->active_thread) {
			// Swap to the new thread
			bmk_thread_pthread_t *old = core->active_thread;
			core->active_thread = t;
			fprintf(stdout, "Note: swap %p => %p\n",
					old, t);
			swapcontext(
					&old->ctxt,
					&t->ctxt);
		} else {
			// Nothing to do, since we'll just return
			fprintf(stdout, "Note: same thread\n");
		}
	} else {
		// No active threads. This means that the active
		// thread is the only one
		fprintf(stdout, "Error: no active threads\n");
	}
	pthread_mutex_unlock(&prv_scheduler.mutex);

}

void bmk_pthread_scheduler_init(void) {
	memset(&prv_scheduler, 0, sizeof(bmk_thread_pthread_t));
	pthread_mutex_init(&prv_scheduler.mutex, 0);
	pthread_cond_init(&prv_scheduler.cond, 0);
}


