PATH := ./toolchain/bin/:$(PATH)
TARGET := x86_64-elf
CC := $(TARGET)-gcc
LD := $(TARGET)-ld
WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -std=gnu99 -ffreestanding $(WARNINGS)
BUILDDIR := ./build


boot: $(BUILDDIR)/boot.bin
	qemu-system-x86_64 \
		-accel tcg,thread=single \
		-m 128 \
		-no-reboot \
		-drive format=raw,file=$<,index=0,if=floppy \
		-serial stdio \
		-smp 1 \
		-usb \
		-vga std

$(BUILDDIR)/boot.bin: boot/boot.asm $(BUILDDIR)
	nasm -f bin -o $@ $<

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	rm -rf build/*

.PHONY: clean
