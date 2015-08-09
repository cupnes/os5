	.code32

	.text

	.global	main, idt, gdt, keyboard_handler, exception_handler, load_task_register

	movl	$0x00080000, %esp

	lgdt	gdt_descr

	lea		ignore_int, %edx
	movl	$0x00080000, %eax
	movw	%dx, %ax
	movw	$0x8E00, %dx
	lea		idt, %edi
	mov		$256, %ecx
rp_sidt:
	movl	%eax, (%edi)
	movl	%edx, 4(%edi)
	addl	$8, %edi
	dec		%ecx
	jne		rp_sidt
	lidt	idt_descr

	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$end
	pushl	$main
	ret

end:
	jmp		end

keyboard_handler:
	call	do_ir_keyboard
	iret

exception_handler:
	jmp		exception_handler
	iret

ignore_int:
	iret

load_task_register:
	movl	$0x18, %eax
	ltr		%ax
	ret

	.data
idt_descr:
	.word 256*8-1		# idt contains 256 entries
	.long idt

gdt_descr:
	.word 4*8-1
	.long gdt

idt:	.fill 256,8,0		/* idt is uninitialized			*/

gdt:
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad 0x00cf92000000ffff	/* 4GB(rw-:Data) */
	.fill 5, 8, 0
