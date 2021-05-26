/*****************************************************************************
 *
 * Copyright 2018-2021 Matthew Ballance
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * bmk_stubs_types.h
 *
 *  Created on: Jun 20, 2018
 *      Author: ballance
 *****************************************************************************/

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

