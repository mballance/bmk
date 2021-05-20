/*
 * bmk_stubs.h
 *
 *  Created on: Jun 20, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_STUBS_H
#define INCLUDED_BMK_STUBS_H
#include "bmk_thread_types.h"
#include "bmk_stubs_types.h"

extern bmk_stubs_t			*__bmk_stubs;

static inline void bmk_thread_init(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			main_f,
		void				*ud) {
	__bmk_stubs->bmk_thread_init(t, stk, main_f, ud);
}

void bmk_stubs_init(bmk_stubs_t *stubs);

#endif /* INCLUDED_BMK_STUBS_H */
