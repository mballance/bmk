/*
 * bmk_impl_context_riscv.c
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#include "../riscv/bmk_impl_context_riscv.h"

#include "bmk_thread_types.h"



//void _bmk_context_init(void);
//
//void bmk_context_makecontext(
//		bmk_context_t		*ctxt,
//		void				*stk,
//		uint32_t			stk_sz,
//		bmk_thread_main_f	main_f,
//		void				*ud) {
//	uintptr_t f_val = (uintptr_t)main_f;
//	uintptr_t stk_val = (uintptr_t)(stk+stk_sz-sizeof(uintptr_t));
//	uintptr_t ud_val = (uintptr_t)ud;
//
////	ctxt->gpr[RA] = 0; // Zero return address
//	ctxt->gpr[S0] = 0;
//	ctxt->gpr[S1] = f_val;
//	ctxt->gpr[S2] = ud_val; // Setup the arg for call to f_val
//	ctxt->gpr[SP] = stk_val;
//	// Jump to special initialization function
//	ctxt->gpr[RA] = (uintptr_t)&_bmk_context_init;
//	// TODO:
//}

void bmk_context_getcontext(
		bmk_context_t		*ctxt) {
	// NOP
}

//void bmk_context_swapcontext(
//		bmk_context_t		*from,
//		bmk_context_t		*to) {
//	// TODO:
//}

