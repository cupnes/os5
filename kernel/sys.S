#include <asm/cpu.h>

	.code32

	.text

	.global	main, idt, keyboard_handler

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
	call	do_ir_keyboard
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

idt:
	.fill	256, 8, 0	/* idt is uninitialized */

gdt:
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad	0x00cf92000000ffff	/* 4GB(rw-:Data) */
	.fill	GDT_SIZE-3, 8, 0
