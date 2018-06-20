/*
 * bmk_impl_context_riscv.c
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#include "bmk_impl_context_riscv.h"
#include "bmk_thread_types.h"

void bmk_context_makecontext(
		bmk_context_t		*ctxt,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud) {
	// TODO:
}

void bmk_context_swapcontext(
		bmk_context_t		*from,
		bmk_context_t		*to) {
	// TODO:
}

