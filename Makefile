PATH := ./toolchain/bin/:$(PATH)
TARGET := i686-elf
CC := $(TARGET)-gcc
LD := $(TARGET)-ld
ASM := nasm
CFLAGS := -std=gnu99 -ffreestanding -Wall -Wextra
BUILDDIR := ./build


boot: $(BUILDDIR)/os.bin
	qemu-system-i386 \
		-accel tcg,thread=single \
		-m 128 \
		-no-reboot \
		-drive format=raw,file=$<,index=0,if=floppy \
		-serial stdio \
		-smp 1 \
		-usb \
		-vga std

$(BUILDDIR)/os.bin: $(BUILDDIR)/boot.bin $(BUILDDIR)/kernel.bin
	cat $^ > $@

$(BUILDDIR)/kernel.bin: $(BUILDDIR)/enter_kernel.o $(BUILDDIR)/init.o
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

$(BUILDDIR)/init.o: kernel/init.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/enter_kernel.o: boot/enter_kernel.asm
	$(ASM) -f elf $< -o $@

$(BUILDDIR)/boot.bin: boot/boot.asm $(BUILDDIR)
	$(ASM) -f bin $< -o $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	rm -rf build/*

.PHONY: clean
