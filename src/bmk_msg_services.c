/*
 * bmk_msg_services.c
 *
 *  Created on: Sep 14, 2018
 *      Author: ballance
 */
#include <stdarg.h>
#include <stddef.h>
#include "bmk_msg_services.h"
#include "bmk_int_sys.h"

static int vsnprintf(char * out, size_t n, const char* s, va_list vl);


void bmk_info_low(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	bmk_info_low_v(fmt, ap);

	va_end(ap);
}

void bmk_info_low_v(const char *fmt, va_list ap) {
	int res;
	char buf[256];
	char *ptr = buf;
	const char *pref = "[INFO] ";

	while (*pref) {
		*ptr = *pref;
		ptr++;
		pref++;
	}

	res = vsnprintf(ptr, sizeof(buf)-8-2, fmt, ap);
	ptr[res] = '\n';
	ptr[res+1] = 0;
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);
}

void bmk_info_mid(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	bmk_info_mid_v(fmt, ap);

	va_end(ap);
}

void bmk_info_mid_v(const char *fmt, va_list ap) {
	char buf[256];
	int res;
	char *ptr = buf;
	const char *pref = "[INFO] ";

	while (*pref) {
		*ptr = *pref;
		ptr++;
		pref++;
	}

	res = vsnprintf(ptr, sizeof(buf)-8-2, fmt, ap);
	ptr[res] = '\n';
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);
}

void bmk_info_high(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	bmk_info_high_v(fmt, ap);

	va_end(ap);
}

void bmk_info_high_v(const char *fmt, va_list ap) {
	char buf[256];
	int res;
	char *ptr = buf;
	const char *pref = "[INFO] ";

	while (*pref) {
		*ptr = *pref;
		ptr++;
		pref++;
	}

	res = vsnprintf(ptr, sizeof(buf)-8-2, fmt, ap);
	ptr[res] = '\n';
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);
}

void bmk_error(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	bmk_error_v(fmt, ap);

	va_end(ap);
}

void bmk_error_v(const char *fmt, va_list ap) {
	char buf[256];
	int res;
	char *ptr = buf;
	const char *pref = "[ERROR] ";

	while (*pref) {
		*ptr = *pref;
		ptr++;
		pref++;
	}

	res = vsnprintf(ptr, sizeof(buf)-9-1, fmt, ap);
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);
}

void bmk_fatal(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	bmk_fatal_v(fmt, ap);

	va_end(ap);
}

void bmk_fatal_v(const char *fmt, va_list ap) {
	char buf[256];
	int res;
	char *ptr = buf;
	const char *pref = "[FATAL] ";

	while (*pref) {
		*ptr = *pref;
		ptr++;
		pref++;
	}

	res = vsnprintf(ptr, sizeof(buf)-9-1, fmt, ap);
	buf[sizeof(buf)-1] = 0;

	// TODO: provide a BMK call for this
	bmk_sys_emit(buf);

	// bail out
	// TODO: provide a dedicated BMK panic-exit call
	bmk_sys_exit(1);
}

/**
 * vsnprintf is from libfemto, which is licensed under the following license:
 *
 * Copyright (c) 2013, The Regents of the University of California (Regents).
 * Copyright (c) 2018, Michael Clark <mjc@sifive.com>
 * All Rights Reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Regents nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 * SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 * OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF REGENTS HAS
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED
 * HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 */
int vsnprintf(char * out, size_t n, const char* s, va_list vl)
{
  int format = 0;
  int longarg = 0;
  size_t pos = 0;
  for( ; *s; s++)
  {
    if(format)
    {
      switch(*s)
      {
        case 'l':
          longarg = 1;
          break;
        case 'p':
          longarg = 1;
          if (++pos < n) out[pos-1] = '0';
          if (++pos < n) out[pos-1] = 'x';
        case 'x':
        {
          int i;
          long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
          for(i = 2*(longarg ? sizeof(long) : sizeof(int))-1; i >= 0; i--) {
            int d = (num >> (4*i)) & 0xF;
            if (++pos < n) out[pos-1] = (d < 10 ? '0'+d : 'a'+d-10);
          }
          longarg = 0;
          format = 0;
          break;
        }
        case 'd':
        {
          int i;
          long nn;
          long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
          if (num < 0) {
            num = -num;
            if (++pos < n) out[pos-1] = '-';
          }
          long digits = 1;
          for (nn = num; nn /= 10; digits++)
            ;
          for (i = digits-1; i >= 0; i--) {
            if (pos + i + 1 < n) out[pos + i] = '0' + (num % 10);
            num /= 10;
          }
          pos += digits;
          longarg = 0;
          format = 0;
          break;
        }
        case 's':
        {
          const char* s2 = va_arg(vl, const char*);
          while (*s2) {
            if (++pos < n)
              out[pos-1] = *s2;
            s2++;
          }
          longarg = 0;
          format = 0;
          break;
        }
        case 'c':
        {
          if (++pos < n) out[pos-1] = (char)va_arg(vl,int);
          longarg = 0;
          format = 0;
          break;
        }
        default:
          break;
      }
    }
    else if(*s == '%')
      format = 1;
    else
      if (++pos < n) out[pos-1] = *s;
  }
  if (pos < n)
    out[pos] = 0;
  else if (n)
    out[n-1] = 0;
  return pos;
}

