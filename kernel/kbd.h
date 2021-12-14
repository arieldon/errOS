#ifndef KBD_H
#define KBD_H

#include <stdint.h>

#include "screen.h"

#define isalpha(c)	((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
#define PROMPT_DISPLAY	"> "
#define PROMPT_PRINT	"print "
#define PROMPT_CLEAR	"clear"

enum KBD {
	  KBD_INPUT_PORT = 0x60,
	  KBD_BUFFER_MAX = 64,
};

enum PROMPT {
	PROMPT_LEN = 2,
	PROMPT_PRINT_LEN = 6,
	PROMPT_CLEAR_LEN = 5,
};

enum CMD_TYPE {
	CMD_NONE,
	CMD_CLEAR,
	CMD_PRINT,
};

extern int buflen;
extern char kbd_buffer[KBD_BUFFER_MAX + 1];

void write_kbd_input(uint8_t c);

#endif
