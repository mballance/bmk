
BMK_RISCV_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_RISCV_SRC_DIR)/../sys.mk

ifneq (1,$(RULES))

BMK_RISCV_SRC_C = $(notdir $(wildcard $(BMK_RISCV_SRC_DIR)/*.c))
BMK_RISCV_SRC_S = $(notdir $(wildcard $(BMK_RISCV_SRC_DIR)/*.S))

SRC_DIRS += $(BMK_RISCV_SRC_DIR)

LIBBMK_OBJS += $(BMK_RISCV_SRC_C:.c=.o) $(BMK_RISCV_SRC_S:.S=.o)

else # Rules


endif
