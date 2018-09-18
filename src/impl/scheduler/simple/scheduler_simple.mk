

BMK_SCHEDULER_SIMPLE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_SCHEDULER_SIMPLE_DIR)/../scheduler.mk

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_SIMPLE_DIR)

BMK_SCHEDULER_SIMPLE_SRC = $(notdir $(wildcard $(BMK_SCHEDULER_SIMPLE_DIR)/*.c))

BMK_DEPS += libbmk_scheduler_simple.o

LIBBMK_OBJS += $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)

else # Rules

libbmk_scheduler_simple.o : $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)
	$(Q)$(LD) -r -o $@ $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)

endif
