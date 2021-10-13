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

	mov	sp, 0x7c00	; Initialize stack for real mode.

	mov	si, msgrm
	call	bios_print

	call	load_A20
	call	load_disk

	hlt			; Wait until next interrupt.


bios_print:
	lodsb
	or	al, al		; Exit if terminating byte.
	jz	.exit
	mov	ah, 0x0e	; Enable BIOS teletype mode.
	int	0x10
	jmp	bios_print
.exit:	ret


load_A20:
	mov	ax, 0x2401	; Ask BIOS to enable A20 gate.
	int	0x15
	jnc	.exit
	mov	si, A20err	; Output message and halt upon error.
	call	bios_print
	hlt
.exit:	ret


load_disk:
	mov	bx, 0x7c00 + 512; Load address 512MB after start of bootloader.
	mov	ah, 0x02	; Select BIOS read sector function.
	mov	al, 15		; Set number of sectors to read.

	;; Use CHS (Cylinder-Head-Sector) addressing to specify location on
	;; floppy.
	mov	ch, 0x00	; Select first cylinder.
	mov	dh, 0x00	; Select first head.
	mov	cl, 0x02	; Select sector following boot sector.
	mov	dl, [disk]	; Select drive number.

	int	0x13
	jnc	.exit		; BIOS sets carry flag upon error.

	mov	si, diskerr	; Upon error, output message and stop.
	call	bios_print
	hlt
.exit:	ret


	;; Declare and define variables.
disk	db	0
msgrm	db	"Enter real mode.", 0x0d, 0x0a, 0x00
diskerr	db	"Unable to load disk.", 0x0d, 0x0a, 0x00
A20err	db	"Unable to enable A20.", 0x0d, 0x0a, 0x00

	;; Pad remaining bits of boot sector and mark it bootable.
	times	510 - ($ - $$) db 0
	dw	0xaa55
