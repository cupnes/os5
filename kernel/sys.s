	.code32

	.text

	.global	main

	movl	$0x00800000, %esp

	lgdt gdt_descr

	lea ignore_int,%edx
	movl $0x00080000,%eax
	movw %dx,%ax		/* selector = 0x0008 = cs */
	movw $0x8E00,%dx	/* interrupt gate - dpl=0, present */

	lea idt,%edi
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	dec %ecx
	jne rp_sidt
	lidt idt_descr

	/* マスタPICの初期化 */
	movb	$0x11, %al
	outb	%al, $0x20	/* ICW1 */
	movb	$0x20, %al
	outb	%al, $0x21	/* ICW2 */
	movb	$0x04, %al
	outb	%al, $0x21	/* ICW3 */
	movb	$0x01, %al
	outb	%al, $0x21	/* ICW4 */
	movb	$0xfd, %al
	outb	%al, $0x21	/* OCW1 */

	/* スレーブPICの初期化 */
	movb	$0x11, %al
	outb	%al, $0xa0	/* ICW1 */
	movb	$0x28, %al
	outb	%al, $0xa1	/* ICW2 */
	movb	$0x02, %al
	outb	%al, $0xa1	/* ICW3 */
	movb	$0x01, %al
	outb	%al, $0xa1	/* ICW4 */
	movb	$0xff, %al
	outb	%al, $0xa1	/* OCW1 */

	sti

	/*
	pushl $0
	pushl $0
	pushl $0
	pushl $end
	pushl $main
	ret
	*/

end:
	jmp		end

ignore_int:
	incb	0xb8000+160		/* put something on the screen	*/
	movb	$2, 0xb8000+161	/* so that we know something	*/
	movb	$0x61, %al
	outb	%al, $0x20
	inb		$0x60, %al
	iret

	.data
.align 2
.word 0
idt_descr:
	.word 256*8-1		# idt contains 256 entries
	.long idt

gdt_descr:
	.word 3*8-1
	.long gdt

	.align	8
idt:	.fill 256,8,0		/* idt is uninitialized			*/

gdt:
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad 0x00cf92000000ffff	/* 4GB(rw-:Data) */

	.bss
