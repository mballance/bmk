/****************************************************************************
 *
 * bmk.h
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
 *  Created on: Feb 21, 2018
 *      Author: ballance
 *
 ****************************************************************************/
#ifndef INCLUDED_BMK_H
#define INCLUDED_BMK_H
#include <stdint.h>
#include "bmk_thread_services.h"
#include "bmk_mem_services.h"
#include "bmk_msg_services.h"
#include "bmk_sys_services.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * bmk_main must be supplied by the user
 */
void bmk_main(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_BMK_H */

