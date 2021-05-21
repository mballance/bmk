/*
 * bmk_sys_services.h
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_SYS_SERVICES_H
#define INCLUDED_BMK_SYS_SERVICES_H
#include <stdint.h>
#include "bmk_int_atomics.h"

/**
 * This function is used to register a core-specific
 * interrupt handler.
 */
typedef void (*bmk_irq_f)(uint64_t cause);

/**
 * bmk_get_nprocs()
 */
uint32_t bmk_get_nprocs(void);

/**
 * bmk_get_procid()
 */
uint32_t bmk_get_procid(void);

// BMK startup hook methods

void bmk_hardware_init();

void bmk_core0_init(void);

typedef void (*bmk_main_f)(void);

typedef void (*bmk_level1_main_f)(uint32_t cid);

void bmk_set_level0_main_func(bmk_main_f func);

//
void bmk_level0_main(void);

void bmk_sys_set_irq_handler(bmk_irq_f handler);

void bmk_sys_enable_interrupts(void);

void bmk_sys_disable_interrupts(void);


/**
 * Called by level0 main to initialize the
 * specified core. Only initialized cores
 * will execute
 */
void bmk_init_core(
		uint32_t 	cid,
		void 		(*main_f)(uint32_t),
		void 		*stk,
		uint32_t 	stk_sz);

void bmk_set_level1_main_func(bmk_level1_main_f func);

void bmk_level1_main(uint32_t cid);

void bmk_set_bmk_main_func(bmk_main_f func);

void bmk_main(void);



#endif /* INCLUDED_BMK_SYS_SERVICES_H */
