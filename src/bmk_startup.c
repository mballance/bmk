/*
 * bmk_startup.c
 *
 *  Created on: Feb 21, 2018
 *      Author: ballance
 */
#include <stdint.h>
#include <stdio.h>
#include "bmk.h"
#include "bmk_int_sys.h"
#include "bmk_int_scheduler.h"
#include "bmk_core_info.h"
#include "bmk_sys_services.h"

// Called for the

/**
 *
 */
void bmk_startup(void) {
	// Allow the user's code to initialize some core hardware
	bmk_hardware_init();

	// Perform basic initialization running on core0
	// This might be as involved as waiting for a program to be loaded
	bmk_core0_init();

	// Perform single-core startup activities
	// This function will initialize enabled cores with the application stack
	bmk_level0_main();

	// TODO: Before exiting, notify nonprimary cores that we're ready for them


//	uint32_t i, nprocs;
//
//	if (cid == 0) {
//		// Initialize core_info array first
//		for (i=0; i<BMK_MAX_CORES; i++) {
//			_bmk_core_info[i].active = 0;
//			_bmk_core_info[i].ready_list = 0;
//// TODO
////			bmk_mutex_init(&_bmk_core_info[i].ready_list_mutex);
////			bmk_cond_init(&_bmk_core_info[i].ready_list_cond);
//		}
//
//		// Note that the primary core is active
//		_bmk_core_info[0].active = 1;
//
//		// Unblock non-primary cores
//		bmk_sys_release_nonprimary_cores();
//
//		// Wait for non-primary cores to wake up and register
//		nprocs = bmk_get_nprocs();
//		i=1;
//		while (i<nprocs) {
//			if (_bmk_core_info[i].active != 1) {
//				continue;
//			} else {
//				i++;
//			}
//		}
//
//		// Call the real main for c0
//		bmk_main();
//
//		// TODO: notify non-primary cores that they should exit
//	} else { // non-primary core
//
//		// Signal that we're active
//		_bmk_core_info[cid].active = 1;
//
//		// We enter with a stack, which we must now associate with a thread (?)
//		bmk_scheduler_nonprimary();
//	}
}



