
BMK_ATOMICS_GCC_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))


ifneq (1,$(RULES))


SRC_DIRS += $(BMK_ATOMICS_GCC_DIR)
CFLAGS += -I$(BMK_ATOMICS_GCC_DIR)

include $(BMK_ATOMICS_GCC_DIR)/../atomics.mk

else # Rules

endif
