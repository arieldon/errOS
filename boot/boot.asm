	[org 0x7c00]		; BIOS loads this at 0000:7c00.
start:
	cli			; Ignore hardware interrupts.
	cld			; Move forward in memory for string operations.

	mov	[disk], dl	; Save boot disk set by BIOS.

	xor	ax, ax		; Zero segment registers.
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	fs, ax
	mov	gs, ax

	mov	si, msg
	call	bios_print

	hlt			; Wait until next interrupt.


bios_print:
	lodsb
	or	al, al		; Exit if terminating byte.
	jz	.exit
	mov	ah, 0x0e	; Enable BIOS teletype mode.
	int	0x10
	jmp	bios_print
.exit:	ret


	;; Declare and define variables.
disk	db	0
msg	db	"hello, world", 0x0d, 0x0a, 0x00

	;; Pad remaining bits of boot sector and mark it bootable.
	times	510 - ($ - $$) db 0
	dw	0xaa55
