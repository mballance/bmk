/*
 * bmk_core_info.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_CORE_INFO_H
#define INCLUDED_BMK_CORE_INFO_H
#include "bmk_thread_services_target.h"
#include "bmk_config.h"

typedef struct bmk_core_info_s {
	bmk_mutex_t			ready_list_mutex;
	bmk_cond_t			ready_list_cond;
	uint32_t			active;
	bmk_thread_t		main_thread;
	bmk_thread_t		*ready_list;
} bmk_core_info_t;

extern bmk_core_info_t		_bmk_core_info[];



#endif /* INCLUDED_BMK_CORE_INFO_H */
