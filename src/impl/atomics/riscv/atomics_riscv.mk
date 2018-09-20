
BMK_ATOMICS_RISCV_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_ATOMICS_RISCV_DIR)/../atomics.mk

ifneq (1,$(RULES))

BMK_ATOMICS_RISCV_SRC = $(notdir $(wildcard $(BMK_ATOMICS_RISCV_DIR)/*.S))

LIBBMK_OBJS += $(BMK_ATOMICS_RISCV_SRC:.S=.o)

SRC_DIRS += $(BMK_ATOMICS_RISCV_DIR)

else # Rules

endif
