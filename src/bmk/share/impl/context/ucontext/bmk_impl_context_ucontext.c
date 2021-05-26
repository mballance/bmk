/****************************************************************************
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
 * bmk_impl_context_ucontext.c
 *
 *  Created on: Mar 14, 2018
 *      Author: ballance
 ****************************************************************************/
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

