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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * bmk_get_clusterid()
 *
 *
 */

typedef void (*bmk_irq_handler_f)(void *ud, uint32_t id);

/**
 * bmk_set_irq_handler()
 *
 * Sets the user-interrupt handler for the specified core
 */
void bmk_set_irq_handler(
		uint32_t			procid,
		bmk_irq_handler_f	handler,
		void				*ud);

/**
 * bmk_main must be supplied by the user
 */
void bmk_main(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_BMK_H */

