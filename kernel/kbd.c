#include "kbd.h"

static int shift_mod;

void
write_kbd_input(uint8_t scan_code)
{
	char c;
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
		c = 'a';
		break;
	case 0x30:
		c = 'b';
		break;
	case 0x2e:
		c = 'c';
		break;
	case 0x20:
		c = 'd';
		break;
	case 0x12:
		c = 'e';
		break;
	case 0x21:
		c = 'f';
		break;
	case 0x22:
		c = 'g';
		break;
	case 0x23:
		c = 'h';
		break;
	case 0x17:
		c = 'i';
		break;
	case 0x24:
		c = 'j';
		break;
	case 0x25:
		c = 'k';
		break;
	case 0x26:
		c = 'l';
		break;
	case 0x32:
		c = 'm';
		break;
	case 0x31:
		c = 'n';
		break;
	case 0x18:
		c = 'o';
		break;
	case 0x19:
		c = 'p';
		break;
	case 0x10:
		c = 'q';
		break;
	case 0x13:
		c = 'r';
		break;
	case 0x1f:
		c = 's';
		break;
	case 0x14:
		c = 't';
		break;
	case 0x16:
		c = 'u';
		break;
	case 0x2f:
		c = 'v';
		break;
	case 0x11:
		c = 'w';
		break;
	case 0x2d:
		c = 'x';
		break;
	case 0x15:
		c = 'y';
		break;
	case 0x2c:
		c = 'z';
		break;
	case 0x0b:
		c = shift_mod ? ')' : '0';
		break;
	case 0x02:
		c = shift_mod ? '!' : '1';
		break;
	case 0x03:
		c = shift_mod ? '@' : '2';
		break;
	case 0x04:
		c = shift_mod ? '#' : '3';
		break;
	case 0x05:
		c = shift_mod ? '$' : '4';
		break;
	case 0x06:
		c = shift_mod ? '%' : '5';
		break;
	case 0x07:
		c = shift_mod ? '^' : '6';
		break;
	case 0x08:
		c = shift_mod ? '&' : '7';
		break;
	case 0x09:
		c = shift_mod ? '*' : '8';
		break;
	case 0x0a:
		c = shift_mod ? '(' : '9';
		break;
	case 0x39:
		c = ' ';
		break;
	case 0x29:
		c = shift_mod ? '~' : '`';
		break;
	case 0x0c:
		c = shift_mod ? '_' : '-';
		break;
	case 0x0d:
		c = shift_mod ? '+' : '=';
		break;
	case 0x2b:
		c = shift_mod ? '|' : '\\';
		break;
	case 0x1a:
		c = shift_mod ? '{' : '[';
		break;
	case 0x1b:
		c = shift_mod ? '}' : ']';
		break;
	case 0x27:
		c = shift_mod ? ':' : ';';
		break;
	case 0x28:
		c = shift_mod ? '"' : '\'';
		break;
	case 0x33:
		c = shift_mod ? '<' : ',';
		break;
	case 0x34:
		c = shift_mod ? '>' : '.';
		break;
	case 0x35:
		c = shift_mod ? '?' : '/';
		break;
	case 0x0e:
		char_input = 0;
		bksp = 1;
		print_char('\0');
		bksp = 0;
		break;
	default:
		char_input = 0;
		break;
	}

	if (char_input) {
		print_char(isalpha(c) ? c - (shift_mod * 32) : c);
	}
}
