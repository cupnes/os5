#include <asm/cpu.h>

	.code32

	.text

	.global	main, idt, gdt, keyboard_handler, timer_handler, exception_handler, general_protection_handler, page_fault_handler, syscall_handler

	movl	$0x00080000, %esp

	lgdt	gdt_descr

	lea	ignore_int, %edx
	movl	$0x00080000, %eax
	movw	%dx, %ax
	movw	$0x8E00, %dx
	lea	idt, %edi
	mov	$256, %ecx
rp_sidt:
	movl	%eax, (%edi)
	movl	%edx, 4(%edi)
	addl	$8, %edi
	dec	%ecx
	jne	rp_sidt
	lidt	idt_descr

	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$end
	pushl	$main
	ret

end:
	jmp	end

keyboard_handler:
	pushal
	call	do_ir_keyboard
	popal
	iret

timer_handler:
	pushal
	call	do_ir_timer
	popal
	iret

exception_handler:
	pushal
	call	do_exception
	popal
	iret

general_protection_handler:
	jmp	general_protection_handler
	iret

page_fault_handler:
	jmp	page_fault_handler
	iret

syscall_handler:
	call	do_syscall
	iret

ignore_int:
	iret

	.data
idt_descr:
	.word	256*8-1		/* idt contains 256 entries */
	.long	idt

gdt_descr:
	.word	GDT_SIZE*8-1
	.long	gdt

	.balign	8
idt:
	.fill	256, 8, 0	/* idt is uninitialized */

gdt:
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad	0x00cf92000000ffff	/* 4GB(rw-:Data) */
	.fill	GDT_SIZE-3, 8, 0
