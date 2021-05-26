/****************************************************************************
 * bmk_config.h
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
 ****************************************************************************/
#ifdef BMK_HAVE_CUSTOM_CONFIG_H
#include "bmk_custom_config.h"
#endif /* BMK_HAVE_CUSTOM_CONFIG_H */

#ifndef BMK_MAX_CORES
#define BMK_MAX_CORES 4
#endif

#ifndef BMK_NUM_CORES
#define BMK_NUM_CORES BMK_MAX_CORES
#endif

#ifndef BMK_EX_STACK_SZ
#define BMK_EX_STACK_SZ 64
#endif

// #define BMK_BSS_CLEARED

// #define BMK_HAVE_SEMIHOST

