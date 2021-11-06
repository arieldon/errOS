#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "vga.h"

void
main(void)
{
	clear();
	init_idt();
	print("Enter kernel land.");
	for (;;) {}
}
