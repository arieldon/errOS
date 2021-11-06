#ifndef KBD_H
#define KBD_H

#include <stdint.h>

#include "vga.h"

enum KBD {
	  KBD_INPUT_PORT = 0x60,
};

void write_kbd_input(uint8_t c);

#endif
