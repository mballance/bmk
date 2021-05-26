/****************************************************************************
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
 * bmk_impl_sys_pthread.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 ****************************************************************************/
#include "bmk_impl_sys_pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct bmk_impl_sys_pthread_data_s {
	uint32_t				prv_nprocs;
	uint32_t				main_core_active;
	pthread_key_t			prv_key;
	pthread_mutex_t			prv_global_mutex;
	pthread_cond_t			prv_global_cond;
	uint32_t				prv_global_release;
	pthread_mutex_t			event_mutex;
	pthread_cond_t			event_cond;
	bmk_core_data_t			*prv_cores;
} bmk_impl_sys_pthread_data_t;

static bmk_impl_sys_pthread_data_t 		prv_impl_data = {0};

void bmk_sys_release_nonprimary_cores(void) {
	fprintf(stdout, "--> bmk_int_release_nonprimary_cores\n");
	pthread_mutex_lock(&prv_impl_data.prv_global_mutex);
	prv_impl_data.prv_global_release = 1;
	pthread_cond_broadcast(&prv_impl_data.prv_global_cond);
	pthread_mutex_unlock(&prv_impl_data.prv_global_mutex);
	fprintf(stdout, "<-- bmk_int_release_nonprimary_cores\n");
}

/**
 * Internal function called by the scheduler to notify cores that there
 * may be a change for them to react to
 */
void bmk_sys_core_notify_event(uint64_t mask) {
	// For now we just wakeup all processors
	pthread_mutex_lock(&prv_impl_data.event_mutex);
	pthread_cond_broadcast(&prv_impl_data.event_cond);
	pthread_mutex_unlock(&prv_impl_data.event_mutex);
}

/**
 * Internal function called by the scheduler to wait for an event
 */
void bmk_sys_core_wait_event(void) {
	pthread_mutex_lock(&prv_impl_data.event_mutex);
	pthread_cond_wait(&prv_impl_data.event_cond,
			&prv_impl_data.event_mutex);
	pthread_mutex_unlock(&prv_impl_data.event_mutex);
}

uint32_t bmk_get_nprocs(void) {
	return prv_impl_data.prv_nprocs;
}

uint32_t bmk_get_procid(void) {
	bmk_core_data_t *core_data =
			(bmk_core_data_t *)pthread_getspecific(prv_impl_data.prv_key);
	return core_data->procid;
}

void bmk_sys_core_init(int cid) {
	bmk_core_data_t *core_data =
			(bmk_core_data_t *)pthread_getspecific(prv_impl_data.prv_key);

	bmk_context_getcontext(&core_data->main_thread.ctxt);
	core_data->active_thread = &core_data->main_thread;
}

bmk_core_data_t *bmk_sys_get_core_data(void) {
	bmk_core_data_t *core_data =
			(bmk_core_data_t *)pthread_getspecific(prv_impl_data.prv_key);

	return core_data;
}

uint32_t bmk_sys_main_core_active(void) {
	uint32_t ret;

	pthread_mutex_lock(&prv_impl_data.prv_global_mutex);
	ret = prv_impl_data.main_core_active;
	pthread_mutex_unlock(&prv_impl_data.prv_global_mutex);

	return ret;
}

// This function is called for all non-primary cores
static void *bmk_pthread_core_main(void *ud) {
	bmk_core_data_t *core_data = (bmk_core_data_t *)ud;
//	bmk_core_data_t *primary_core = prv_impl_data.prv_cores;
	fprintf(stdout, "bmk_pthread_core_main %p\n", core_data);
	pthread_setspecific(prv_impl_data.prv_key, core_data);

	// Wait to be released
	fprintf(stdout, "--> wait unlock procid=%d\n", core_data->procid);
	pthread_mutex_lock(&prv_impl_data.prv_global_mutex);
	while (prv_impl_data.prv_global_release == 0) {
		pthread_cond_wait(&prv_impl_data.prv_global_cond,
				&prv_impl_data.prv_global_mutex);
	}
	pthread_mutex_unlock(&prv_impl_data.prv_global_mutex);
	fprintf(stdout, "<-- wait unlock procid=%d\n", core_data->procid);


	fprintf(stdout, "--> call non-zero main procid=%d\n", core_data->procid);
	bmk_startup(core_data->procid); // This is the main thread
	fprintf(stdout, "<-- call non-zero main procid=%d\n", core_data->procid);

	free(core_data);
	return 0;
}

void bmk_pthread_main(uint32_t n_cores) {
	uint32_t i;
	bmk_core_data_t *last_core_data = 0;

	memset(&prv_impl_data, 0, sizeof(bmk_impl_sys_pthread_data_t));

	prv_impl_data.prv_nprocs = n_cores;
	prv_impl_data.main_core_active = 1;

	pthread_mutex_init(&prv_impl_data.prv_global_mutex, 0);
	pthread_cond_init(&prv_impl_data.prv_global_cond, 0);

	pthread_mutex_init(&prv_impl_data.event_mutex, 0);
	pthread_cond_init(&prv_impl_data.event_cond, 0);

	pthread_key_create(&prv_impl_data.prv_key, 0);
//	bmk_pthread_scheduler_init();

	for (i=0; i<n_cores; i++) {
		bmk_core_data_t *core_data = (bmk_core_data_t *)malloc(
				sizeof(bmk_core_data_t));
		memset(core_data, 0, sizeof(bmk_core_data_t));

		pthread_mutex_init(&core_data->impl_data.proc_ev_mutex, 0);
		pthread_cond_init(&core_data->impl_data.proc_ev_cond, 0);

		core_data->procid = i;

		core_data->active_thread = &core_data->main_thread;

		// Core main threads can only run on their target processor
		core_data->main_thread.procmask = (1 << i);
		if (i == 0) {
			pthread_setspecific(prv_impl_data.prv_key, core_data);
			core_data->impl_data.thread = pthread_self();
			prv_impl_data.prv_cores = core_data;
			last_core_data = core_data;
		} else {
			fprintf(stdout, "--> Create core%d\n", i);
			pthread_create(&core_data->impl_data.thread, 0,
					&bmk_pthread_core_main, core_data);
			last_core_data->next = core_data;
			last_core_data = core_data;
			fprintf(stdout, "<-- Create core%d\n", i);
		}
	}

	// TODO: Wait for non-primary threads to start up
	last_core_data = prv_impl_data.prv_cores->next;
	while (last_core_data) {
		last_core_data = last_core_data->next;
	}

	bmk_startup(0); // This is the main thread

	pthread_mutex_lock(&prv_impl_data.prv_global_mutex);
	prv_impl_data.main_core_active = 1;
	pthread_mutex_unlock(&prv_impl_data.prv_global_mutex);

	// TODO: Once the main thread exits, notify the others that we're done

}

void bmk_sys_wait_proc_event(void) {
	bmk_core_data_t *core_data = bmk_sys_get_core_data();
	fprintf(stdout, "--> bmk_sys_wait_proc_event core_data=%p\n", core_data);
	pthread_mutex_lock(&core_data->impl_data.proc_ev_mutex);
	pthread_cond_wait(&core_data->impl_data.proc_ev_cond,
			&core_data->impl_data.proc_ev_mutex);
	pthread_mutex_unlock(&core_data->impl_data.proc_ev_mutex);
	fprintf(stdout, "<-- bmk_sys_wait_proc_event\n");
}

void bmk_sys_send_proc_event(uint8_t *mask, uint32_t mask_sz) {
	bmk_core_data_t *core_data = prv_impl_data.prv_cores;

	fprintf(stdout, "--> bmk_sys_send_proc_event\n");
	while (core_data) {
		pthread_mutex_lock(&core_data->impl_data.proc_ev_mutex);
		pthread_cond_broadcast(&core_data->impl_data.proc_ev_cond);
		pthread_mutex_unlock(&core_data->impl_data.proc_ev_mutex);
		core_data = core_data->next;
	}
	fprintf(stdout, "<-- bmk_sys_send_proc_event\n");
}

