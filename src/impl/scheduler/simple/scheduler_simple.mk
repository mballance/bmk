

BMK_SCHEDULER_SIMPLE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_SIMPLE_DIR)

BMK_SCHEDULER_SIMPLE_SRC = $(notdir $(wildcard $(BMK_SCHEDULER_SIMPLE_DIR)/*.c))

else # Rules

libbmk_scheduler_simple.o : $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)
	$(Q)ld -r -o $@ $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)

endif
