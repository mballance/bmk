/*
 * bmk_msg_services.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ballance
 */

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
