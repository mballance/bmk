
CONTEXT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(CONTEXT_DIR)

else # Rules


endif
