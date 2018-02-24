/*
 * bmk.h
 *
 *  Created on: Feb 21, 2018
 *      Author: ballance
 */
#ifndef INCLUDED_BMK_H
#define INCLUDED_BMK_H
#include <stdint.h>

/**
 * bmk_get_nprocs()
 */
uint32_t bmk_get_nprocs(void);

/**
 * bmk_get_procid()
 */
uint32_t bmk_get_procid(void);

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

#endif /* INCLUDED_BMK_H */

