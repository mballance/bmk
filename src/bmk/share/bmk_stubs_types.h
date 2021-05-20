/*
 * bmk_stubs_types.h
 *
 *  Created on: Jun 20, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_STUBS_TYPES_H
#define INCLUDED_BMK_STUBS_TYPES_H
#include "bmk_thread_types.h"

typedef struct bmk_stubs_s {

void (*bmk_thread_init)(
		bmk_thread_t		*t,
		void				*stk,
		uint32_t			stk_sz,
		bmk_thread_main_f	main_f,
		void				*ud);

} bmk_stubs_t;


#endif /* INCLUDED_BMK_STUBS_TYPES_H */

