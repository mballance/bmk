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



core_data_t *bmk_pthread_get_core_data(void) {
	return (core_data_t *)pthread_getspecific(prv_key);
}


core_data_t *bmk_get_core_data(void) {
	return (core_data_t *)pthread_getspecific(prv_key);
}

uint32_t bmk_get_procid(void) {
	core_data_t *core_data = (core_data_t *)pthread_getspecific(prv_key);
	return core_data->procid;
}


