/*
 * bmk_pthreads.c
 *
 *  Created on: Feb 24, 2018
 *      Author: ballance
 */
#include "bmk_config.h"
#include "bmk_pthreads.h"
#include "bmk_int.h"
#include <pthread.h>

static uint32_t				prv_nprocs;
static pthread_key_t		prv_key;
static pthread_mutex_t		prv_global_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t		prv_global_cond = PTHREAD_COND_INITIALIZER;
static uint32_t				prv_global_release = 0;

typedef struct core_data_s {
	uint32_t				procid;
	pthread_t				thread;
	bmk_thread_pthread_t	main_thread;
} core_data_t;

void bmk_int_release_nonprimary_cores(void) {
	pthread_mutex_lock(&prv_global_mutex);
	prv_global_release = 1;
	pthread_cond_broadcast(&prv_global_cond);
	pthread_mutex_unlock(&prv_global_mutex);
}

void *bmk_pthread_core_main(void *ud) {
	core_data_t *core_data = (core_data_t *)ud;
	pthread_setspecific(&prv_key, core_data);

	// Wait to be released
	pthread_mutex_lock(&prv_global_mutex);
	while (prv_global_release == 0) {
		pthread_cond_wait(&prv_global_cond, &prv_global_mutex);
	}
	pthread_mutex_unlock(&prv_global_mutex);

	bmk_startup(core_data->procid); // This is the main thread

	free(core_data);
	return 0;
}

uint32_t bmk_get_nprocs(void) {
	return prv_nprocs;
}

uint32_t bmk_get_procid(void) {
	core_data_t *core_data = (core_data_t *)pthread_getspecific(&prv_key);
	return core_data->procid;
}

void bmk_pthreads_main(uint32_t n_cores) {
	uint32_t i;

	prv_nprocs = n_cores;

	pthread_key_create(&prv_key, 0);

	for (i=0; i<n_cores; i++) {
		core_data_t *core_data = (core_data_t *)malloc(sizeof(core_data_t));
		memset(core_data, 0, sizeof(core_data_t));
		core_data->procid = i;

		if (i == 0) {
			pthread_setspecific(&prv_key, core_data);
			core_data->thread = pthread_self();
		} else {
			pthread_create(&core_data->thread, 0,
					&bmk_pthread_core_main, core_data);
		}
	}

	bmk_startup(0); // This is the main thread
}

