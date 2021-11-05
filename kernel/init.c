#include <stddef.h>
#include <stdint.h>

enum VGA {
	VGA_MEMORY_START = 0xb8000,
	VGA_SCREEN_WIDTH = 80,
	VGA_SCREEN_HEIGHT = 25,
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_WHITE = 15,
};

enum PIC {
	PIC_MASTER_COMMAND_PORT = 0x20,
	PIC_MASTER_DATA_PORT = 0x21,
	PIC_SLAVE_COMMAND_PORT = 0xa0,
	PIC_SLAVE_DATA_PORT = 0xa1,
	PIC_ICW1_INIT = 0x10,
	PIC_EOI = 0x20,
};

enum KBD {
	  KBD_INPUT_PORT = 0x60,
};

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

struct register_state {
	uint32_t ds;
	/* These registers are pushed by instruction `pusha`. */
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t intr_gate, err;
	/* The following are pushed by CPU upon interrupt. */
	uint32_t eip, cs, eflags, useresp, ss;
};

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct intr_gate {
	uint16_t low_offset;	/* Indicate number of interrupt. */
	uint16_t selector;	/* Specify code segment offset in GDT. */
	uint8_t zilch;
	uint8_t type_attr;	/* Specify type as interrupt gate. */
	uint16_t high_offset;	/* Denote starting address of IDT. */
} __attribute__((packed));

struct intr_gate idt[256];
struct idt_ptr idt_desc;

void
issue_pic_eoi(uint8_t intr_gate)
{
	/*
	 * Only issue end-of-interrupt signal to slave PIC if it handled the
	 * IRQ. Always issue end-of-interrupt to master PIC.
	 */
	if (intr_gate > 0x28) {
		outb(PIC_SLAVE_COMMAND_PORT, PIC_EOI);
	}
	outb(PIC_MASTER_COMMAND_PORT, PIC_EOI);
}

void
set_intr_gate(uint8_t intr_gate, uint32_t service)
{
	idt[intr_gate].low_offset = service & 0xffff;
	idt[intr_gate].selector = 0x08;
	idt[intr_gate].zilch = 0x00;
	idt[intr_gate].type_attr = 0x8e;
	idt[intr_gate].high_offset = (service >> 16) && 0xffff;
}

void
init_idt(void)
{
	/*
	 * Instantiate PIC for protected mode instead of real mode with a series
	 * of initialization control words.
	 *
	 * 1. Send initialization signal to master and slave PICs, respectively.
	 * 2. Map base address of IRQs in IDT for each PIC. Master PIC handles
	 *    IRQs 32 to 39; slave PIC handles IRQs 40 to 47, both inclusive.
	 *    IRQs 0 to 31 are reserved.
	 * 3. Notify each PIC of the IRQ line it must use to communicate with
	 *    the other. In this case, PICs are directed to communicate over
	 *    IRQ line 2. For the master PIC, the second bit is set, 0100, to
	 *    indicate this line.
	 */
	outb(PIC_MASTER_COMMAND_PORT, PIC_ICW1_INIT);
	outb(PIC_SLAVE_COMMAND_PORT, PIC_ICW1_INIT);

	outb(PIC_MASTER_DATA_PORT, 0x20);
	outb(PIC_SLAVE_DATA_PORT, 0x28);

	outb(PIC_MASTER_DATA_PORT, 0x04);
	outb(PIC_SLAVE_DATA_PORT, 0x02);

	/* Set IA-32 reserved interrupt responses. */
	set_intr_gate(0, (uint32_t)isr0);
	set_intr_gate(1, (uint32_t)isr1);
	set_intr_gate(2, (uint32_t)isr2);
	set_intr_gate(3, (uint32_t)isr3);
	set_intr_gate(4, (uint32_t)isr4);
	set_intr_gate(5, (uint32_t)isr5);
	set_intr_gate(6, (uint32_t)isr6);
	set_intr_gate(7, (uint32_t)isr7);
	set_intr_gate(8, (uint32_t)isr8);
	set_intr_gate(9, (uint32_t)isr9);
	set_intr_gate(10, (uint32_t)isr10);
	set_intr_gate(11, (uint32_t)isr11);
	set_intr_gate(12, (uint32_t)isr12);
	set_intr_gate(13, (uint32_t)isr13);
	set_intr_gate(14, (uint32_t)isr14);
	set_intr_gate(15, (uint32_t)isr15);
	set_intr_gate(16, (uint32_t)isr16);
	set_intr_gate(17, (uint32_t)isr17);
	set_intr_gate(18, (uint32_t)isr18);
	set_intr_gate(19, (uint32_t)isr19);
	set_intr_gate(20, (uint32_t)isr20);
	set_intr_gate(21, (uint32_t)isr21);
	set_intr_gate(22, (uint32_t)isr22);
	set_intr_gate(23, (uint32_t)isr23);
	set_intr_gate(24, (uint32_t)isr24);
	set_intr_gate(25, (uint32_t)isr25);
	set_intr_gate(26, (uint32_t)isr26);
	set_intr_gate(27, (uint32_t)isr27);
	set_intr_gate(28, (uint32_t)isr28);
	set_intr_gate(29, (uint32_t)isr29);
	set_intr_gate(30, (uint32_t)isr30);
	set_intr_gate(31, (uint32_t)isr31);

	/* Set responses to custom interrupt requests. */
	set_intr_gate(32, (uint32_t)irq0);
	set_intr_gate(33, (uint32_t)irq1);
	set_intr_gate(34, (uint32_t)irq2);
	set_intr_gate(35, (uint32_t)irq3);
	set_intr_gate(36, (uint32_t)irq4);
	set_intr_gate(37, (uint32_t)irq5);
	set_intr_gate(38, (uint32_t)irq6);
	set_intr_gate(39, (uint32_t)irq7);
	set_intr_gate(40, (uint32_t)irq8);
	set_intr_gate(41, (uint32_t)irq9);
	set_intr_gate(42, (uint32_t)irq10);
	set_intr_gate(43, (uint32_t)irq11);
	set_intr_gate(44, (uint32_t)irq12);
	set_intr_gate(45, (uint32_t)irq13);
	set_intr_gate(46, (uint32_t)irq14);
	set_intr_gate(47, (uint32_t)irq15);

	idt_desc.limit = (sizeof(struct intr_gate) * 256) - 1;
	idt_desc.base = (uint32_t)&idt;

	asm("lidt %0"			/* Load address of IDT to register. */
		:
		: "m" (idt_desc));	/* Allow a memory address. */
	asm("sti");			/* Enable interrupts. */
}

uint8_t
inb(uint16_t port)
{
	/* Read a single byte from the specified hardware I/O port. */
	uint8_t ret;
	asm("inb %1, %0"
		: "=a" (ret)
		: "Nd" (port));
	return ret;
}

void
outb(uint16_t port, uint8_t val)
{
	/* Write a designated single byte to the specified hardware I/O port. */
	asm("outb %0, %1"
		:
		: "a" (val), "Nd" (port));
}

void
clear(void)
{
	uint16_t *vgamem = (uint16_t *)VGA_MEMORY_START;
	for (size_t row = 0; row < VGA_SCREEN_HEIGHT; ++row) {
		for (size_t col = 0; col < VGA_SCREEN_WIDTH; ++col) {
			vgamem[row * VGA_SCREEN_WIDTH + col] = ' ' |
				(uint16_t) VGA_COLOR_BLACK << 8;
		}
	}
}

void
print(char *str)
{
	uint16_t *vgamem = (uint16_t *)VGA_MEMORY_START;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		vgamem[i] = str[i] | (uint16_t) VGA_COLOR_WHITE << 8;
	}
}

void
print_char(char c)
{
	static uint16_t *vgaloc = (uint16_t *)VGA_MEMORY_START;

	if (vgaloc == (uint16_t *)VGA_MEMORY_START) {
		clear();
	}

	*vgaloc = c | (uint16_t) VGA_COLOR_WHITE << 8;
	++vgaloc;
}

void
ack_intr(int n)
{
	char s[] = "INTERRUPT 0000";
	for (int i = 13; i > 9; --i, n /= 10) {
		s[i] = ((char)(n % 10)) + 48;
	}
	print(s);
}

void
handle_isr(struct register_state state)
{
	clear();
	ack_intr(state.intr_gate);
	issue_pic_eoi(state.intr_gate);
}

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
	default:
		break;
	}
}

void
handle_irq(struct register_state state)
{
	switch (state.intr_gate) {
	case 0x20:
		break;
	case 0x21:
		write_kbd_input(inb(KBD_INPUT_PORT));
		break;
	default:
		break;
	}
	issue_pic_eoi(state.intr_gate);
}

void
main(void)
{
	clear();
	init_idt();
	print("Enter kernel land.");
	for (;;) {}
}
