/****************************************************************************
 * crt0_riscv.S
 *
 *
 ****************************************************************************/
#include "bmk_config.h"

#define UNLOCK_KEY 0x55EA1234

#define TRACEPOINT(x) \
  li t0, 0x60001000; \
  li t1, 0x02a5; \
  sh t1, 0(t0); \
  li t1, x; \
  sll t1, t1, 8; \
  ori t1, t1, 0x05; \
  sw t1, 0(t0);

// .section .data
// c0_ready: .word 0

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

//  TRACEPOINT(100)


  csrr t0, mhartid // HW Thread ID (core)
  bne t0, x0, non_primary_startup

  /**
   * Startup for the primary core
   */
primary_startup:

  TRACEPOINT(200)
  // Ensure the other cores stay blocked: bmk_sys_data.c0_ready_key = 0
  la t1, c0_ready_key
  li t2, 0
  sw t2, 0(t1)

  la sp, _stack_top
  add sp, sp, -8

  TRACEPOINT(300)

  // Clear BSS
  la t1, _bss_start
  la t2, _bss_end

  bge t1, t2, zero_loop_end
zero_loop:
  sw x0, 0(t1)
  addi t1, t1, 4
  ble t1, t2, zero_loop

zero_loop_end:
  TRACEPOINT(301)

  // Target address range is __data_start to _edata
  la t1, __data_start
  la t2, _edata
  la t3, __data_load
reloc_loop:
  lw t4, 0(t3)
  sw t4, 0(t1)
  addi t1, t1, 4
  addi t3, t3, 4
  ble t1, t2, reloc_loop
reloc_loop_end:

  TRACEPOINT(400)

  // Run global initialization function
//  call __libc_init_array

  // Call BMK startup
  jal ra, bmk_startup

  TRACEPOINT(500)

  // On return, reconfigure the stack and
  // TODO: reconfigure stack
  csrr t0, mhartid // HW Thread ID (core)
  addi a0, t0, 0 // pass the CID
  jal ra, bmk_level1_main

  // On return, spin
1:
  TRACEPOINT(501)
	j	1b

/**
 * Startup for non-primary cores
 */
non_primary_startup:
//  TRACEPOINT(600)

  // Kill some time to ensure C0 can initialize things
  li t1, 0
  li t2, 100
1:
  addi t1, t1, 1
  bne t1, t2, 1b

  // Wait for c0_ready_key to be set to the KEY value
  la t0, c0_ready_key
  li t1, UNLOCK_KEY
1:
  lw t2, 0(t0)
  bne t1, t2, 1b

  // Load the stack pointer
  csrr t0, mhartid // HW Thread ID (core)
  la t1, core_stack
  li t2, 8
  mul t0, t0, t2    // determine the offset
  add t1, t1, t0
  lw t1, 0(t1)     // Load the stack pointer

  beq t1, zero, nonprimary_idle  // No stack configured

  // Initialize the core's stack

  // Spin, since we haven't been activated
nonprimary_idle:
	j nonprimary_idle

  // Select the appropriate stack
//  la sp, _bmk_c0_stack
//  add t1, x0, t0 //
//  li t2, BMK_EX_STACK_SZ
//  mul t1, t1, t2 // Calculate end of the exception stack
//  add sp, sp, t1
//  add sp, sp, -8

  // Wait for C0 to unblock us
  // We don't want to stomp on its initialization procedure
//  la t1, c0_ready
//  li t2, UNLOCK_KEY
//wait_c0:
//  lw t3, 0(t1)
//  bne t2, t3, wait_c0

//  // Call BMK startup
//  addi a0, t0, 0 // pass the CID
//  jal ra, bmk_startup

.globl bmk_int_release_nonprimary_cores
bmk_int_release_nonprimary_cores:
	// Release the other cores
//	la t0, c0_ready
//	li t1, UNLOCK_KEY
//	sw t1, 0(t0)
	ret

  .size  _start, .-_start

  .global _init
  .type   _init, @function
  .global _fini
  .type   _fini, @function


