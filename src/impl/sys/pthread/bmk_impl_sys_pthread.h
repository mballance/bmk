/*
 * bmk_impl_sys_pthread.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_
#define SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_
#include "bmk_int_sys.h"
#include <pthread.h>

typedef struct bmk_core_data_pthread_s {
	bmk_core_data_base_t	base;
	pthread_t				thread;
} bmk_core_data_pthread_t;

typedef bmk_core_data_pthread_t bmk_core_data_t;

/**
 * bmk_pthread_main()
 *
 * Main entry point. Call this function to launch and run
 */
void bmk_pthread_main(uint32_t n_cores);



#endif /* SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_ */
