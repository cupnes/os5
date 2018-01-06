	.text

	.global	exception_handler
exception_handler:
	jmp	exception_handler
	iretq

	.global	keyboard_handler
keyboard_handler:
	push	%rax
	push	%rcx
	push	%rdx
	push	%rbx
	push	%rsp
	push	%rbp
	push	%rsi
	push	%rdi
	call	do_ir_keyboard
	pop	%rdi
	pop	%rsi
	pop	%rbp
	pop	%rsp
	pop	%rbx
	pop	%rdx
	pop	%rcx
	pop	%rax
	iretq
