
BMK_SRC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_SRC_DIR)/..)

LIBFEMTO_DIR = $(BMK_SRC_DIR)/libfemto

ifneq (1,$(RULES))

BMK_SRC_C = $(filter-out bmk_stubs.c,$(notdir $(wildcard $(BMK_SRC_DIR)/*.c)))
LIBFEMTO_SRC_C = $(notdir $(wildcard $(LIBFEMTO_DIR)/std/*.c))

SRC_DIRS += $(BMK_SRC_DIR) $(LIBFEMTO_DIR)/std
CFLAGS += -I$(LIBFEMTO_DIR)/include
CXXFLAGS += -I$(LIBFEMTO_DIR)/include

LIBBMK_OBJS += $(BMK_SRC_C:.c=.o) $(LIBFEMTO_SRC_C:.c=.o)

BMK_DEPS += libbmk.o

else # Rules

libbmk.o : $(LIBBMK_OBJS)
	$(Q)$(LD) -r -o $@ $(LIBBMK_OBJS)

libbmk_stubs.o : bmk_stubs.o	
	$(Q)$(LD) -r -o $@ $^

endif
