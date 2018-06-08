
UCONTEXT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

ifneq (1,$(RULES))

UCONTEXT_SRC := $(notdir $(wildcard $(UCONTEXT_DIR)/*.c))

SRC_DIRS += $(UCONTEXT_DIR)

else # Rules

libbmk_ucontext.o : $(UCONTEXT_SRC:.c=.o)
	$(Q)ld -r -o $@ $(UCONTEXT_SRC:.c=.o)

endif
