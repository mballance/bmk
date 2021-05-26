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
 * bmk_impl_sys_riscv.h
 *
 *  Created on: Jun 18, 2018
 *      Author: ballance
 *
 ****************************************************************************/

#ifndef INCLUDED_BMK_IMPL_SYS_RISCV_H
#define INCLUDED_BMK_IMPL_SYS_RISCV_H

typedef struct bmk_core_impl_data_s {

	// Counter to track events
	uint32_t		proc_event;

} bmk_core_impl_data_t;

#include "bmk_int_sys.h"


#endif /* INCLUDED_BMK_IMPL_SYS_RISCV_H */
