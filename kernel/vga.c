#include "vga.h"

static inline uint16_t
fill_cell(uint8_t c)
{
	return (uint16_t)c | (uint16_t)(VGA_COLOR_WHITE | VGA_COLOR_BLACK) << 8;
}

void
clear(void)
{
	uint16_t *screen = (uint16_t *)VGA_MEMORY_START;
	for (size_t row = 0; row < VGA_SCREEN_HEIGHT; ++row) {
		for (size_t col = 0; col < VGA_SCREEN_WIDTH; ++col) {
			screen[row * VGA_SCREEN_WIDTH + col] = fill_cell(' ');
		}
	}
}

void
print(char *str)
{
	uint16_t *screen = (uint16_t *)VGA_MEMORY_START;
	uint16_t cur = locate_cursor();

	size_t i = 0;
	for (; str[i] != '\0'; ++i) {
		screen[cur + i] = fill_cell(str[i]);
	}

	update_cursor(cur + i);
}

void
bksp(void)
{
	uint16_t *screen = (uint16_t *)VGA_MEMORY_START;
	uint16_t cur = locate_cursor();

	if (cur == 0) {
		return;
	}

	screen[cur - 1] = fill_cell(' ');
	update_cursor(cur - 1);
}

void
enable_cursor(void)
{
	/*
	 * First, select cursor start register 0x0a, then enable the cursor by
	 * sending 0x00 -- 0x01 disables the cursor.
	 */
	outb(CURSOR_COMMAND_PORT, 0x0a);
	outb(CURSOR_DATA_PORT, 0x00);
}

void
update_cursor(uint16_t location)
{
	/*
	 * Select cursor location low register 0x0f, then write least
	 * significant half of `location` to it.
	 */
	outb(CURSOR_COMMAND_PORT, 0x0f);
	outb(CURSOR_DATA_PORT, (uint8_t)(location & 0xff));

	/*
	 * Select cursor location high register 0x0e, then write most
	 * significant half of `location` to it.
	 *
	 * The halves of `location` used to set corresponding cursor location
	 * are the same as those in locate_cursor().
	 */
	outb(CURSOR_COMMAND_PORT, 0x0e);
	outb(CURSOR_DATA_PORT, (uint8_t)((location >> 8) & 0xff));
}

uint16_t
locate_cursor(void)
{
	uint16_t location = 0;

	/*
	 * Select cursor location low register, read its value, and store it in
	 * least significant half of `location`.
	 */
	outb(CURSOR_COMMAND_PORT, 0x0f);
	location |= inb(CURSOR_DATA_PORT);

	/*
	 * Select cursor location high register, read its value, and store it
	 * in most significant half of `location`.
	 */
	outb(CURSOR_COMMAND_PORT, 0x0e);
	location |= ((uint16_t)inb(CURSOR_DATA_PORT)) << 8;

	return location;
}
