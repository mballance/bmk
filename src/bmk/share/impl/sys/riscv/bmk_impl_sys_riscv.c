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
 * bmk_riscv.c
 *
 *  Created on: Feb 23, 2018
 *      Author: ballance
 ****************************************************************************/
#include "bmk.h"
#include "bmk_impl_sys_riscv.h"
#include "bmk_int_debug.h"
#include "bmk_config.h"

#define BMK_SYS_DEBUG

#ifdef BMK_SYS_DEBUG
#define bmk_sys_debug(...) bmk_debug(__VA_ARGS__)
#else
#define bmk_sys_debug(...)
#endif

uint32_t bmk_get_procid(void) {
	uint32_t ret;

	asm ("csrr %0, mhartid"
			: "=r" (ret));

	return ret;
}

void bmk_sys_riscv_trap(uint64_t mcause, uint64_t mepc) {
	bmk_core_data_t *cd = bmk_sys_get_core_data();

	/** TODO:
	if (cd->irq_handler) {
		cd->irq_handler(mcause);
	}
	 */
}

//void bmk_sys_enable_interrupts(void) {
///* TODO:
//	asm("csrwi mie, 0x800"); // MEIE
//	asm("csrwi mstatus, 0x8"); // MIE
// */
//}

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
}

/**
 * Internal function called by the scheduler to wait for an event
 */
void bmk_sys_core_wait_event(void) {
}

/**
 * Public function to return the number of processors
 */
uint32_t bmk_get_nprocs(void) {
	// TODO: Should query devicetree
	return BMK_NUM_CORES;
}

uint32_t bmk_sys_main_core_active(void) {
	// TODO:
	return 0;
}

void bmk_sys_enable_interrupts() {
	unsigned int mie;
    __asm__ volatile ("csrrs %0, mie, %1\n"
                      : "=r" (mie)
                      : "r" (1 << 11));
}

void bmk_sys_disable_interrupts() {
	unsigned int mie;
    __asm__ volatile ("csrrc %0, mie, %1\n"
                      : "=r" (mie)
                      : "r" (1 << 11));
}

#ifdef UNDEFINED
void bmk_sys_wait_proc_event(void) {
	bmk_core_data_t *core = bmk_sys_get_core_data();
//	bmk_sys_debug("--> bmk_sys_wait_proc_event() procid=%d", core->procid);

	while (!bmk_atomics_compare_and_swap(&core->impl_data.proc_event, 1, 0)) {
		// spin
	}
//	bmk_sys_debug("<-- bmk_sys_wait_proc_event() procid=%d", core->procid);
}

void bmk_sys_send_proc_event(bmk_cpuset_t *mask) {
	bmk_core_data_t *core = bmk_sys_get_core_data();
	uint32_t i;
	/*
	bmk_sys_debug("--> bmk_sys_send_proc_event() procid=%d", core->procid);

	if (mask) {

		for (i=0; i<sizeof(bmk_cpuset_t)/sizeof(uint32_t); i++) {
			if (mask->mask[i]) {
				uint32_t j;
				for (j=0; j<32; j++) {
					if (bmk_cpuset_isset(32*i+j, mask) && (32*i+j) < BMK_MAX_CORES) {
						bmk_sys_debug("-- bmk_sys_send_proc_event::signal core=%d procid=%d",
								32*i+j, core->procid);
						bmk_sys_data.core_data[32*i+j].impl_data.proc_event = 1;
					}
				}
			}
		}
	} else {
		// Notify all processors
		for (i=0; i<BMK_MAX_CORES; i++) {
			bmk_sys_data.core_data[i].impl_data.proc_event = 1;
		}
	}

	bmk_sys_debug("<-- bmk_sys_send_proc_event() procid=%d", core->procid);
	 */
}
#endif

