/****************************************************************************
 * bmk_config.h
 ****************************************************************************/
#ifdef BMK_HAVE_CUSTOM_CONFIG_H
#include "bmk_custom_config.h"
#endif BMK_HAVE_CUSTOM_CONFIG_H

#ifndef BMK_MAX_CORES
#define BMK_MAX_CORES 4
#endif

//#ifndef BMK_NUM_CORES
//#define BMK_NUM_CORES BMK_MAX_CORES
//#endif

#ifndef BMK_C0_STACK_SZ
#define BMK_C0_STACK_SZ 512
#endif

#ifndef BMK_EX_STACK_SZ
#define BMK_EX_STACK_SZ 512
#endif
