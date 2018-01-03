	.text

	.global	exception_handler
exception_handler:
	jmp	exception_handler
	iret
