
BMK_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

BMK_SRC_C = $(notdir $(wildcard $(BMK_SRC_DIR)/*.c))

SRC_DIRS += $(BMK_SRC_DIR)

else # Rules

libbmk.o : $(BMK_SRC_C:.c=.o)
	$(Q)$(LD) -r -o $@ $^
	

endif