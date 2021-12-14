#include "kbd.h"

int buflen = 0;
char kbd_buffer[KBD_BUFFER_MAX + 1] = {0};

static int shift_mod;

void
write_kbd_input(uint8_t scan_code)
{
	char c[2] = {0};
	int char_input = 1;

	/* Use XT scan code set. */
	switch (scan_code) {
	case 0x2a:	/* Detect left shift key press. */
	case 0x36:	/* Detect right shift key press. */
		char_input = 0;
		shift_mod = 1;
		break;
	case 0xaa:	/* Detect left shift release. */
	case 0xb6:	/* Detect right shift release. */
		char_input = 0;
		shift_mod = 0;
		break;
	case 0x1e:
		c[0] = 'a';
		break;
	case 0x30:
		c[0] = 'b';
		break;
	case 0x2e:
		c[0] = 'c';
		break;
	case 0x20:
		c[0] = 'd';
		break;
	case 0x12:
		c[0] = 'e';
		break;
	case 0x21:
		c[0] = 'f';
		break;
	case 0x22:
		c[0] = 'g';
		break;
	case 0x23:
		c[0] = 'h';
		break;
	case 0x17:
		c[0] = 'i';
		break;
	case 0x24:
		c[0] = 'j';
		break;
	case 0x25:
		c[0] = 'k';
		break;
	case 0x26:
		c[0] = 'l';
		break;
	case 0x32:
		c[0] = 'm';
		break;
	case 0x31:
		c[0] = 'n';
		break;
	case 0x18:
		c[0] = 'o';
		break;
	case 0x19:
		c[0] = 'p';
		break;
	case 0x10:
		c[0] = 'q';
		break;
	case 0x13:
		c[0] = 'r';
		break;
	case 0x1f:
		c[0] = 's';
		break;
	case 0x14:
		c[0] = 't';
		break;
	case 0x16:
		c[0] = 'u';
		break;
	case 0x2f:
		c[0] = 'v';
		break;
	case 0x11:
		c[0] = 'w';
		break;
	case 0x2d:
		c[0] = 'x';
		break;
	case 0x15:
		c[0] = 'y';
		break;
	case 0x2c:
		c[0] = 'z';
		break;
	case 0x0b:
		c[0] = shift_mod ? ')' : '0';
		break;
	case 0x02:
		c[0] = shift_mod ? '!' : '1';
		break;
	case 0x03:
		c[0] = shift_mod ? '@' : '2';
		break;
	case 0x04:
		c[0] = shift_mod ? '#' : '3';
		break;
	case 0x05:
		c[0] = shift_mod ? '$' : '4';
		break;
	case 0x06:
		c[0] = shift_mod ? '%' : '5';
		break;
	case 0x07:
		c[0] = shift_mod ? '^' : '6';
		break;
	case 0x08:
		c[0] = shift_mod ? '&' : '7';
		break;
	case 0x09:
		c[0] = shift_mod ? '*' : '8';
		break;
	case 0x0a:
		c[0] = shift_mod ? '(' : '9';
		break;
	case 0x39:
		c[0] = ' ';
		break;
	case 0x29:
		c[0] = shift_mod ? '~' : '`';
		break;
	case 0x0c:
		c[0] = shift_mod ? '_' : '-';
		break;
	case 0x0d:
		c[0] = shift_mod ? '+' : '=';
		break;
	case 0x2b:
		c[0] = shift_mod ? '|' : '\\';
		break;
	case 0x1a:
		c[0] = shift_mod ? '{' : '[';
		break;
	case 0x1b:
		c[0] = shift_mod ? '}' : ']';
		break;
	case 0x27:
		c[0] = shift_mod ? ':' : ';';
		break;
	case 0x28:
		c[0] = shift_mod ? '"' : '\'';
		break;
	case 0x33:
		c[0] = shift_mod ? '<' : ',';
		break;
	case 0x34:
		c[0] = shift_mod ? '>' : '.';
		break;
	case 0x35:
		c[0] = shift_mod ? '?' : '/';
		break;
	case 0x0e:	/* Detect backspace key press. */
		char_input = 0;
		if (buflen > 0) {
			bksp();
			kbd_buffer[--buflen] = 0;
		}
		break;
	case 0x1c: {	/* Detect enter key press. */
		char_input = 0;

		enum CMD_TYPE cmd = CMD_CLEAR;

		char *cmdcmp = PROMPT_CLEAR;
		if (buflen >= PROMPT_CLEAR_LEN) {
			for (uint8_t i = 0; i < PROMPT_CLEAR_LEN; ++i) {
				if (kbd_buffer[i] != cmdcmp[i]) {
					cmd = CMD_NONE;
					break;
				}
			}
		} else {
			cmd = CMD_NONE;
		}

		if (cmd == CMD_NONE && buflen >= PROMPT_PRINT_LEN) {
			cmd = CMD_PRINT;
			cmdcmp = PROMPT_PRINT;
			for (uint8_t i = 0; i < PROMPT_PRINT_LEN; ++i) {
				if (kbd_buffer[i] != cmdcmp[i]) {
					cmd = CMD_NONE;
					break;
				}
			}
		}

		switch (cmd) {
		case CMD_CLEAR:
			clear();
			update_cursor(0);
			break;
		case CMD_PRINT:
			update_cursor(
				locate_cursor() + VGA_SCREEN_WIDTH
					- buflen - PROMPT_LEN
			);
			print(kbd_buffer + PROMPT_PRINT_LEN);
		case CMD_NONE: {
			uint16_t cur = locate_cursor();
			uint16_t width = cur % VGA_SCREEN_WIDTH;
			uint16_t height = cur / VGA_SCREEN_WIDTH;
			if (height + 1 >= VGA_SCREEN_HEIGHT) {
				clear();
				update_cursor(0);
			} else {
				update_cursor(cur + (VGA_SCREEN_WIDTH - width));
			}
			break;
		}
		}

		/* Clear input buffer. */
		for (int i = 0; i < buflen; ++i) {
			kbd_buffer[i] = 0;
		}
		buflen = 0;

		print(PROMPT_DISPLAY);
		break;
	}
	default:	/* Catch all unsupported keys. */
		char_input = 0;
		break;
	}

	if (char_input && buflen < KBD_BUFFER_MAX) {
		c[0] = isalpha(c[0]) ? c[0] - (shift_mod * 32) : c[0];
		kbd_buffer[buflen++] = c[0];
		print(c);
	}
}
