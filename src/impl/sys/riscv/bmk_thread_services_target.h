/*
 * bmk_thread_services_target.h
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_THREAD_SERVICES_TARGET_RISCV_H
#define INCLUDED_BMK_THREAD_SERVICES_TARGET_RISCV_H
#include "bmk_thread_types.h"

typedef struct bmk_thread_riscv_s {
	uint64_t					regs[31];
	// TODO: need a status register?
	void						*stk;
	uint32_t					stk_sz;
	bmk_thread_main_f			main_f;
	void						*ud;
	uint32_t					status;
	struct bmk_thread_core_s	*next; //
} bmk_thread_riscv_t;

typedef struct bmk_mutex_riscv_s {

} bmk_mutex_riscv_t;

typedef struct bmk_cond_riscv_s {

} bmk_cond_riscv_t;

typedef bmk_thread_riscv_t bmk_thread_t;
typedef bmk_mutex_riscv_t bmk_mutex_t;
typedef bmk_cond_riscv_t bmk_cond_t;


#endif /* INCLUDED_BMK_THREAD_SERVICES_TARGET_RISCV_H */
