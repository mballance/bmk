/*
 * bmk_riscv.c
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */
#include "bmk.h"
#include "bmk_impl_sys_riscv.h"
#include "bmk_config.h"

uint32_t bmk_get_procid(void) {
	uint32_t ret;

	asm ("csrr %0, mhartid"
			: "=r" (ret));

	return ret;
}

/**
 * Internal function called by the BMK core to release all non-primary cores
 */
void bmk_sys_release_nonprimary_cores(void) {
	// TODO:
}

/**
 * Internal function called by the scheduler to notify cores that there
 * may be a change for them to react to
 */
void bmk_sys_core_notify_event(uint64_t mask) {
	// TODO:
}

/**
 * Internal function called by the scheduler to wait for an event
 */
void bmk_sys_core_wait_event(void) {
	// TODO:
}

/**
 * Public function to return the number of processors
 */
uint32_t bmk_get_nprocs(void) {
	// TODO: Should query devicetree
	return BMK_NUM_CORES;
}

/**
 * Returns the core data for the active processor
 */
bmk_core_data_t *bmk_sys_get_core_data(void) {
	return &bmk_sys_data.core_data[bmk_get_procid()];
}

uint32_t bmk_sys_main_core_active(void) {
	// TODO:
	return 0;
}

void bmk_sys_wait_proc_event(void) {
	// TODO:
}

void bmk_sys_send_proc_event(uint8_t *mask, uint32_t mask_sz) {
	// TODO:
}

