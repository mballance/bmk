

BMK_SCHEDULER_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SCHEDULER_DIR)

else # Rules

endif
