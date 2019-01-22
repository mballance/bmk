
#include "bmk_int_sys.h"
#include "bmk_int_atomics.h"
#include <stdint.h>

static uint32_t uart_init = 0;
static bmk_atomic_t emit_lock;

void bmk_sys_emit(const char *str) {
	uintptr_t uart_ctrl_addr = 0x10000000;
	volatile uint8_t *uart_ctrl_p = (uint8_t *)uart_ctrl_addr;

	if (!uart_init) {
		uint32_t uart_freq = 1843200;
		uint32_t baud_rate = 115200;
		uint32_t divisor = uart_freq / (16 * baud_rate);

		bmk_atomics_init(&emit_lock);

		uart_ctrl_p[3] = 0x80; // enable DLAB
		uart_ctrl_p[1] = (divisor >> 8);
		uart_ctrl_p[0] = divisor;
		uart_ctrl_p[3] = 0x0f; // odd parity, 8-bit, no DLAB
		uart_init = 1;
	}

	bmk_atomics_lock(&emit_lock);
	while (*str) {
		// outchar
		while ((uart_ctrl_p[5] & 0x20) == 0) {
			;
		}
		uart_ctrl_p[0] = *str;
		str++;
	}
	bmk_atomics_unlock(&emit_lock);
}

void bmk_sys_exit(int code) {
	uintptr_t test_ctrl_addr = 0x100000;
	volatile uint32_t *test_ctrl_p = (uint32_t *)test_ctrl_addr;

	if (code == 0) {
		// Pass
		*test_ctrl_p = 0x5555;
	} else {
		// Fail
		*test_ctrl_p = 0x3333;
	}

	while (1) {
		asm volatile("");
	}
}

