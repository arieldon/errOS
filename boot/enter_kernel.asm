	[bits 32]
	[extern main]
enter_kernel:
	call	main
	hlt
