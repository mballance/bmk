

BMK_SCHEDULER_SIMPLE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_SCHEDULER_SIMPLE_DIR)/../scheduler.mk

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_SIMPLE_DIR)

BMK_SCHEDULER_SIMPLE_SRC = $(notdir $(wildcard $(BMK_SCHEDULER_SIMPLE_DIR)/*.c))

LIBBMK_OBJS += $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)

else # Rules


endif
