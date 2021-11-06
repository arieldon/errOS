#include "idt.h"

struct intr_gate idt[256] = {0};
struct idt_ptr idt_desc = {0};

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
handle_isr(struct register_state state)
{
	clear();
	print("INTERRUPT");
	issue_pic_eoi(state.intr_gate);
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
