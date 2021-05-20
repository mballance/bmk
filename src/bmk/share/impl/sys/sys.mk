
BMK_SYS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SYS_DIR)

BMK_SYS_SRC = $(notdir $(wildcard $(BMK_SYS_DIR)/*.c))

LIBBMK_OBJS += $(BMK_SYS_SRC:.c=.o)

else # Rules


endif
