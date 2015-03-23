	.code32

	.text

	.global	main

	movl	$0x00800000, %esp

	lgdt gdt_descr

	pushl $0
	pushl $0
	pushl $0
	pushl $end
	pushl $main
	ret

end:
	jmp		end

	.data
gdt_descr:
	.word 3*8-1
	.long gdt
gdt:
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad 0x00cf92000000ffff	/* 4GB(rw-:Data) */

	.bss
