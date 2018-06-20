/*
 * bmk_impl_context_uncontext.h
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 */

#ifndef SRC_IMPL_CONTEXT_UCONTEXT_BMK_IMPL_CONTEXT_UCONTEXT_H_
#define SRC_IMPL_CONTEXT_UCONTEXT_BMK_IMPL_CONTEXT_UCONTEXT_H_
#include <stdint.h>
#include <ucontext.h>

typedef ucontext_t	bmk_context_t;

#ifdef __cplusplus
extern "C" {
#endif

static inline void bmk_context_getcontext(
		bmk_context_t		*ctxt) {
	getcontext(ctxt);
}

static inline void bmk_context_swapcontext(bmk_context_t *from, bmk_context_t *to) {
	swapcontext(from, to);
}


#ifdef __cplusplus
}
#endif


#endif /* SRC_IMPL_CONTEXT_UCONTEXT_BMK_IMPL_CONTEXT_UCONTEXT_H_ */
