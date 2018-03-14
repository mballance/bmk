/*
 * bmk_pthread_scheduler.h
 *
 *  Created on: Mar 10, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_PTHREAD_BMK_PTHREAD_SCHEDULER_H_
#define SRC_IMPL_PTHREAD_BMK_PTHREAD_SCHEDULER_H_
#include <pthread.h>
#include "bmk_pthread_thread_services.h"
#include "bmk_pthread.h"

typedef struct scheduler_s {
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	bmk_thread_pthread_t	*ready_thread_list;
	bmk_thread_pthread_t	*dead_thread_list;
} scheduler_t;

void bmk_pthread_scheduler_init(void);

void bmk_pthread_scheduler_add2ready(bmk_thread_pthread_t *t);

void bmk_pthread_scheduler_thread_exit(bmk_thread_pthread_t *t);


#endif /* SRC_IMPL_PTHREAD_BMK_PTHREAD_SCHEDULER_H_ */
