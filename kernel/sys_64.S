	.text

	.global	keyboard_handler
keyboard_handler:
	jmp	keyboard_handler
	iretq

	.global	exception_handler
exception_handler:
	jmp	exception_handler
	iretq
