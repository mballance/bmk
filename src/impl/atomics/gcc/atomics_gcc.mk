
BMK_ATOMICS_GCC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_ATOMICS_GCC_DIR)

else # Rules

endif
