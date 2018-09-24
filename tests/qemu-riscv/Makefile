
ARCH=riscv64-unknown-elf-
CC=$(ARCH)gcc
CXX=$(ARCH)g++
LD=$(ARCH)ld
AR=$(ARCH)ar

include ../../configs/riscv/qemu/virt.mk

CFLAGS += $(foreach d,$(SRC_DIRS),-I$(d))
ASFLAGS += $(foreach d,$(SRC_DIRS),-I$(d))

RULES := 1

vpath %.c $(SRC_DIRS)
vpath %.S $(SRC_DIRS)

LDFLAGS += -nostartfiles -nostdlib -nostdinc -static \
                     -Wl,--nmagic -Wl,--gc-sections
CFLAGS += -g
ASFLAGS += -g
CFLAGS += -mcmodel=medany -ffunction-sections -fdata-sections

BMK_OBJS += libbmk.o #libbmk_sys_riscv.o libbmk_atomics_riscv.o
BMK_OBJS += stub.o single_core_pingpong_test.o
#BMK_OBJS += libbmk_scheduler_simple.o libbmk_context_riscv.o

all : $(BMK_OBJS) 
	$(LD) -o test.elf \
		$(BMK_OBJS) \
        -T$(SYS_RISCV_QEMU_VIRT_DIR)/riscv-qemu-virt.lds

clean :
	rm -f *.o

include ../../configs/riscv/qemu/virt.mk

