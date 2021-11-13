#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#include "io.h"
#include "kbd.h"
#include "screen.h"

enum PIC {
	PIC_MASTER_COMMAND_PORT = 0x20,
	PIC_MASTER_DATA_PORT = 0x21,
	PIC_SLAVE_COMMAND_PORT = 0xa0,
	PIC_SLAVE_DATA_PORT = 0xa1,
	PIC_ICW1_INIT = 0x10,
	PIC_EOI = 0x20,
};

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

extern struct intr_gate idt[256];
extern struct idt_ptr idt_desc;

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

void set_intr_gate(uint8_t intr_gate, uint32_t service);
void init_idt(void);
void issue_pic_eoi(uint8_t intr_gate);
void handle_isr(struct register_state state);
void handle_irq(struct register_state state);

#endif
