#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

#include "io.h"

enum VGA {
	VGA_MEMORY_START = 0xb8000,
	VGA_SCREEN_WIDTH = 80,
	VGA_SCREEN_HEIGHT = 25,
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_WHITE = 15,
};

enum CURSOR {
	CURSOR_COMMAND_PORT = 0x03d4,
	CURSOR_DATA_PORT = 0x03d5,
};

void bksp(void);
void clear(void);
void print(char *str);
void printh(uint32_t hex);

void enable_cursor(void);
void update_cursor(uint16_t location);
uint16_t locate_cursor(void);

#endif
