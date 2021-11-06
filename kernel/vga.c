#include "vga.h"

int bksp = 0;

void
clear(void)
{
	uint16_t *vgamem = (uint16_t *)VGA_MEMORY_START;
	for (size_t row = 0; row < VGA_SCREEN_HEIGHT; ++row) {
		for (size_t col = 0; col < VGA_SCREEN_WIDTH; ++col) {
			vgamem[row * VGA_SCREEN_WIDTH + col] = ' ' |
				(uint16_t) VGA_COLOR_BLACK << 8;
		}
	}
}

void
print(char *str)
{
	uint16_t *vgamem = (uint16_t *)VGA_MEMORY_START;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		vgamem[i] = str[i] | (uint16_t) VGA_COLOR_WHITE << 8;
	}
}

void
print_char(char c)
{
	static uint16_t *vgaloc = (uint16_t *)VGA_MEMORY_START;

	if (vgaloc == (uint16_t *)VGA_MEMORY_START) {
		clear();
	}

	if (bksp && (vgaloc > (uint16_t *)VGA_MEMORY_START)) {
		--vgaloc;
		*vgaloc = c | (uint16_t) VGA_COLOR_WHITE << 8;
	} else {
		*vgaloc = c | (uint16_t) VGA_COLOR_WHITE << 8;
		++vgaloc;
	}
}
