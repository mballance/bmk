
BMK_RISCV_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

BMK_RISCV_SRC_C = $(notdir $(wildcard $(BMK_RISCV_SRC_DIR)/*.c))
BMK_RISCV_SRC_S = $(notdir $(wildcard $(BMK_RISCV_SRC_DIR)/*.S))

SRC_DIRS += $(BMK_RISCV_SRC_DIR)

else # Rules

libbmk_riscv.o : $(BMK_RISCV_SRC_C:.c=.o) $(BMK_RISCV_SRC_S:.S=.o)
	$(Q)$(LD) -r -o $@ $^

endif
