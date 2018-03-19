/*
 * bmk_riscv.c
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 */
#include "bmk.h"
#include "bmk_config.h"

uint32_t bmk_get_nprocs(void) {
	// TODO: Should query devicetree
	return BMK_NUM_CORES;
}

uint32_t bmk_get_procid(void) {
	uint32_t ret;

	asm ("csrr %0, mhartid"
			: "=r" (ret));

	return ret;
}
