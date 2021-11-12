#include "kbd.h"

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
	case 0x0e:
		char_input = 0;
		bksp();
		break;
	default:	/* Catch all unsupported keys. */
		char_input = 0;
		break;
	}

	if (char_input) {
		c[0] = isalpha(c[0]) ? c[0] - (shift_mod * 32) : c[0];
		print(c);
	}
}
