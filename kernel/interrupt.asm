%macro	ISR_NOERR 1
	global	isr%1
isr%1:
	cli
	push	0		; No error code to be sent for these ISRs.
	push	%1		; Push the interrupt number.
	jmp	isr_common_stub
%endmacro


%macro	ISR_ERR 1		; These ISRs provide error based on Intel ISA.
	global isr%1
isr%1:
	cli
	push	%1		; Push only the interrupt number.
	jmp	isr_common_stub
%endmacro


%macro	IRQ 1
	global	irq%1
irq%1:
	cli
	push	%1
	push	32 + %1
	jmp	irq_common_stub
%endmacro


	[extern	handle_isr]
isr_common_stub:
	pusha			; Save current state of registers.
	mov	ax, ds
	push	eax

	mov	ax, 0x10	; Load data segments with data offset from GDT.
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	call	handle_isr

	pop	eax		; Restore state of data segment registers.
	mov	ax, ds
	mov	ax, es
	mov	ax, fs
	mov	ax, gs

	popa			; Restore state of registers.
	add	esp, 8		; Restore state of stack.
	sti			; Enable more interrupts.
	iret


	[extern	handle_irq]
irq_common_stub:
	pusha
	mov	ax, ds
	push	eax

	mov	ax, 0x10
	mov	ds, ax
	mov	es, ax
	mov	fs, ax
	mov	gs, ax

	call	handle_irq

	pop	eax
	mov	ax, ds
	mov	ax, es
	mov	ax, fs
	mov	ax, gs

	popa
	add	esp, 8
	sti
	iret


ISR_NOERR	0
ISR_NOERR	1
ISR_NOERR	2
ISR_NOERR	3
ISR_NOERR	4
ISR_NOERR	5
ISR_NOERR	6
ISR_NOERR	7
ISR_ERR	8
ISR_NOERR	9
ISR_ERR	10
ISR_ERR	11
ISR_ERR	12
ISR_ERR	13
ISR_ERR	14
ISR_NOERR	15
ISR_NOERR	16
ISR_ERR	17
ISR_NOERR	18
ISR_NOERR	19
ISR_NOERR	20
ISR_ERR	21
ISR_NOERR	22
ISR_NOERR	23
ISR_NOERR	24
ISR_NOERR	25
ISR_NOERR	26
ISR_NOERR	27
ISR_NOERR	28
ISR_NOERR	29
ISR_ERR	30
ISR_NOERR	31

IRQ		0
IRQ		1
IRQ		2
IRQ		3
IRQ		4
IRQ		5
IRQ		6
IRQ		7
IRQ		8
IRQ		9
IRQ		10
IRQ		11
IRQ		12
IRQ		13
IRQ		14
IRQ		15
