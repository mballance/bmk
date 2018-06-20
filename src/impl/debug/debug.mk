
BMK_DEBUG_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_DEBUG_DIR)

else # Rules

endif
