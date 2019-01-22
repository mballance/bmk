BMK_CONFIGS_RISCV_QEMU_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_CONFIGS_RISCV_QEMU_DIR)/../../..)

ifneq (1,$(RULES))
MK_INCLUDES += $(BMK_DIR)/src/bmk.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/atomics/gcc/atomics_gcc.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/sys/riscv/qemu-virt/sys_riscv_qemu_virt.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/context/riscv/context_riscv.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/scheduler/simple/scheduler_simple.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/debug/msg/debug_msg.mk

include $(MK_INCLUDES)

else # Rules

include $(MK_INCLUDES)

endif

