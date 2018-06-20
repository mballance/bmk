
BMK_SYS_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SYS_DIR)

BMK_SYS_SRC = $(notdir $(wildcard $(BMK_SYS_DIR)/*.c))

BMK_DEPS += libbmk_sys.o

else # Rules

libbmk_sys.o : $(BMK_SYS_SRC:.c=.o)
	$(Q)$(LD) -r -o $@ $(BMK_SYS_SRC:.c=.o)


endif
