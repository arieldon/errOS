#ifndef PROMPT_H
#define PROMPT_H

#include <stdint.h>

#include "screen.h"

#define CMD_PROMPT	"> "
#define CMD_PRINT	"print "
#define CMD_CLEAR	"clear"

enum CMD_TYPE {
	CMD_TYPE_NONE = 1,
	CMD_TYPE_CLEAR = 2,
	CMD_TYPE_PRINT = 4,
};

enum CMD_LEN {
	CMD_LEN_CLEAR = 5,
	CMD_LEN_PRINT = 6,
	CMD_LEN_PROMPT = 2,
};

enum CMD_TYPE parse_cmd(char *inputbuf, int buflen);
void exec_cmd(enum CMD_TYPE cmd, char *input_buffer, int buflen);

#endif
