
BMK_CONTEXT_RISCV_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_CONTEXT_RISCV_DIR)/../context.mk

ifneq (1,$(RULES))

BMK_CONTEXT_RISCV_SRC_C = $(notdir $(wildcard $(BMK_CONTEXT_RISCV_DIR)/*.c))
BMK_CONTEXT_RISCV_SRC_S = $(notdir $(wildcard $(BMK_CONTEXT_RISCV_DIR)/*.S))

SRC_DIRS += $(BMK_CONTEXT_RISCV_DIR)

LIBBMK_OBJS += $(BMK_CONTEXT_RISCV_SRC_C:.c=.o)
LIBBMK_OBJS += $(BMK_CONTEXT_RISCV_SRC_S:.S=.o)

BMK_DEPS += libbmk_context_riscv.o

else # Rules

libbmk_context_riscv.o : $(BMK_CONTEXT_RISCV_SRC:.c=.o)
	$(Q)$(LD) -r -o $@ $(BMK_CONTEXT_RISCV_SRC:.c=.o)

endif
