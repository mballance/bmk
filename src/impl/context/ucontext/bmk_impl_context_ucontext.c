/*
 * bmk_impl_context_ucontext.c
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */
#include "bmk_impl_context_ucontext.h"
#include "bmk_thread_types.h"

void bmk_context_makecontext(
		bmk_context_t		*ctxt,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud ) {

	getcontext(ctxt);
	ctxt->uc_stack.ss_sp = stk;
	ctxt->uc_stack.ss_size = stk_sz;
	ctxt->uc_stack.ss_flags = 0;
	ctxt->uc_link = 0;
	makecontext(ctxt, (void (*)())main_f, 1, ud);
}

