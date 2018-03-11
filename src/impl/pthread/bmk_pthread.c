/*
 * bmk_pthreads.c
 *
 *  Created on: Feb 24, 2018
 *      Author: ballance
 */
#include "bmk_pthread.h"
#include "bmk_thread_services_target.h"
#include "bmk_pthread_scheduler.h"

#include "bmk_config.h"
#include "bmk_int.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static uint32_t				prv_nprocs;
static pthread_key_t		prv_key;
static pthread_mutex_t		prv_global_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t		prv_global_cond = PTHREAD_COND_INITIALIZER;
static uint32_t				prv_global_release = 0;
static core_data_t			*prv_cores = 0;


void bmk_int_release_nonprimary_cores(void) {
	pthread_mutex_lock(&prv_global_mutex);
	prv_global_release = 1;
	pthread_cond_broadcast(&prv_global_cond);
	pthread_mutex_unlock(&prv_global_mutex);
}

core_data_t *bmk_pthread_get_core_data(void) {
	return (core_data_t *)pthread_getspecific(prv_key);
}


// This function is called for all non-primary cores
void *bmk_pthread_core_main(void *ud) {
	core_data_t *core_data = (core_data_t *)ud;
	core_data_t *primary_core = prv_cores;
	pthread_setspecific(prv_key, core_data);

	// Wait to be released
	pthread_mutex_lock(&prv_global_mutex);
	while (prv_global_release == 0) {
		pthread_cond_wait(&prv_global_cond, &prv_global_mutex);
	}
	pthread_mutex_unlock(&prv_global_mutex);


	//
	while (1) {
		uint32_t is_running = 0;

	}

	bmk_startup(core_data->procid); // This is the main thread

	free(core_data);
	return 0;
}

uint32_t bmk_get_nprocs(void) {
	return prv_nprocs;
}

uint32_t bmk_get_procid(void) {
	core_data_t *core_data = (core_data_t *)pthread_getspecific(prv_key);
	return core_data->procid;
}

void bmk_pthread_main(uint32_t n_cores) {
	uint32_t i;
	core_data_t *last_core_data = 0;

	prv_nprocs = n_cores;

	pthread_key_create(&prv_key, 0);
	bmk_pthread_scheduler_init();

	for (i=0; i<n_cores; i++) {
		core_data_t *core_data = (core_data_t *)malloc(sizeof(core_data_t));
		memset(core_data, 0, sizeof(core_data_t));
		core_data->procid = i;

		core_data->active_thread = &core_data->main_thread;
		// Core main threads can only run on their target processor
		core_data->main_thread.procmask = (1 << i);
		if (i == 0) {
			pthread_setspecific(prv_key, core_data);
			core_data->thread = pthread_self();
			prv_cores = core_data;
			last_core_data = core_data;
		} else {
			pthread_create(&core_data->thread, 0,
					&bmk_pthread_core_main, core_data);
			last_core_data->next = core_data;
			last_core_data = core_data;
		}
	}

	// TODO: Wait for non-primary threads to start up
	last_core_data = prv_cores->next;
	while (last_core_data) {
		last_core_data = last_core_data->next;
	}

	bmk_main(); // This is the main thread

	// TODO: Once the main thread exits, notify the others that we're done

}

