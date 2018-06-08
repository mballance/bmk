
BMK_SYS_PTHREAD_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_SYS_PTHREAD_DIR)
LIB_TARGETS += libbmk_sys_pthread.o
BMK_SYS_PTHREAD_SRC = $(notdir $(wildcard $(BMK_SYS_PTHREAD_DIR)/*.c))

else # Rules

libbmk_sys_pthread.o : $(BMK_SYS_PTHREAD_SRC:.c=.o)
	$(Q)$(LD) -r -o $@ $(BMK_SYS_PTHREAD_SRC:.c=.o)

endif
