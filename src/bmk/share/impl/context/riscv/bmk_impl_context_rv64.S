/****************************************************************************
 * Copyright 2018-2021 Matthew Ballance
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#include "bmk_impl_context_rv64_regs.h"

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
  sd		gp, OFF(GP)(a0)
  sd		zero, OFF(S0)(a0)
  sd		a3, OFF(S1)(a0) // S1 <= main_f
  sd		a4, OFF(S2)(a0) // S2 <= ud
  add		a1, a1, a2 // Compute stack top
  addi		a1, a1, -8
  sd		a1, OFF(SP)(a0)
  la		t0, _bmk_context_init
  sd		t0, OFF(RA)(a0)

  // Return
  ret

/**
 * a0 - src context
 * a1 - dst context
 */
bmk_context_swapcontext:

  // First, save all registers in the src context
  sd		ra, OFF(RA)(a0)
  sd		sp, OFF(SP)(a0)
  sd		gp, OFF(GP)(a0)
  sd		tp, OFF(TP)(a0)
  // Temps are not preserved
  sd		s0, OFF(S0)(a0)
  sd		s1, OFF(S1)(a0)
  // a0, a1 already used here
  sd		a2, OFF(A2)(a0)
  sd		a3, OFF(A3)(a0)
  sd		a4, OFF(A4)(a0)
  sd		a5, OFF(A5)(a0)
  sd		a6, OFF(A6)(a0)
  sd		a7, OFF(A7)(a0)
  sd		s2, OFF(S2)(a0)
  sd		s3, OFF(S3)(a0)
  sd		s4, OFF(S4)(a0)
  sd		s5, OFF(S5)(a0)
  sd		s6, OFF(S6)(a0)
  sd		s7, OFF(S7)(a0)
  sd		s8, OFF(S8)(a0)
  sd		s9, OFF(S9)(a0)
  sd		s10, OFF(S10)(a0)
  sd		s11, OFF(S11)(a0)
  sd		t3, OFF(T3)(a0)
  sd		t4, OFF(T4)(a0)
  sd		t5, OFF(T5)(a0)
  sd		t6, OFF(T6)(a0)

  // Save this point as where we return
//  la		t0, swapcontext_return
//  sd		t0, OFF(PC)(a0)

swapcontext_return:

  // Load up execution context from the
  // destination pointer
  ld		ra, OFF(RA)(a1)
  ld		sp, OFF(SP)(a1)
  ld		gp, OFF(GP)(a1)
  ld		tp, OFF(TP)(a1)
  // Temps are not preserved
  ld		s0, OFF(S0)(a1)
  ld		s1, OFF(S1)(a1)
  // a1, a1 already used here
  ld		a2, OFF(A2)(a1)
  ld		a3, OFF(A3)(a1)
  ld		a4, OFF(A4)(a1)
  ld		a5, OFF(A5)(a1)
  ld		a6, OFF(A6)(a1)
  ld		a7, OFF(A7)(a1)
  ld		s2, OFF(S2)(a1)
  ld		s3, OFF(S3)(a1)
  ld		s4, OFF(S4)(a1)
  ld		s5, OFF(S5)(a1)
  ld		s6, OFF(S6)(a1)
  ld		s7, OFF(S7)(a1)
  ld		s8, OFF(S8)(a1)
  ld		s9, OFF(S9)(a1)
  ld		s10, OFF(S10)(a1)
  ld		s11, OFF(S11)(a1)
  ld		t3, OFF(T3)(a1)
  ld		t4, OFF(T4)(a1)
  ld		t5, OFF(T5)(a1)
  ld		t6, OFF(T6)(a1)

  // Finally, load the PC and jump back
  // to where we came from
  ld		t0, OFF(RA)(a1)
  jr		t0

