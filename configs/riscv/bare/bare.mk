BMK_CONFIGS_RISCV_BARE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_CONFIGS_RISCV_BARE_DIR)/../../..)

ifneq (1,$(RULES))
BARE_MK_INCLUDES += $(BMK_DIR)/src/bmk.mk
# BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/atomics/riscv/atomics_riscv.mk
BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/atomics/gcc/atomics_gcc.mk
BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/sys/riscv/sys_riscv.mk
BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/context/riscv/context_riscv.mk
BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/scheduler/simple/scheduler_simple.mk
# BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/debug/msg/debug_msg.mk
BARE_MK_INCLUDES += $(BMK_DIR)/src/impl/debug/null/debug_null.mk

include $(BARE_MK_INCLUDES)

else # Rules

include $(BARE_MK_INCLUDES)

endif

