
ARCH=riscv64-unknown-elf-
CC=$(ARCH)gcc
CXX=$(ARCH)g++
LD=$(ARCH)ld
AR=$(ARCH)ar

#LIBGCC=$(shell $(CC) -print-libgcc-file-name)
#LIBC=$(shell $(CC) -print-file-name=libc.a)

include ../../configs/riscv/qemu/virt.mk

SRC_DIRS += ../../src/libfemto/include ../../src/libfemto/std

CFLAGS += $(foreach d,$(SRC_DIRS),-I$(d))
ASFLAGS += $(foreach d,$(SRC_DIRS),-I$(d))

RULES := 1

vpath %.c $(SRC_DIRS)
vpath %.S $(SRC_DIRS)

LIBFEMTO_SRC = $(notdir $(wildcard ../../src/libfemto/std/*.c))

LDFLAGS += -nostartfiles -nostdlib -nostdinc -static \
                     -Wl,--nmagic -Wl,--gc-sections
CFLAGS += -g -fPIC
ASFLAGS += -g
CFLAGS += -mcmodel=medany -ffunction-sections -fdata-sections

CFLAGS += -march=rv64imafd 
ASFLAGS += -march=rv64imafd

BMK_OBJS += libbmk.o #libbmk_sys_riscv.o libbmk_atomics_riscv.o
BMK_OBJS += stub.o 
BMK_OBJS += single_core_inorder_exit_test.o
# BMK_OBJS += dual_core_pingpong_test.o
BMK_OBJS += $(LIBFEMTO_SRC:.c=.o)
#BMK_OBJS += libbmk_scheduler_simple.o libbmk_context_riscv.o

all : $(BMK_OBJS) 
	$(LD) -o test.elf \
		$(BMK_OBJS) \
        -T$(SYS_RISCV_QEMU_VIRT_DIR)/riscv-qemu-virt.lds \
		$(LIBC) $(LIBGCC)

clean :
	rm -f *.o

include ../../configs/riscv/qemu/virt.mk

