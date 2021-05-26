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
 * bmk_impl_mem_generic.h
 ****************************************************************************/
#ifndef INCLUDED_BMK_IMPL_MEM_GENERIC_H
#define INCLUDED_BMK_IMPL_MEM_GENERIC_H
#include <stdint.h>

static inline void bmk_iowrite8(uint8_t data, intptr_t addr) {
	*((volatile uint8_t *)addr) = data;
}

static inline void bmk_iowrite16(uint16_t data, intptr_t addr) {
	*((volatile uint16_t *)addr) = data;
}

static inline void bmk_iowrite32(uint32_t data, intptr_t addr) {
	*((volatile uint32_t *)addr) = data;
}

static inline void bmk_iowrite64(uint64_t data, intptr_t addr) {
	*((volatile uint64_t *)addr) = data;
}

static inline uint8_t bmk_ioread8(intptr_t addr) {
	return *((volatile uint8_t *)addr);
}

static inline uint16_t bmk_ioread16(intptr_t addr) {
	return *((volatile uint16_t *)addr);
}

static inline uint32_t bmk_ioread32(intptr_t addr) {
	return *((volatile uint32_t *)addr);
}

#endif /* INCLUDED_BMK_IMPL_MEM_GENERIC_H */
