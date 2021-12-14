#include "prompt.h"

enum CMD_TYPE
parse_cmd(char *input_buffer, int buflen)
{
	enum CMD_TYPE cmd = CMD_TYPE_CLEAR;

	char *cmdcmp = CMD_CLEAR;
	if (buflen >= CMD_LEN_CLEAR) {
		for (uint8_t i = 0; i < CMD_LEN_CLEAR; ++i) {
			if (input_buffer[i] != cmdcmp[i]) {
				cmd = CMD_TYPE_NONE;
				break;
			}
		}
	} else {
		cmd = CMD_TYPE_NONE;
	}

	if (cmd == CMD_TYPE_NONE && buflen >= CMD_LEN_PRINT) {
		cmd = CMD_TYPE_PRINT;
		cmdcmp = CMD_PRINT;
		for (uint8_t i = 0; i < CMD_LEN_PRINT; ++i) {
			if (input_buffer[i] != cmdcmp[i]) {
				cmd = CMD_TYPE_NONE;
				break;
			}
		}
	}

	return cmd;
}

void
exec_cmd(enum CMD_TYPE cmd, char *input_buffer, int buflen)
{
	if (cmd == CMD_TYPE_CLEAR) {
		clear();
		update_cursor(0);
	} else if (cmd == CMD_TYPE_PRINT) {
		update_cursor(
			locate_cursor() + VGA_SCREEN_WIDTH - buflen
				- CMD_LEN_PROMPT
		);
		print(input_buffer + CMD_LEN_PRINT);
	}

	if (cmd == CMD_TYPE_PRINT || cmd == CMD_TYPE_NONE) {
		uint16_t cur = locate_cursor();
		uint16_t width = cur % VGA_SCREEN_WIDTH;
		uint16_t height = cur / VGA_SCREEN_WIDTH;
		if (height + 1 >= VGA_SCREEN_HEIGHT) {
			clear();
			update_cursor(0);
		} else {
			update_cursor(cur + (VGA_SCREEN_WIDTH - width));
		}
	}
}
