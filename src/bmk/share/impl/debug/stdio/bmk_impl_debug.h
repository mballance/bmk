/*
 * bmk_impl_debug.h
 *
 *  Created on: Jun 19, 2018
 *      Author: ballance
 */

#ifndef INCLUDED_BMK_IMPL_DEBUG_STDIO_H
#define INCLUDED_BMK_IMPL_DEBUG_STDIO_H
#include <stdio.h>

#define bmk_debug(fmt, ...) fprintf(stdout, fmt, __VA_ARGS__); fflush(stdout)


#endif /* INCLUDED_BMK_IMPL_DEBUG_STDIO_H */

