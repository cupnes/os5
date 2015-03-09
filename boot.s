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

msg_welcome:
	.ascii	"Welcome to OS5!\r\n"
	.byte	0
msg_now_loading:
	.ascii	"Now Loading ... "
	.byte	0
msg_completed:
	.ascii	"Completed!\r\n"
	.byte	0
