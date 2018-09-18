/*
 * bmk_msg_services.h
 *
 *  Created on: Sep 14, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_MSG_SERVICES_H
#define INCLUDED_BMK_MSG_SERVICES_H

#ifdef __cplusplus
extern "C" {
#endif


void bmk_info_low(const char *fmt, ...);

void bmk_info_mid(const char *fmt, ...);

void bmk_info_high(const char *fmt, ...);

void bmk_error(const char *fmt, ...);

void bmk_fatal(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_BMK_MSG_SERVICES_H */
