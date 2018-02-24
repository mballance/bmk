/****************************************************************************
 * crt0_riscv.S
 *
 *
 ****************************************************************************/
#include "bmk_config.h"

#define UNLOCK_KEY 0x55EA1234

.section .data
c0_ready: .word 0

.section .text.start, "ax", @progbits
.globl _start
_start:

/* Copied from libgloss */
.option push
.option norelax
/*
1:auipc gp, %pcrel_hi(__global_pointer$)
  addi  gp, gp, %pcrel_lo(1b)
 */
.option pop

  mv  x1, x0 // Zero out x1
  mv  x2, x1
  mv  x3, x1
  mv  x4, x1
  mv  x5, x1
  mv  x6, x1
  mv  x7, x1
  mv  x8, x1
  mv  x9, x1
  mv x10, x1
  mv x11, x1
  mv x12, x1
  mv x13, x1
  mv x14, x1
  mv x15, x1
  mv x16, x1
  mv x17, x1
  mv x18, x1
  mv x19, x1
  mv x20, x1
  mv x21, x1
  mv x22, x1
  mv x23, x1
  mv x24, x1
  mv x25, x1
  mv x26, x1
  mv x27, x1
  mv x28, x1
  mv x29, x1
  mv x30, x1
  mv x31, x1


  csrr t0, mhartid // HW Thread ID (core)
  bne t0, x0, non_primary_startup
  /**
   * Startup for the primary core
   */
primary_startup:
  // Ensure the other cores stay blocked
  la t1, c0_ready
  li t2, 0
  sw t2, 0(t1)

  la sp, _bmk_c0_stack
  li t1, BMK_C0_STACK_SZ
  add sp, sp, t1
  add sp, sp, -8

  // Clear BSS
  la t1, _bss_start
  la t2, _bss_end

  bge t1, t2, zero_loop_end
zero_loop:
  sw x0, 0(t1)
  addi t1, t1, 4
  ble t1, t2, zero_loop

zero_loop_end:

  // Run global initialization function
//  call __libc_init_array

  // Call BMK startup
  addi a0, t0, 0 // pass the CID
  jal ra, bmk_startup

/**
 * Startup for non-primary cores
 */
non_primary_startup:
  // Select the appropriate stack
  la sp, _bmk_c0_stack
  add t1, x0, t0 //
  li t2, BMK_EX_STACK_SZ
  mul t1, t1, t2 // Calculate end of the exception stack
  add sp, sp, t1
  add sp, sp, -8

  // Wait for C0 to unblock us
  // We don't want to stomp on its initialization procedure
  la t1, c0_ready
  li t2, UNLOCK_KEY
wait_c0:
  lw t3, 0(t1)
  bne t2, t3, wait_c0

  // Call BMK startup
  addi a0, t0, 0 // pass the CID
  jal ra, bmk_startup

.globl bmk_int_release_nonprimary_cores
bmk_int_release_nonprimary_cores:
	// Release the other cores
	la t0, c0_ready
	li t1, UNLOCK_KEY
	sw t1, 0(t0)
	ret

  .size  _start, .-_start

  .global _init
  .type   _init, @function
  .global _fini
  .type   _fini, @function


