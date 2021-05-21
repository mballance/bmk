/****************************************************************************
 * crt0_riscv.S
 *
 *
 ****************************************************************************/
#include "bmk_config.h"

// .section .data
// c0_ready: .word 0

.section .text.start, "ax", @progbits
.globl _start
_start:
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

  // Initialize exception handling
	la t0, trap_handler
	csrw mtvec, t0

/*
	csrwi mstatus, 0
	csrwi mideleg, 0 // disable delegation
	csrwi medeleg, 0

  // Disable all interrupts
  csrwi mie, 0
 */

	// Locate the core-specific core data using
	// the hartid
	la t0, core_data
	la t1, sizeof_bmk_core_data_s
	lw t1, 0(t1)

	// Poor-man's multiply to determine which coredata is ours
	csrr t2, mhartid
	beqz t2, 2f
1:
	add t0, t0, t1
	addi t2, t2, -1
	bnez t2, 1b
2:
	// Zero out the main sp for this core
	la t4, main_sp_offset
	lw t4, 0(t4)
	add t4, t0, t4
	sw zero, 0(t4)

	// Save the base of the core-data structure for later
	mv t3, t0
	csrw mscratch, t3

	csrr t2, mhartid

	// Fill in the coreid field
	la t5, coreid_offset
	lw t5, 0(t5)
	add t0, t3, t5
	sw t2, 0(t0)

	// Now, decide whether we're the boot core or a
	// secondary core
	beqz t2, primary_core_startup

	// Load up main_f for this core
	la t5, main_f_offset
	lw t5, 0(t5)
	add t5, t3, t5

	// Enable interrupts for the non-primary core,
	// since we're relying on being awakened by
	// a software interrupt
	li    t0, (1 << 11)
	csrrs t0, mie, t0

	// Note: expects the following to be preserved
	// t4 -- points to main_sp for the core
	// t5 -- points to main_f for this core
nonprimary_core_park:
	wfi
	// Load the primary stack-pointer location
	lw		t0, 0(t4)

	// See if we have a stack yet
	beqz	t0, nonprimary_core_park

	// Disable interrupts prior to entry
	li    t1, (1 << 11)
	csrrc t1, mie, t1

	// Load the entry function
	lw		t1, 0(t5)

	// parameter is coreid
	mv		a0, t2
	mv		sp, t0

	jalr	t1

	nop
	nop
	nop
	nop

	// When we return, spin forever
1:
	wfi
	j		1b

	// Startup process for the primary core
primary_core_startup:

	// Initialize stack to the global stack
	la sp, _stack_top
	addi sp, sp, -4
	// Store the stack top to core-data
	sw sp, 0(t3)

#ifndef BMK_BSS_CLEARED
	/* clear BSS */
	la x26, _bss_start
	la x27, _bss_end

	bge x26, x27, zero_loop_end

zero_loop:
	sw x0, 0(x26)
	addi x26, x26, 4
	ble x26, x27, zero_loop
zero_loop_end:
#endif /* BMK_BSS_CLEARED */

	// TODO: Call basic initialization

	// Call main() with coreid as the single parameter
	csrr a0, mhartid
	call main

	nop
	nop
	nop
	nop

	/*

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
	*/

  // Run global initialization function
//  call __libc_init_array


  // On return, spin
1:
	wfi
	j	1b

/********************************************************************
 * trap_handler
 *
 * Wrapper prior to calling into C code
 ********************************************************************/
trap_handler:
	// Save the current SP and grab the core-data pointer
	csrrw sp, mscratch, sp

    // Find the top
    addi sp, sp, 256-4

	// Now, save everything
	sw x1, 0(sp)
	// Note: we'll come back for sp
	sw x3, -8(sp)
	sw x4, -12(sp)
	sw x5, -16(sp)
	sw x6, -20(sp)
	sw x7, -24(sp)
	sw x8, -28(sp)
	sw x9, -32(sp)
	sw x10, -36(sp)
	sw x11, -40(sp)
	sw x12, -44(sp)
	sw x13, -48(sp)
	sw x14, -52(sp)
	sw x15, -56(sp)
	sw x16, -60(sp)
	sw x17, -64(sp)
	sw x18, -68(sp)
	sw x19, -72(sp)
	sw x20, -76(sp)
	sw x21, -80(sp)
	sw x22, -84(sp)
	sw x23, -88(sp)
	sw x24, -92(sp)
	sw x25, -96(sp)
	sw x26, -100(sp)
	sw x27, -104(sp)
	sw x28, -108(sp)
	sw x29, -112(sp)
	sw x30, -116(sp)
	sw x31, -120(sp)

	// Save the original stack pointer
	csrr t0, mscratch
	sw t0, -4(sp)

	// Restore the core-data pointer in mscratch
    addi a0, sp, -(256-4)
    csrw mscratch, a0

	// Save the mepc
	csrr t0, mepc
	sw t0, -124(sp)

	// Adjust the sp before calling
	addi sp, sp, -128
	csrr a1, mcause

	/**
	 * exception_handler(core_data, mcause)
	 */
	la t0, bmk_exception_handler
	jalr ra, t0

	// On return, re-acquire the core-data from mscratch
	csrr sp, mscratch
	addi sp, sp, 256-4

	// Re-load mepc
	lw t0, -124(sp)
	csrw mepc, t0

	// Re-load all registers except old sp
	lw x1, 0(sp)
	// Note: we'll come back for sp
	lw x3, -8(sp)
	lw x4, -12(sp)
	lw x5, -16(sp)
	lw x6, -20(sp)
	lw x7, -24(sp)
	lw x8, -28(sp)
	lw x9, -32(sp)
	lw x10, -36(sp)
	lw x11, -40(sp)
	lw x12, -44(sp)
	lw x13, -48(sp)
	lw x14, -52(sp)
	lw x15, -56(sp)
	lw x16, -60(sp)
	lw x17, -64(sp)
	lw x18, -68(sp)
	lw x19, -72(sp)
	lw x20, -76(sp)
	lw x21, -80(sp)
	lw x22, -84(sp)
	lw x23, -88(sp)
	lw x24, -92(sp)
	lw x25, -96(sp)
	lw x26, -100(sp)
	lw x27, -104(sp)
	lw x28, -108(sp)
	lw x29, -112(sp)
	lw x30, -116(sp)
	lw x31, -120(sp)

	// Restore the original stack pointer
	lw sp, -4(sp)

	// Back where we came from
	mret

/*
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
 */


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


