
BMK_SCHEDULER_SIMPLE_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_SIMPLE_DIR)
CFLAGS += -I$(BMK_SCHEDULER_SIMPLE_DIR)

BMK_SCHEDULER_SIMPLE_SRC = $(notdir $(wildcard $(BMK_SCHEDULER_SIMPLE_DIR)/*.c))

LIBBMK_OBJS += $(BMK_SCHEDULER_SIMPLE_SRC:.c=.o)

include $(BMK_SCHEDULER_SIMPLE_DIR)/../scheduler.mk

else # Rules


endif
