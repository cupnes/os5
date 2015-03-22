	.code32

	.text

	.global	main

	movl	$0x00800000, %esp

	pushl $0
	pushl $0
	pushl $0
	pushl $end
	pushl $main
	ret

end:
	jmp		end

	.data
	.bss
