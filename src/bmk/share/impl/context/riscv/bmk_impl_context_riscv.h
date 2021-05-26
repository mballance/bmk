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
 *
 * bmk_impl_context_riscv.h
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 ****************************************************************************/

#ifndef INCLUDED_BMK_IMPL_CONTEXT_RISCV_H
#define INCLUDED_BMK_IMPL_CONTEXT_RISCV_H
// #include "bmk_impl_context_riscv.h"

#include <stdint.h>

// #include "bmk_impl_context_rv64_regs.h"

typedef struct bmk_context_s {
	uint32_t			gpr[32];
	/** TODO: need to recognize 32 vs 64 bit
	uint64_t			gpr[32];
	uint64_t			fpr[32];
	 */
} bmk_context_t;


#endif /* INCLUDED_BMK_IMPL_CONTEXT_RISCV_H */
