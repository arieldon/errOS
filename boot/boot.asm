	[bits 16]
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
	call	load_pm

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
	mov	ax, 0x2401	; Ask BIOS (nicely) to enable A20 gate.
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


load_pm:
	lgdt	[gdt.desc]	; Load GDT defined below.

	;; Ensure interrupts are disabled upon entering protected mode since
	;; IVT is not available between modes. As a result, an interrupt crashes
	;; the system.
	mov	eax, cr0	; Set control bit to enter protected mode.
	or	eax, 1
	mov	cr0, eax

	jmp	0x08:enter_pm	; `enter_pm` exists ins econd stage bootloader.


gdt:
.null:	;; Create obligatory null descriptor.
	dq	0x00
.code:	;; Create executable/readable code segment at ring 0.
	;; a0 <- Set by CPU when segment accessed.
	;; a1 <- Set readable.
	;; a2 <- Limit access to segment to its privilege ring (nonconforming).
	;; a3 <- Label as code descriptor.
	;; a4 <- Label as code/data segment, not system segment.
	;; a5 <- Set privilege level to ring 00b, not 11b.
	;; a6 <- Set privilege level to ring 00b, not 11b.
	;; a7 <- Indicate segment is present in memory.
	;; b4 <- This bit is available for use by system software.
	;; b5 <- Do not define as a 64-bit long mode segment.
	;; b6 <- Define as 32-bit segment instead of 16-bit.
	;; b7 <- Increment segments in 4KB chunks up to 4GB.
	dw	0xffff		; Set segment limit with first 2 bytes.
	dw	0x00		; Set base with next 3 bytes.
	db	0x00		; Allow code segment access from 0x0 to 0xffff.
	db	10011010b	; a7 a6 a5 a4 a3 a2 a1 a0
	db	11001111b	; b7 b6 b5 b4 (flags, limit)
	db	0x00		; Set base or starting address.
.data:	;; Create readable/writable data segment at ring 0.
	;; Barring bit a3, flags for data segment remain same as above, but
	;; some flags hold different meanings in some cases specified below.
	;; a1 <- Set writable.
	;; a2 <- Grow downward in memory.
	;; a3 <- Label as data descriptor.
	dw	0xffff		; Set limit.
	dw	0x00		; Set base to zero across next 3 bytes.
	db	0x00
	db	10010010b
	db	11001111b	; Top 4 bits represent flags, least 4 limit.
	db	0x00		; Set base or starting address.
.desc:	;; Indicate size and start of GDT, respectively.
	dw	$ - gdt - 1
	dd	gdt


	;; Declare and define variables.
disk	db	0
msgrm	db	"Enter real mode.", 0x0d, 0x0a, 0x00
diskerr	db	"Unable to load disk.", 0x0d, 0x0a, 0x00
A20err	db	"Unable to enable A20.", 0x0d, 0x0a, 0x00


	;; Pad remaining bits of boot sector and mark it bootable.
	times	510 - ($ - $$) db 0
	dw	0xaa55


	[bits 32]
enter_pm:
	mov	ax, 0x10	; Set data segments to data selector from
	mov	ds, ax		; GDT defined in prior stage of bootloader.
	mov	es, ax
	mov	ss, ax
	mov	fs, ax
	mov	gs, ax

	hlt


clear_cursor:
	mov	dx, 0x3d4
	mov	al, 0x0a
	out	dx, al
	inc	dx
	mov	al, 0x20
	out	dx, al
	ret


clear_screen:
	mov	edx, vgamem
	xor	ax, ax

.fill:	mov	[edx], ax
	add	edx, 2
	cmp	edx, vgamem + 80 * 25 * 2
	jl	.fill

	call	clear_cursor
	ret


pm_print:
	mov	edx, vgamem
	mov	ah, 0x0f

.print:	lodsb
	or	al, al
	jz	.exit
	mov	[edx], ax
	add	edx, 2
	jmp	.print
.exit:	ret


msgpm	db	"Enter protected mode.", 0x00
vgamem	equ	0xb8000
