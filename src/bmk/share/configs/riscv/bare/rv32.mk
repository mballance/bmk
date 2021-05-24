BMK_CONFIGS_RISCV_BARE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_CONFIGS_RISCV_BARE_DIR)/../../..)

ifneq (1,$(RULES))
BARE_MK_INCLUDES += $(BMK_DIR)/bmk.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/atomics/gcc/atomics_gcc.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/context/riscv/rv32.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/mem/generic/generic.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/sys/riscv/sys_riscv.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/scheduler/simple/scheduler_simple.mk
BARE_MK_INCLUDES += $(BMK_DIR)/impl/debug/msg/debug_msg.mk
#BARE_MK_INCLUDES += $(BMK_DIR)/impl/debug/null/debug_null.mk

include $(BARE_MK_INCLUDES)

else # Rules

include $(BARE_MK_INCLUDES)

endif

