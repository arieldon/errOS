#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH	80
#define VGA_HEIGHT	25

#define BLACK	 0
#define WHITE	15

enum PIC {
	PIC_MASTER_COMMAND_PORT = 0x20,
	PIC_MASTER_DATA_PORT = 0x21,
	PIC_SLAVE_COMMAND_PORT = 0xa0,
	PIC_SLAVE_DATA_PORT = 0xa1,
	PIC_ICW1_INIT = 0x10,
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

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

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

	idt_desc.limit = (sizeof(struct intr_gate) * 256) - 1;
	idt_desc.base = (uint32_t)&idt;

	asm("lidt %0"
		:
		: "m" (idt_desc));	/* Allow a memory address. */
	asm("sti");
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
	uint16_t *vgamem = (uint16_t *)0xb8000;
	for (size_t row = 0; row < VGA_HEIGHT; ++row) {
		for (size_t col = 0; col < VGA_WIDTH; ++col) {
			vgamem[row * VGA_WIDTH + col] = ' ' | (uint16_t) BLACK << 8;
		}
	}
}

void
print(char *str)
{
	uint16_t *vgamem = (uint16_t *)0xb8000;
	for (size_t i = 0; str[i] != '\0'; ++i) {
		vgamem[i] = str[i] | (uint16_t) WHITE << 8;
	}
}

void
handle_isr(void)
{
	clear();
	print("INTERRUPT.");

	/* Issue end-of-interrupt (EOI) command to PIC. */
	outb(PIC_MASTER_COMMAND_PORT, 0x20);
	outb(PIC_SLAVE_COMMAND_PORT, 0x20);

	asm("hlt");
}

void
main(void)
{
	clear();
	print("Enter kernel land.");
	init_idt();
	for (;;) {}
}
