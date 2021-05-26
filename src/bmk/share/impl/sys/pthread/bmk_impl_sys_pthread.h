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
 * bmk_impl_sys_pthread.h
 *
 *  Created on: Mar 17, 2018
 *      Author: ballance
 ****************************************************************************/

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
