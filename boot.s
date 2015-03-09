	.code16

	cli

	movw	$0x07c0, %ax
	movw	%ax, %ds
	movw	$0x0000, %ax
	movw	%ax, %ss
	movw	$0x1000, %sp

	/* ビデオモード設定(画面クリア) */
	movw	$0x0003, %ax
	int		$0x10

	movw	$msg_welcome, %si
	call	print_msg

	movw	$msg_now_loading, %si
	call	print_msg

load_sector:
	movw	$0x1000, %ax
	movw	%ax, %es
	movw	$0x0201, %ax
	movw	$0x0002, %cx
	movw	$0x0000, %dx
	movw	$0x0000, %bx
	int		$0x13
	jc		load_sector

	movw	$msg_completed, %si
	call	print_msg

	call	waitkbdout
	movb	$0xd1, %al
	outb	%al, $0x64
	call	waitkbdout
	movb	$0xdf, %al
	outb	%al, $0x60
	call	waitkbdout

	/* GDTを0x0009 0000から配置 */
	movw	$0x07c0, %ax	/* src */
	movw	%ax, %ds
	movw	$gdt, %si
	movw	$0x9000, %ax	/* dst */
	movw	%ax, %es
	subw	%di, %di
	movw	$12, %cx		/* words */
	rep		movsw

	movw	$0x07c0, %ax
	movw	%ax, %ds
	lgdtw	gdt_descr

end:
	jmp		end

print_msg:
	lodsb
	andb	%al, %al
	jz		print_msg_ret
	movb	$0xe, %ah
	movw	$7, %bx
	int		$0x10
	jmp		print_msg
print_msg_ret:
	ret
waitkbdout:
	inb		$0x64, %al
	andb	$0x02, %al
	inb		$0x60, %al
	jnz		waitkbdout
	ret

	.align 2
	.word 0
gdt_descr:
	.word 3*8-1
	.word 0x0000, 0x09
	/* .word gdt,0x07c0				*/
	/* と設定しても、				*/
	/* GDTRには、ベースアドレスが	*/
	/* 0x00c0 [gdtの場所]			*/
	/* と読み込まれてしまう			*/
gdt:
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00c09a01000007ff	/* 8Mb */
	.quad 0x00c09201000007ff	/* 8Mb */

msg_welcome:
	.ascii	"Welcome to OS5!\r\n"
	.byte	0
msg_now_loading:
	.ascii	"Now Loading ... "
	.byte	0
msg_completed:
	.ascii	"Completed!\r\n"
	.byte	0
