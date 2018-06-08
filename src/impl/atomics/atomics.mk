
BMK_ATOMICS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_ATOMICS_DIR)

else # Rules

endif
