
BMK_ATOMICS_GCC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_ATOMICS_GCC_DIR)/../atomics.mk

ifneq (1,$(RULES))


SRC_DIRS += $(BMK_ATOMICS_GCC_DIR)

else # Rules

endif
