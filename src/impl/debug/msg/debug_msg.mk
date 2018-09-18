
BMK_DEBUG_MSG_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(BMK_DEBUG_MSG_DIR)/../debug.mk

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_DEBUG_MSG_DIR)

else # Rules

endif
