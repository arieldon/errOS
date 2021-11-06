#include "io.h"

uint8_t
inb(uint16_t port)
{
	/* Read a single byte from the specified hardware I/O port. */
	uint8_t ret;
	asm("inb %1, %0"
		: "=a" (ret)
		: "Nd" (port));
	return ret;
}

void
outb(uint16_t port, uint8_t val)
{
	/* Write a designated single byte to the specified hardware I/O port. */
	asm("outb %0, %1"
		:
		: "a" (val), "Nd" (port));
}
