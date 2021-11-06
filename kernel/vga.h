#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

enum VGA {
	VGA_MEMORY_START = 0xb8000,
	VGA_SCREEN_WIDTH = 80,
	VGA_SCREEN_HEIGHT = 25,
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_WHITE = 15,
};

extern int bksp;

void clear(void);
void print(char *str);
void print_char(char c);

#endif
