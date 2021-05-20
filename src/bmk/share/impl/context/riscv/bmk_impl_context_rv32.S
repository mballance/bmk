
#include "bmk_impl_context_rv32_regs.h"

.section .text
// .globl _bmk_context_init
.globl bmk_context_swapcontext

/**
 * On entry:
 * s1 is the function to call
 */
_bmk_context_init:
  mv		a0, s2 // user-data value
  jr		s1

// a0 - ctxt
// a1 - stk
// a2 - stk_sz
// a3 - main_f
// a4 - ud
.globl bmk_context_makecontext
bmk_context_makecontext:
  sw		gp, OFF(GP)(a0)
  sw		zero, OFF(S0)(a0)
  sw		a3, OFF(S1)(a0) // S1 <= main_f
  sw		a4, OFF(S2)(a0) // S2 <= ud
  add		a1, a1, a2 // Compute stack top
  addi		a1, a1, -8
  sw		a1, OFF(SP)(a0)
  la		t0, _bmk_context_init
  sw		t0, OFF(RA)(a0)

  // Return
  ret

/**
 * a0 - src context
 * a1 - dst context
 */
bmk_context_swapcontext:

  // First, save all registers in the src context
  sw		ra, OFF(RA)(a0)
  sw		sp, OFF(SP)(a0)
  sw		gp, OFF(GP)(a0)
  sw		tp, OFF(TP)(a0)
  // Temps are not preserved
  sw		s0, OFF(S0)(a0)
  sw		s1, OFF(S1)(a0)
  // a0, a1 already used here
  sw		a2, OFF(A2)(a0)
  sw		a3, OFF(A3)(a0)
  sw		a4, OFF(A4)(a0)
  sw		a5, OFF(A5)(a0)
  sw		a6, OFF(A6)(a0)
  sw		a7, OFF(A7)(a0)
  sw		s2, OFF(S2)(a0)
  sw		s3, OFF(S3)(a0)
  sw		s4, OFF(S4)(a0)
  sw		s5, OFF(S5)(a0)
  sw		s6, OFF(S6)(a0)
  sw		s7, OFF(S7)(a0)
  sw		s8, OFF(S8)(a0)
  sw		s9, OFF(S9)(a0)
  sw		s10, OFF(S10)(a0)
  sw		s11, OFF(S11)(a0)
  sw		t3, OFF(T3)(a0)
  sw		t4, OFF(T4)(a0)
  sw		t5, OFF(T5)(a0)
  sw		t6, OFF(T6)(a0)

  // Save this point as where we return
//  la		t0, swapcontext_return
//  sw		t0, OFF(PC)(a0)

swapcontext_return:

  // Load up execution context from the
  // destination pointer
  lw		ra, OFF(RA)(a1)
  lw		sp, OFF(SP)(a1)
  lw		gp, OFF(GP)(a1)
  lw		tp, OFF(TP)(a1)
  // Temps are not preserved
  lw		s0, OFF(S0)(a1)
  lw		s1, OFF(S1)(a1)
  // a1, a1 already used here
  lw		a2, OFF(A2)(a1)
  lw		a3, OFF(A3)(a1)
  lw		a4, OFF(A4)(a1)
  lw		a5, OFF(A5)(a1)
  lw		a6, OFF(A6)(a1)
  lw		a7, OFF(A7)(a1)
  lw		s2, OFF(S2)(a1)
  lw		s3, OFF(S3)(a1)
  lw		s4, OFF(S4)(a1)
  lw		s5, OFF(S5)(a1)
  lw		s6, OFF(S6)(a1)
  lw		s7, OFF(S7)(a1)
  lw		s8, OFF(S8)(a1)
  lw		s9, OFF(S9)(a1)
  lw		s10, OFF(S10)(a1)
  lw		s11, OFF(S11)(a1)
  lw		t3, OFF(T3)(a1)
  lw		t4, OFF(T4)(a1)
  lw		t5, OFF(T5)(a1)
  lw		t6, OFF(T6)(a1)

  // Finally, load the PC and jump back
  // to where we came from
  lw		t0, OFF(RA)(a1)
  jr		t0

