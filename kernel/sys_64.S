	.text

	.global	exception_handler
exception_handler:
	jmp	exception_handler
	iret

	.global	keyboard_handler
keyboard_handler:
	jmp	keyboard_handler
	iret
