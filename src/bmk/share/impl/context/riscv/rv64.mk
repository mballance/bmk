
BMK_CONTEXT_RISCV_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


ifneq (1,$(RULES))

BMK_CONTEXT_RISCV_SRC_C = $(notdir $(wildcard $(BMK_CONTEXT_RISCV_DIR)/*.c))
BMK_CONTEXT_RISCV_SRC_S = $(notdir $(wildcard $(BMK_CONTEXT_RISCV_DIR)/*.S))

SRC_DIRS += $(BMK_CONTEXT_RISCV_DIR)
CFLAGS += -I$(BMK_CONTEXT_RISCV_DIR)

LIBBMK_OBJS += $(BMK_CONTEXT_RISCV_SRC_C:.c=.o)
LIBBMK_OBJS += $(BMK_CONTEXT_RISCV_SRC_S:.S=.o)

include $(BMK_CONTEXT_RISCV_DIR)/../context.mk

else # Rules

endif
