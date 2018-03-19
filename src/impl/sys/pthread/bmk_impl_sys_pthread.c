/*
 * bmk_impl_sys_pthread.c
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */
#include "bmk_impl_sys_pthread.h"

typedef struct bmk_impl_sys_pthread_data_s {
	uint32_t				prv_nprocs;
	pthread_key_t			prv_key;
	pthread_mutex_t			prv_global_mutex;
	pthread_cond_t			prv_global_cond;
	uint32_t				prv_global_release;
	pthread_mutex_t			event_mutex;
	pthread_mutex_t			event_cond;
	bmk_core_data_pthread_t	*prv_cores = 0;
} bmk_impl_sys_pthread_data_t;

static bmk_impl_sys_pthread_data_t 		prv_impl_data;

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
	bmk_core_data_pthread_t *core_data =
			(bmk_core_data_pthread_t *)pthread_getspecific(prv_impl_data.prv_key);
	return core_data->base.procid;
}

// This function is called for all non-primary cores
static void *bmk_pthread_core_main(void *ud) {
	bmk_core_data_pthread_t *core_data = (bmk_core_data_pthread_t *)ud;
	bmk_core_data_pthread_t *primary_core = prv_impl_data.prv_cores;
	pthread_setspecific(prv_impl_data.prv_key, core_data);

	// Wait to be released
	fprintf(stdout, "--> wait unlock procid=%d\n", core_data->base.procid);
	pthread_mutex_lock(&prv_impl_data.prv_global_mutex);
	while (prv_impl_data.prv_global_release == 0) {
		pthread_cond_wait(&prv_impl_data.prv_global_cond,
				&prv_impl_data.prv_global_mutex);
	}
	pthread_mutex_unlock(&prv_impl_data.prv_global_mutex);
	fprintf(stdout, "<-- wait unlock procid=%d\n", core_data->base.procid);


	fprintf(stdout, "--> call non-zero main procid=%d\n", core_data->base.procid);
	bmk_startup(core_data->base.procid); // This is the main thread
	fprintf(stdout, "<-- call non-zero main procid=%d\n", core_data->base.procid);

	free(core_data);
	return 0;
}

void bmk_pthread_main(uint32_t n_cores) {
	uint32_t i;
	bmk_core_data_pthread_t *last_core_data = 0;

	memset(&prv_impl_data, 0, sizeof(bmk_impl_sys_pthread_data_t));

	prv_impl_data.prv_nprocs = n_cores;

	pthread_mutex_init(&prv_impl_data.prv_global_mutex, 0);
	pthread_cond_init(&prv_impl_data.prv_global_cond, 0);

	pthread_mutex_init(&prv_impl_data.event_mutex, 0);
	pthread_cond_init(&prv_impl_data.event_cond, 0);

	pthread_key_create(&prv_impl_data.prv_key, 0);
//	bmk_pthread_scheduler_init();

	for (i=0; i<n_cores; i++) {
		bmk_core_data_pthread_t *core_data = (bmk_core_data_pthread_t *)malloc(
				sizeof(bmk_core_data_pthread_t));
		memset(core_data, 0, sizeof(bmk_core_data_pthread_t));
		core_data->base.procid = i;

		core_data->base.active_thread = &core_data->base.main_thread;

		// Core main threads can only run on their target processor
		core_data->base.main_thread.procmask = (1 << i);
		if (i == 0) {
			pthread_setspecific(prv_impl_data.prv_key, core_data);
			core_data->thread = pthread_self();
			prv_impl_data.prv_cores = core_data;
			last_core_data = core_data;
		} else {
			fprintf(stdout, "--> Create core%d\n", i);
			pthread_create(&core_data->thread, 0,
					&bmk_pthread_core_main, core_data);
			last_core_data->base.next = core_data;
			last_core_data = core_data;
			fprintf(stdout, "<-- Create core%d\n", i);
		}
	}

	// TODO: Wait for non-primary threads to start up
	last_core_data = prv_impl_data.prv_cores->base.next;
	while (last_core_data) {
		last_core_data = last_core_data->base.next;
	}

	bmk_startup(0); // This is the main thread

	// TODO: Once the main thread exits, notify the others that we're done

}


