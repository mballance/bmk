/****************************************************************************
 * crt0_riscv.S
 *
 *
 ****************************************************************************/
#include "bmk_config.h"

#define UNLOCK_KEY 0x55EA1234

#define TRACEPOINT(x)
/*
#define TRACEPOINT(x) \
  li t0, 0x60001000; \
  li t1, 0x02a5; \
  sh t1, 0(t0); \
  li t1, x; \
  sll t1, t1, 8; \
  ori t1, t1, 0x05; \
  sw t1, 0(t0);
 */

// .section .data
// c0_ready: .word 0

.section .text.init, "ax", @progbits
.globl _start
_start:
	j	reset

nmi_vector:
	j	nmi_vector

trap_vector:
	j	trap_handler


reset:
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

  // Initialize exception handling
  la t0, trap_handler
  csrw mtvec, t0
  csrwi mstatus, 0
  csrwi mideleg, 0 // disable delegation
  csrwi medeleg, 0

  // Disable all interrupts
  csrwi mie, 0


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

  la sp, _memory_end
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
  la t1, _data_start
  la t2, _data_end
  la t3, _data_load
  // Don't relocate if we don't need to
  beq t1, t3, reloc_loop_end
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

  // On return, reconfigure the stack if one has been specified
  csrr t0, mhartid // HW Thread ID (core)
  li t1, 8
  mul t1, t0, t1 // determine the offset to the core stack
  la t2, core_stack
  add t2, t2, t1 // t2 now holds the address of the stack pointer
  ld t4, 0(t2)

  beq t4, zero, no_stack // If the stack pointer is zero, keep going
    // Otherwise, now determine the stack size
    li t1, 4
    mul t1, t0, t1
    la t2, core_stack_sz
    add t2, t2, t1
    lw t2, 0(t2)

    // Okay, now t2 has the stack size
    // and t4 has the stack base
    add t4, t2, t4
    add sp, t4, -8

no_stack:

  // Now that we've setup the stack,
  // call level1 main
  csrr t0, mhartid // HW Thread ID (core)
  addi a0, t0, 0 // pass the CID
  jal ra, _bmk_level1_main

  // On return,
  jal ra, bmk_sys_poweroff

  // On return, spin
1:
  TRACEPOINT(501)
	j	1b

/**
 * Startup for non-primary cores
 */
non_primary_startup:

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
  ld t1, 0(t1)     // Load the stack pointer

  beq t1, zero, nonprimary_idle  // No stack configured

  // Initialize the core's stack
  csrr	t0, mhartid // HW Thread Id
  slli	t0, t0, 2 // multiply by 4
  la	t2, core_stack_sz
  add	t2, t2, t0  // Offset to stack size
  lw	t2, 0(t2)	// Load the stack size
  add	t1, t1, t2	// Top of stack
  addi	sp, t1, -8

  csrr a0, mhartid // HW Thread ID (core)
  jal ra, _bmk_level1_main

  // Spin, since we haven't been activated
nonprimary_idle:
	wfi
	j nonprimary_idle

/********************************************************************
 * trap_handler
 *
 * Wrapper code prior to calling
 ********************************************************************/
trap_handler:
	addi	sp, sp, -(32*8)
	sd		x1, 1*8(sp)
	sd		x2, 2*8(sp)
	sd		x3, 3*8(sp)
	sd		x4, 4*8(sp)
	sd		x5, 5*8(sp)
	sd		x6, 6*8(sp)
	sd		x7, 7*8(sp)
	sd		x8, 8*8(sp)
	sd		x9, 9*8(sp)
	sd		x10, 10*8(sp)
	sd		x11, 11*8(sp)
	sd		x12, 12*8(sp)
	sd		x13, 13*8(sp)
	sd		x14, 14*8(sp)
	sd		x15, 15*8(sp)
	sd		x16, 16*8(sp)
	sd		x17, 17*8(sp)
	sd		x18, 18*8(sp)
	sd		x19, 19*8(sp)
	sd		x20, 20*8(sp)
	sd		x21, 21*8(sp)
	sd		x22, 22*8(sp)
	sd		x23, 23*8(sp)
	sd		x24, 24*8(sp)
	sd		x25, 25*8(sp)
	sd		x26, 26*8(sp)
	sd		x27, 27*8(sp)
	sd		x28, 28*8(sp)
	sd		x29, 29*8(sp)
	sd		x30, 30*8(sp)
	sd		x31, 31*8(sp)

	csrr a0, mcause
	csrr a1, mepc
	sd		a1, 0*8(sp)
	mv a2, sp
	jal bmk_sys_riscv_trap
	ld		a1, 0*8(sp)
	csrw mepc, a1

	// Stay in M-mode after return
//	li		t0, 0x1800
//	csrw	mstatus, t0

	ld		x1, 1*8(sp)
	ld		x2, 2*8(sp)
	ld		x3, 3*8(sp)
	ld		x4, 4*8(sp)
	ld		x5, 5*8(sp)
	ld		x6, 6*8(sp)
	ld		x7, 7*8(sp)
	ld		x8, 8*8(sp)
	ld		x9, 9*8(sp)
	ld		x10, 10*8(sp)
	ld		x11, 11*8(sp)
	ld		x12, 12*8(sp)
	ld		x13, 13*8(sp)
	ld		x14, 14*8(sp)
	ld		x15, 15*8(sp)
	ld		x16, 16*8(sp)
	ld		x17, 17*8(sp)
	ld		x18, 18*8(sp)
	ld		x19, 19*8(sp)
	ld		x20, 20*8(sp)
	ld		x21, 21*8(sp)
	ld		x22, 22*8(sp)
	ld		x23, 23*8(sp)
	ld		x24, 24*8(sp)
	ld		x25, 25*8(sp)
	ld		x26, 26*8(sp)
	ld		x27, 27*8(sp)
	ld		x28, 28*8(sp)
	ld		x29, 29*8(sp)
	ld		x30, 30*8(sp)
	ld		x31, 31*8(sp)

	addi	sp, sp, 32*8
	mret


.globl bmk_sys_enable_interrupts
bmk_sys_enable_interrupts:
	li		t0, 0x800
	csrw	mie, t0
	csrwi	mstatus, 0x8
	ret

.globl bmk_sys_disable_interrupts
bmk_sys_disable_interrupts:
	li		t0, ~0x8
	csrr	t1, mstatus
	and		t1, t1, t0
	csrw	mstatus, t1
	ret


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


