# ==============================
# Bee Kernel Makefile
# ==============================

# Tools
CC      = gcc
AS      = nasm
LD      = ld

# Flags
CFLAGS  = -m32 -ffreestanding -Wall -Wextra -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

# Files
C_SRC = kernel.c idt.c isr.c keyboard.c panic.c pit.c shell.c vga.c pmm.c paging.c
ASM_SRC = boot.S idt_load.S interrupts.S exceptions.S multiboot_header.S paging_asm.S

OBJ = $(C_SRC:.c=.o) $(ASM_SRC:.S=.o)

# Output
KERNEL = kernel.bin
ISO    = bee.iso

# ==============================
# Default Target
# ==============================

all: $(ISO)

# ==============================
# Compile C
# ==============================

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# ==============================
# Assemble ASM
# ==============================

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

# ==============================
# Link Kernel
# ==============================

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $(KERNEL) $(OBJ)

# ==============================
# Build ISO
# ==============================

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Bee Kernel" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

# ==============================
# Run Kernel
# ==============================

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# ==============================
# Clean Build
# ==============================

clean:
	rm -f *.o
	rm -f $(KERNEL)
	rm -f $(ISO)
	rm -rf iso

# ==============================
# Rebuild
# ==============================

re: clean all
