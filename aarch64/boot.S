	.globl	_start
_start:
	ldr x5, =0x00100000
	mov sp, x5

	bl	main

	b	.
