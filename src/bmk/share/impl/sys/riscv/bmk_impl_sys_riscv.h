/*
 * bmk_impl_sys_riscv.h
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_IMPL_SYS_RISCV_H
#define INCLUDED_BMK_IMPL_SYS_RISCV_H

typedef struct bmk_core_impl_data_s {

	// Flag to signal a processor event
	uint32_t		proc_event;

} bmk_core_impl_data_t;

#include "bmk_int_sys.h"




#endif /* INCLUDED_BMK_IMPL_SYS_RISCV_H */
