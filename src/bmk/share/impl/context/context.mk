
CONTEXT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

ifneq (1,$(RULES))

SRC_DIRS += $(CONTEXT_DIR)
CFLAGS += -I$(CONTEXT_DIR)

else # Rules


endif
