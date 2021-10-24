#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH	80
#define VGA_HEIGHT	25

#define BLACK	 0
#define WHITE	15

uint8_t
inb(uint16_t port)
{
	/* Read a single byte from the specified hardware I/O port. */
	uint8_t ret;
	asm volatile("inb %1, %0"
		: "=a"(ret)
		: "Nd"(port));
	return ret;
}

void
outb(uint16_t port, uint8_t val)
{
	/* Write a designated single byte to the specified hardware I/O port. */
	asm volatile("outb %0, %1"
		:
		: "a"(val), "Nd"(port));
}

void
clear(void)
{
	uint16_t *vgamem = (uint16_t *)0xb8000;
	for (size_t row = 0; row < VGA_HEIGHT; ++row) {
		for (size_t col = 0; col < VGA_WIDTH; ++col) {
			vgamem[row * VGA_WIDTH + col] = ' ' | (uint16_t) BLACK << 8;
		}
	}
}

void
print(char *str)
{
	uint16_t *vgamem = (uint16_t *)0xb8000;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		vgamem[i] = str[i] | (uint16_t) WHITE << 8;
	}
}

void
main(void)
{
	clear();
	print("Enter kernel land.");
	for (;;) {}
}
