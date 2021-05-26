/*****************************************************************************
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
 * bmk_msg_services.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ballance
 *
 *****************************************************************************/

#ifndef INCLUDED_BMK_MSG_SERVICES_H
#define INCLUDED_BMK_MSG_SERVICES_H
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


void bmk_info_low(const char *fmt, ...);

void bmk_info_low_v(const char *fmt, va_list ap);

void bmk_info_mid(const char *fmt, ...);

void bmk_info_mid_v(const char *fmt, va_list ap);

void bmk_info_high(const char *fmt, ...);

void bmk_info_high_v(const char *fmt, va_list ap);

void bmk_error(const char *fmt, ...);

void bmk_error_v(const char *fmt, va_list ap);

void bmk_fatal(const char *fmt, ...);

void bmk_fatal_v(const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_BMK_MSG_SERVICES_H */
