
BMK_PTHREAD_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

SRC_DIRS += $(BMK_PTHREAD_DIR)
LIB_TARGETS += libbmk_pthread.o
BMK_PTHREAD_SRC = $(notdir $(wildcard $(BMK_PTHREAD_DIR)/*.c))

else # Rules

libbmk_pthread.o : $(BMK_PTHREAD_SRC:.c=.o)
	$(Q)$(LD) -r -o $@ $(BMK_PTHREAD_SRC:c.=.o)

endif
