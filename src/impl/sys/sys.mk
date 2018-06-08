
BMK_SYS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SYS_DIR)

else # Rules


endif
