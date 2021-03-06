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
 * bmk_impl_atomics_riscv.S
 *
 *  Created on: Jun 21, 2018
 *      Author: ballance
 ****************************************************************************/

// a0 - handle to initialize
.globl bmk_atomics_init
bmk_atomics_init:
	sw		zero, (a0)
	jr		ra

.globl bmk_atomics_lock
bmk_atomics_lock:
1:
	li		t1, 1
	lr.w	t0, (a0)
	// If the value is non-zero, try again
	bne		t0, zero, 1b
	sc.w	t2, t1, (a0)
	// If the store conditional fails, try again
	bne		t2, zero, 1b
	jr		ra


.globl bmk_atomics_unlock
bmk_atomics_unlock:
1:
	lr.w	t0, (a0)
	sc.w	t1, zero, (a0)
	bne		t1, zero, 1b
	jr		ra

// a0 - pointer
// a1 - new value
.globl bmk_atomics_swap
bmk_atomics_swap:

	amoswap.w.aq a1, a1, (a0)
	add			a0, a1, zero
	ret
