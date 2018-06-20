/*
 * bmk_int_sys.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_INT_SYS_H
#define INCLUDED_BMK_INT_SYS_H
#include "bmk_thread_services.h"
#include "bmk_config.h"
#define UNLOCK_KEY 0x55EA1234

typedef struct bmk_core_data_s {
	uint32_t					procid;
	bmk_thread_t				*active_thread;
	bmk_thread_t				main_thread;

	struct bmk_core_data_s		*next;
} bmk_core_data_t;

typedef struct bmk_sys_data_s {
	uint32_t					c0_ready_key;
	uint32_t					core_release_mask[((BMK_MAX_CORES-1)/32)+1];
	bmk_core_data_t				core_data[BMK_MAX_CORES];
} bmk_sys_data_t;

extern bmk_sys_data_t			bmk_sys_data;

/**
 * Called by boot code to execute the startup procedure
 */
void bmk_startup(void);

void bmk_sys_init(void);


/**
 * Internal function called by the BMK core to release all non-primary cores
 */
void bmk_sys_release_nonprimary_cores(void);

/**
 * Internal function called by the scheduler to notify cores that there
 * may be a change for them to react to
 */
void bmk_sys_core_notify_event(uint64_t mask);

/**
 * Internal function called by the scheduler to wait for an event
 */
void bmk_sys_core_wait_event(void);

/**
 * Public function to return the number of processors
 */
uint32_t bmk_get_nprocs(void);

/**
 * Public function to return the current processor ID
 */
uint32_t bmk_get_procid(void);

/**
 * Returns the core data for the active processor
 */
bmk_core_data_t *bmk_sys_get_core_data(void);

uint32_t bmk_sys_main_core_active(void);

#endif /* INCLUDED_BMK_INT_SYS_H */
