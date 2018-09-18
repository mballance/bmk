
SYS_RISCV_QEMU_VIRT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

include $(SYS_RISCV_QEMU_VIRT_DIR)/../sys_riscv.mk

SRC_DIRS += $(SYS_RISCV_QEMU_VIRT_DIR)

SYS_RISCV_QEMU_VIRT_SRC = $(notdir $(wildcard $(SYS_RISCV_QEMU_VIRT_DIR)/*.c))
LIBBMK_OBJS += $(SYS_RISCV_QEMU_VIRT_SRC:.c=.o)

else # Rules

include $(SYS_RISCV_QEMU_VIRT_DIR)/../sys_riscv.mk

endif

