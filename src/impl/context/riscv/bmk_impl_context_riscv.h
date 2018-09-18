/*
 * bmk_impl_context_riscv.h
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_IMPL_CONTEXT_RISCV_H
#define INCLUDED_BMK_IMPL_CONTEXT_RISCV_H
#include "bmk_impl_context_riscv.h"
#include "bmk_impl_context_riscv_regs.h"
#include <stdint.h>



typedef struct bmk_context_s {
	uint64_t			gpr[32];
	uint64_t			fpr[32];

} bmk_context_t;



#endif /* INCLUDED_BMK_IMPL_CONTEXT_RISCV_H */
