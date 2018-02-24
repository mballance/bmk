
#include <stdint.h>
#include "bmk_config.h"

uint8_t _bmk_c0_stack[BMK_C0_STACK_SZ];
uint8_t _bmk_ex_stack[BMK_EX_STACK_SZ][BMK_MAX_CORES];
