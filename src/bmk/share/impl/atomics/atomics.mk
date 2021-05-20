
BMK_ATOMICS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_ATOMICS_DIR)
CFLAGS += -I$(BMK_ATOMICS_DIR)

else # Rules

endif
