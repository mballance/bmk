/*
 * bmk_int_sys.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_INT_SYS_H
#define INCLUDED_BMK_INT_SYS_H
#include "bmk_thread_services.h"
#include "bmk_sys_services.h"
#include "bmk_config.h"
#define UNLOCK_KEY 0x55EA1234
#include "bmk_impl_sys.h"
#include "bmk_int_scheduler.h"

/**
 * BMK_SEMIHOST_ANN must be defined to whatever
 * annotation should be applied to semi-hosting
 * functions that must not be inlined
 */
#ifndef BMK_SEMIHOST_ANN
#define BMK_SEMIHOST_ANN __attribute__((noinline))
#endif

/**
 * BMK_SEMIHOST_STMT must be defined to a value
 * that will cause an empty function to not
 * be collapsed.
 */
#ifndef BMK_SEMIHOST_STMT
#define BMK_SEMIHOST_STMT asm("")
#endif

/**
 * Holds key core-specific data
 */
typedef struct bmk_core_data_s {
	uint32_t					exception_stack[BMK_EX_STACK_SZ];
	uint32_t					*main_sp;
	void (*main_f)(uint32_t);
	bmk_thread_t				main_thread;
	uint32_t					coreid;

	// Allow implementation to provide specific data if desired
	bmk_core_impl_data_t		impl_data;

	// Each core has specific scheduler data
	bmk_scheduler_impl_data_t	sched_data;
} bmk_core_data_t;

// Constant that can be used to obtain the core-data size
extern const uint32_t sizeof_bmk_core_data_s;
extern const uint32_t main_sp_offset;
extern const uint32_t main_f_offset;
extern const uint32_t coreid_offset;

extern bmk_core_data_t *core_data_p[BMK_MAX_CORES];

// Keep individual fields for simplicity of access from ASM

/**
 * Entrypoint to BMK-proper from boot code. Runs
 * any global and/or per-core initialization and executes
 * the main function for the core
 */
void bmk_entry(bmk_core_data_t *core_data);

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

/**
 * Initializes the specified core with a stack
 */
void bmk_sys_core_init(int cid, void *stk);

uint32_t bmk_sys_main_core_active(void);

void bmk_sys_wait_proc_event(void);

void bmk_sys_send_proc_event(bmk_cpuset_t *mask);

void bmk_sys_emit(const char *str);

void bmk_sys_exit(int code);

void bmk_sys_poweroff(void);

#endif /* INCLUDED_BMK_INT_SYS_H */


