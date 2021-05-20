/*
 * bmk.h
 *
 *  Created on: Feb 21, 2018
 *      Author: ballance
 */
#ifndef INCLUDED_BMK_H
#define INCLUDED_BMK_H
#include <stdint.h>
#include "bmk_thread_services.h"
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

