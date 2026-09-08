.RECIPEPREFIX := >

NASM := nasm
GCC := gcc
LD := ld

BUILD := build
OBJ := $(BUILD)/obj

C_OBJ := $(OBJ)/c
ASM_OBJ := $(OBJ)/asm
BOOT_OBJ := $(OBJ)/boot

STAGE1 := $(BUILD)/stage1.bin
STAGE2 := $(BUILD)/stage2.bin
STAGE2_OBJ := $(BOOT_OBJ)/stage2.o

STAGE2_SECTORS_INC := $(BUILD)/stage2_sectors.inc
DISK := $(BUILD)/NexisOS.img

LINKER := linker.ld

DISK_SIZE := 1474560
SECTOR_SIZE := 512

C_SOURCES := $(shell find kernel -type f -name '*.c')
ASM_SOURCES := $(shell find kernel -type f -name '*.asm')

C_OBJECTS := $(patsubst kernel/%.c,$(C_OBJ)/%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst kernel/%.asm,$(ASM_OBJ)/%.o,$(ASM_SOURCES))

INCLUDE_DIRS := $(shell find kernel -type d -print)
INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS := \
    -m32 \
    -march=i386 \
    -ffreestanding \
    -fno-pie \
    -fno-stack-protector \
    -fno-builtin \
    -nostdlib \
    -nodefaultlibs \
    $(INCLUDES)

NASMFLAGS := -f elf32 $(INCLUDES)


.PHONY: all
all: $(DISK)


$(BUILD):
> mkdir -p $@

$(C_OBJ):
> mkdir -p $@

$(ASM_OBJ):
> mkdir -p $@

$(BOOT_OBJ):
> mkdir -p $@


$(C_OBJ)/%.o: kernel/%.c
> mkdir -p $(dir $@)
> $(GCC) $(CFLAGS) -c $< -o $@


$(ASM_OBJ)/%.o: kernel/%.asm
> mkdir -p $(dir $@)
> $(NASM) $(NASMFLAGS) $< -o $@


$(STAGE2_OBJ): boot/stage2.S | $(BOOT_OBJ)
> $(NASM) $(NASMFLAGS) $< -o $@


$(STAGE2): $(STAGE2_OBJ) $(C_OBJECTS) $(ASM_OBJECTS) $(LINKER) | $(BUILD)
> $(LD) \
>     -m elf_i386 \
>     -T $(LINKER) \
>     --oformat binary \
>     -o $@ \
>     $(STAGE2_OBJ) \
>     $(C_OBJECTS) \
>     $(ASM_OBJECTS)
> @echo "[+] Stage2: $$(stat -c%s $@) bytes"


$(STAGE2_SECTORS_INC): $(STAGE2) | $(BUILD)
> size=$$(stat -c%s $(STAGE2)); \
> sectors=$$(( (size + $(SECTOR_SIZE) - 1) / $(SECTOR_SIZE) )); \
> if [ $$sectors -gt 17 ]; then \
>     echo "ERROR: Stage2 use $$sectors sectors"; \
>     echo "ERROR: Stage1 CHS 1 track."; \
>     exit 1; \
> fi; \
> printf '%%define STAGE2_SECTORS %s\n' $$sectors > $@; \
> echo "[+] Stage2 sectors: $$sectors"


$(STAGE1): boot/stage1.S $(STAGE2_SECTORS_INC) | $(BUILD)
> $(NASM) -f bin -I$(BUILD)/ $< -o $@
> @size=$$(stat -c%s $@); \
> if [ $$size -ne 512 ]; then \
>     echo "ERROR: Stage1 have $$size bytes."; \
>     echo "ERROR: Stage1 not have 512 bytes."; \
>     exit 1; \
> fi
> @signature=$$(od -An -tx1 -j510 -N2 $@ | tr -d ' \n'); \
> if [ "$$signature" != "55aa" ]; then \
>     echo "ERROR: Stage1  assign 55 AA."; \
>     exit 1; \
> fi
> @echo "[+] Stage1: 512 bytes"
> @echo "[+] Boot signature: 55 AA"


$(DISK): $(STAGE1) $(STAGE2) | $(BUILD)
> cat $(STAGE1) $(STAGE2) > $@
> truncate -s $(DISK_SIZE) $@
> @echo "[+] Disk image: $@"
> @echo "[+] Size: $$(stat -c%s $@) bytes"


.PHONY: check
check: $(DISK)
> echo
> echo "NexisOS boot image "
> echo
> echo "Stage1:"
> stat -c "  %n = %s bytes" $(STAGE1)
> echo
> echo "Stage2:"
> stat -c "  %n = %s bytes" $(STAGE2)
> echo
> echo "Stage2 sectors:"
> cat $(STAGE2_SECTORS_INC)
> echo
> echo "Disk:"
> stat -c "  %n = %s bytes" $(DISK)
> echo
> echo "Sector 0:"
> od -An -tx1 -N512 $(DISK) | tail -n 2
> echo
> echo "Boot signature:"
> od -An -tx1 -j510 -N2 $(DISK)
> echo


.PHONY: run
run: $(DISK)
> qemu-system-i386 \
>     -drive format=raw,file=$(DISK) \
>     -serial stdio \
>     -net none


.PHONY: clean
clean:
> rm -rf $(BUILD)