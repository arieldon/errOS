%macro	ISR 1
	global	isr%1
isr%1:
	cli
	push	0
	push	%1
	jmp	isr_common_stub
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


ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR 30
ISR 31
