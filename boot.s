	.code16

	cli

	/* ビデオモード設定(画面クリア) */
	movw	$0x0000, %ax
	int		$0x10

	/* 文字列出力 */
	BOOTSEG = 0x07c0
	movw	$0x0000, %dx
	movw	$5, %cx
	movw	$0x0007, %bx
	movw	$BOOTSEG, %ax
	movw	%ax, %es
	movw	$msg, %bp
	movw	$0x1301, %ax
	int		$0x10

end:
	jmp		end

msg:
	.ascii	"Hello"
