/*
 * bmk_impl_sys_pthread.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_
#define SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_
#include <pthread.h>

typedef struct bmk_core_impl_data_s {
	pthread_t				thread;
	pthread_cond_t			proc_ev_cond;
	pthread_mutex_t			proc_ev_mutex;
} bmk_core_impl_data_t;

#include "bmk_int_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bmk_pthread_main()
 *
 * Main entry point. Call this function to launch and run
 */
void bmk_pthread_main(uint32_t n_cores);


#ifdef __cplusplus
}
#endif

#endif /* SRC_IMPL_SYS_PTHREAD_BMK_IMPL_SYS_PTHREAD_H_ */
