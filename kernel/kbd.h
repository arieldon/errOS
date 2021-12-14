#ifndef KBD_H
#define KBD_H

#include <stdint.h>

#include "screen.h"

#define isalpha(c)	((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
#define PROMPT_DISPLAY	"> "
#define PROMPT_REPEAT	"repeat "

enum KBD {
	  KBD_INPUT_PORT = 0x60,
	  KBD_BUFFER_MAX = 64,
};

enum PROMPT {
	PROMPT_LENGTH = 2,
	PROMPT_REPEAT_LENGTH = 7,
};

extern int buflen;
extern char kbd_buffer[KBD_BUFFER_MAX + 1];

void write_kbd_input(uint8_t c);

#endif
