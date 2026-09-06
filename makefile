NASM := nasm
GCC := gcc
LD := ld
QEMU := qemu-system-i386
GENISOIMAGE := genisoimage

.RECIPEPREFIX := >

BUILD := build
OBJ := $(BUILD)/obj
C_OBJ := $(OBJ)/c
ASM_OBJ := $(OBJ)/asm
BOOT_OBJ := $(OBJ)/boot
ISO_ROOT := $(BUILD)/iso

STAGE1 := $(BUILD)/stage1.bin
STAGE2 := $(BUILD)/stage2.bin
DISK := $(BUILD)/disk.img
BOOT_IMAGE := $(ISO_ROOT)/boot.img
ISO := $(BUILD)/NexisK.iso
QEMU_LOG := $(BUILD)/qemu.log

C_SOURCES := $(shell find kernel -type f -name '*.c')
ASM_SOURCES := $(shell find kernel -type f -name '*.asm')

C_OBJECTS := $(patsubst kernel/%.c,$(C_OBJ)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst kernel/%.asm,$(ASM_OBJ)/%.o,$(ASM_SOURCES))

STAGE2_SOURCE := boot/stage2.S
STAGE2_OBJECT := $(BOOT_OBJ)/stage2.o

INCLUDE_DIRS := $(shell find kernel -type d -print)
INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS := -m32 -march=i386 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -nostdlib -nodefaultlibs $(INCLUDES)

NASMFLAGS := -f elf32 $(INCLUDES)

all: $(ISO)

$(BUILD):

> mkdir -p $(BUILD)

$(C_OBJ):

> mkdir -p $(C_OBJ)

$(ASM_OBJ):

> mkdir -p $(ASM_OBJ)

$(BOOT_OBJ):

> mkdir -p $(BOOT_OBJ)

$(ISO_ROOT):

> mkdir -p $(ISO_ROOT)

$(STAGE1): boot/stage1.S | $(BUILD)

> $(NASM) -f bin $< -o $@

$(STAGE2_OBJECT): $(STAGE2_SOURCE) | $(BOOT_OBJ)

> $(NASM) $(NASMFLAGS) $< -o $@

$(C_OBJ)/%.o: kernel/%.c

> mkdir -p $(dir $@)
> $(GCC) $(CFLAGS) -c $< -o $@

$(ASM_OBJ)/%.o: kernel/%.asm

> mkdir -p $(dir $@)
> $(NASM) $(NASMFLAGS) $< -o $@

$(STAGE2): $(STAGE2_OBJECT) $(C_OBJECTS) $(ASM_OBJECTS)

> $(LD) -m elf_i386 -T linker.ld --oformat binary $^ -o $@

$(DISK): $(STAGE1) $(STAGE2)

> mkdir -p $(dir $@)
> cat $(STAGE1) $(STAGE2) > $@
> truncate -s %512 $@
> truncate -s 1474560 $@

$(BOOT_IMAGE): $(DISK) | $(ISO_ROOT)

> cp $(DISK) $(BOOT_IMAGE)

$(ISO): $(BOOT_IMAGE)

> $(GENISOIMAGE) -R -J -V NEXISK -b boot.img -o $@ $(ISO_ROOT)

iso: $(ISO)

run: $(ISO)

> $(QEMU) -drive format=raw,media=cdrom,file=$(ISO) -serial stdio -net none

dev: $(ISO)

> rm -f $(QEMU_LOG)
> $(QEMU) -drive format=raw,media=cdrom,file=$(ISO) -serial stdio -D $(QEMU_LOG) -d guest_errors,unimp,int,cpu_reset,pcall,mmu -no-reboot -no-shutdown -net none

clean:

> rm -rf $(BUILD)

.PHONY: all iso run dev clean
