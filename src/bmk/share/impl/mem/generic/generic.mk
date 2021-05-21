
BMK_MEM_GENERIC_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ifneq (1,$(RULES))
SRC_DIRS += $(BMK_MEM_GENERIC_DIR)

else

endif

