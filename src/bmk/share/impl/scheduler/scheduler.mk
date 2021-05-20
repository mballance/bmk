

BMK_SCHEDULER_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_DIR)
CFLAGS += -I$(BMK_SCHEDULER_DIR)

else # Rules

endif
