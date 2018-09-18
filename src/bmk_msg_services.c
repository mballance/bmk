/*
 * bmk_msg_services.c
 *
 *  Created on: Sep 14, 2018
 *      Author: ballance
 */
#include <stdarg.h>
#include "bmk_msg_services.h"
#include "bmk_int_sys.h"
#include "libfemto/include/stdio.h"
#include "libfemto/include/stdlib.h"
#include "libfemto/include/string.h"

void bmk_info_low(const char *fmt, ...) {
	va_list ap;
	char buf[256];
	int res;

	strncpy(buf, "[INFO] ", sizeof(buf));

	va_start(ap, fmt);

	res = vsnprintf(&buf[7], sizeof(buf)-8-2, fmt, ap);
	buf[res+7] = '\n';
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	va_end(ap);
}

void bmk_info_mid(const char *fmt, ...) {
	va_list ap;
	char buf[256];
	int res;

	strncpy(buf, "[INFO] ", sizeof(buf));

	va_start(ap, fmt);

	res = vsnprintf(&buf[7], sizeof(buf)-8-2, fmt, ap);
	buf[res+7] = '\n';
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	va_end(ap);
}

void bmk_info_high(const char *fmt, ...) {
	va_list ap;
	char buf[256];
	int res;

	strncpy(buf, "[INFO] ", sizeof(buf));

	va_start(ap, fmt);

	res = vsnprintf(&buf[7], sizeof(buf)-8-2, fmt, ap);
	buf[res+7] = '\n';
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	va_end(ap);
}

void bmk_error(const char *fmt, ...) {
	va_list ap;
	char buf[256];
	int res;

	strncpy(buf, "[ERROR] ", sizeof(buf));

	va_start(ap, fmt);

	res = vsnprintf(&buf[8], sizeof(buf)-9-1, fmt, ap);
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	va_end(ap);
}

void bmk_fatal(const char *fmt, ...) {
	va_list ap;
	char buf[256];
	int res;

	strncpy(buf, "[FATAL] ", sizeof(buf));

	va_start(ap, fmt);

	res = vsnprintf(&buf[8], sizeof(buf)-9-1, fmt, ap);
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	// bail out
	// TODO: provide a dedicated BMK panic-exit call
	bmk_sys_exit(1);

	va_end(ap);
}

