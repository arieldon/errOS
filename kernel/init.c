#include <stddef.h>
#include <stdint.h>

#include "idt.h"
#include "kbd.h"
#include "screen.h"

void
main(void)
{
	clear();
	enable_cursor();
	update_cursor(0);
	init_idt();
	print(PROMPT_DISPLAY);
	for (;;) {}
}
