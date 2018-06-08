
ifneq (true,$(BUILD))
UNIT_TEST_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
UNIT_TEST_DIR := $(shell cd $(UNIT_TEST_DIR) ; pwd)

ifneq (,$(J))
NUM_CORES := $(J)
else
NUM_CORES:=$(shell cat /proc/cpuinfo | grep 'processor.*:' | wc -l)
endif
endif

BMK_DIR := $(shell cd $(UNIT_TEST_DIR)/../.. ; pwd)
GOOGLETEST_VERSION=1.8.0
GOOGLETEST_ZIP=googletest-release-$(GOOGLETEST_VERSION).tar.gz
GOOGLETEST_URL=https://github.com/google/googletest/archive/release-$(GOOGLETEST_VERSION).tar.gz
PACKAGES_DIR=$(BMK_DIR)/packages

UNIT_TEST_SRC = $(wildcard $(UNIT_TEST_DIR)/src/*.cpp)
UNIT_TEST_SRC_NOTDIR = $(notdir $(wildcard $(UNIT_TEST_DIR)/src/*.cpp))

SRC_DIRS += $(UNIT_TEST_DIR)/src
SRC_DIRS += $(BUILD_DIR)/googletest-release-$(GOOGLETEST_VERSION)/googletest
SRC_DIRS += $(BUILD_DIR)/googletest-release-$(GOOGLETEST_VERSION)/googletest/src
SRC_DIRS += $(BUILD_DIR)/googletest-release-$(GOOGLETEST_VERSION)/googletest/include

BUILD_DIR := $(UNIT_TEST_DIR)/build

MK_INCLUDES += $(BMK_DIR)/src/bmk.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/atomics/atomics.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/atomics/gcc/atomics_gcc.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/scheduler/scheduler.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/scheduler/simple/scheduler_simple.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/sys/sys.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/sys/pthread/sys_pthread.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/context/context.mk
MK_INCLUDES += $(BMK_DIR)/src/impl/context/ucontext/ucontext.mk

CXXFLAGS += $(foreach dir,$(SRC_DIRS),-I$(dir))
CFLAGS += $(foreach dir,$(SRC_DIRS),-I$(dir))

CFLAGS += -g
CXXFLAGS += -g

EXE_TARGETS += unittest$(EXEEXT)

UNIT_TEST_OBJ = $(UNIT_TEST_SRC_NOTDIR:.cpp=.o) gtest-all.o gtest_main.o

include $(MK_INCLUDES)

RULES := 1

vpath %.c $(SRC_DIRS)
vpath %.cpp $(SRC_DIRS)
vpath %.cc $(SRC_DIRS)


all : do_build_run

debug : $(BUILD_DIR)/googletest.unpack
	if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi
	$(MAKE) -j$(NUM_CORES) BUILD=true UNIT_TEST_DIR=$(UNIT_TEST_DIR) \
		-C $(BUILD_DIR) -f $(UNIT_TEST_DIR)/Makefile __build
	cd $(BUILD_DIR) ; gdb --args ./unittest$(EXEEXT) $(GTEST_ARGS)

clean :
	rm -rf build


do_build_run : $(BUILD_DIR)/googletest.unpack
	if test ! -d $(BUILD_DIR); then mkdir -p $(BUILD_DIR); fi
	$(MAKE) -j$(NUM_CORES) BUILD=true UNIT_TEST_DIR=$(UNIT_TEST_DIR) \
		-C $(BUILD_DIR) -f $(UNIT_TEST_DIR)/Makefile _run

__build : $(LIB_TARGETS) $(EXE_TARGETS)

_run : __build
	./unittest$(EXEEXT)

$(BUILD_DIR)/googletest.unpack : $(PACKAGES_DIR)/$(GOOGLETEST_ZIP)
	if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	cd $(BUILD_DIR) ; tar xvzf $^
	touch $@

$(PACKAGES_DIR)/$(GOOGLETEST_ZIP) :
	if test ! -d `dirname $@`; then mkdir -p `dirname $@`; fi
	cd $(PACKAGES_DIR) ; wget -O $(GOOGLETEST_ZIP) $(GOOGLETEST_URL)


unittest$(EXEEXT) : \
	$(UNIT_TEST_OBJ) \
	libbmk.o \
	libbmk_ucontext.o \
	libbmk_sys_pthread.o \
	libbmk_scheduler_simple.o
	$(CXX) -o $@ $^ -lpthread

include $(MK_INCLUDES)

