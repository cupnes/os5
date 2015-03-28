	.code16

	.text
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

load_sectors:
	movw	$0x1000, %ax
	movw	%ax, %es
	movw	$0x0211, %ax
	movw	$0x0002, %cx
	movw	$0x0000, %dx
	movw	$0x0000, %bx
	int		$0x13
	jc		load_sectors

	movw	$msg_completed, %si
	call	print_msg

	/* マスタPICの初期化 */
	movb	$0x10, %al
	outb	%al, $0x20	/* ICW1 */
	movb	$0x00, %al
	outb	%al, $0x21	/* ICW2 */
	movb	$0x04, %al
	outb	%al, $0x21	/* ICW3 */
	movb	$0x01, %al
	outb	%al, $0x21	/* ICW4 */
	movb	$0xff, %al
	outb	%al, $0x21	/* OCW1 */

	/* スレーブPICの初期化 */
	movb	$0x10, %al
	outb	%al, $0xa0	/* ICW1 */
	movb	$0x00, %al
	outb	%al, $0xa1	/* ICW2 */
	movb	$0x02, %al
	outb	%al, $0xa1	/* ICW3 */
	movb	$0x01, %al
	outb	%al, $0xa1	/* ICW4 */
	movb	$0xff, %al
	outb	%al, $0xa1	/* OCW1 */

	call	waitkbdout
	movb	$0xd1, %al
	outb	%al, $0x64
	call	waitkbdout
	movb	$0xdf, %al
	outb	%al, $0x60
	call	waitkbdout

	/* 0x0001 0000から4KB配置していたsysを	*/
	/* 0x0000 0000からの4KBへ移動			*/
	movw	$0x1000, %ax	/* src */
	movw	%ax, %ds
	subw	%si, %si
	movw	$0x0000, %ax	/* dst */
	movw	%ax, %es
	subw	%di, %di
	movw	$4352, %cx		/* words */
	rep		movsw

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

	movw	$0x0001, %ax
	lmsw	%ax

	movw	$2*8, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	ljmp	$8, $0

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

	.data
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
	.quad 0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad 0x00cf92000000ffff	/* 4GB(rw-:Data) */

msg_welcome:
	.ascii	"Welcome to OS5!\r\n"
	.byte	0
msg_now_loading:
	.ascii	"Now Loading ... "
	.byte	0
msg_completed:
	.ascii	"Completed!\r\n"
	.byte	0
