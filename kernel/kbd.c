#include "kbd.h"

void
write_kbd_input(uint8_t c)
{
	/* Use XT scan code set. */
	switch (c) {
	case 0x1e:
		print_char('A');
		break;
	case 0x30:
		print_char('B');
		break;
	case 0x2e:
		print_char('C');
		break;
	case 0x20:
		print_char('D');
		break;
	case 0x12:
		print_char('E');
		break;
	case 0x21:
		print_char('F');
		break;
	case 0x22:
		print_char('G');
		break;
	case 0x23:
		print_char('H');
		break;
	case 0x17:
		print_char('I');
		break;
	case 0x24:
		print_char('J');
		break;
	case 0x25:
		print_char('K');
		break;
	case 0x26:
		print_char('L');
		break;
	case 0x32:
		print_char('M');
		break;
	case 0x31:
		print_char('N');
		break;
	case 0x18:
		print_char('O');
		break;
	case 0x19:
		print_char('P');
		break;
	case 0x10:
		print_char('Q');
		break;
	case 0x13:
		print_char('R');
		break;
	case 0x1f:
		print_char('S');
		break;
	case 0x14:
		print_char('T');
		break;
	case 0x16:
		print_char('U');
		break;
	case 0x2f:
		print_char('V');
		break;
	case 0x11:
		print_char('W');
		break;
	case 0x2d:
		print_char('X');
		break;
	case 0x15:
		print_char('Y');
		break;
	case 0x2c:
		print_char('Z');
		break;
	case 0x0b:
		print_char('0');
		break;
	case 0x02:
		print_char('1');
		break;
	case 0x03:
		print_char('2');
		break;
	case 0x04:
		print_char('3');
		break;
	case 0x05:
		print_char('4');
		break;
	case 0x06:
		print_char('5');
		break;
	case 0x07:
		print_char('6');
		break;
	case 0x08:
		print_char('7');
		break;
	case 0x09:
		print_char('8');
		break;
	case 0x0a:
		print_char('9');
		break;
	case 0x39:
		print_char(' ');
		break;
	case 0x29:
		print_char('`');
		break;
	case 0x0c:
		print_char('-');
		break;
	case 0x0d:
		print_char('=');
		break;
	case 0x2b:
		print_char('\\');
		break;
	case 0x1a:
		print_char('[');
		break;
	case 0x1b:
		print_char(']');
		break;
	case 0x27:
		print_char(';');
		break;
	case 0x28:
		print_char('\'');
		break;
	case 0x33:
		print_char(',');
		break;
	case 0x34:
		print_char('.');
		break;
	case 0x35:
		print_char('/');
		break;
	case 0x0e:
		bksp = 1;
		print_char('\0');
		bksp = 0;
		break;
	default:
		break;
	}
}
