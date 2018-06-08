/*
 * bmk_int_context.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_CONTEXT_BMK_INT_CONTEXT_H_
#define SRC_IMPL_CONTEXT_BMK_INT_CONTEXT_H_
#include "bmk_impl_context.h"
#include "bmk_thread_types.h"

void bmk_context_makecontext(
		bmk_context_t		*ctxt,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud);

void bmk_context_swapcontext(
		bmk_context_t		*from,
		bmk_context_t		*to);


#endif /* SRC_IMPL_CONTEXT_BMK_INT_CONTEXT_H_ */
