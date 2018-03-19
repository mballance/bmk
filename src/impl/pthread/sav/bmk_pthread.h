/*
 * bmk_pthreads.h
 *
 *  Created on: Feb 27, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_PTHREAD_BMK_PTHREAD_H_
#define SRC_IMPL_PTHREAD_BMK_PTHREAD_H_
#include <stdint.h>
#include <pthread.h>
#include "bmk_pthread_thread_services.h"

#ifdef __cplusplus
extern "C" {
#endif


void bmk_pthread_main(uint32_t n_cores);

core_data_t *bmk_pthread_get_core_data(void);



#ifdef __cplusplus
}
#endif

#endif /* SRC_IMPL_PTHREAD_BMK_PTHREAD_H_ */
