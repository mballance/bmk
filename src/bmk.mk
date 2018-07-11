
BMK_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_SRC_DIR)/..)

ifneq (1,$(RULES))

BMK_SRC_C = $(filter-out bmk_stubs.c,$(notdir $(wildcard $(BMK_SRC_DIR)/*.c)))

SRC_DIRS += $(BMK_SRC_DIR)

BMK_DEPS += libbmk.o

else # Rules

libbmk.o : $(BMK_SRC_C:.c=.o)
	$(Q)$(LD) -r -o $@ $^

libbmk_stubs.o : bmk_stubs.o	
	$(Q)$(LD) -r -o $@ $^

endif
