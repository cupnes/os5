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
	int	$0x10

	movw	$msg_welcome, %si
	call	print_msg

	movw	$msg_now_loading, %si
	call	print_msg

	/* ディスクサービス セクタ読み込み
	 * int 0x13, AH=0x02
	 * 入力
	 * - AL: 読み込むセクタ数
	 * - CH: トラックの下位8ビット
	 * - CL(上位2ビット): トラックの上位2ビット
	 * - CL(下位6ビット): セクタを指定
	 * - DH: ヘッド番号を指定
	 * - DL: セクタを読み込むドライブ番号を指定
	 * - ES: 読み込み先のセグメント指定
	 * - BX: 読み込み先のオフセットアドレス指定
	 * 出力
	 * - EFLAGSのCFビット: 0=成功, 1=失敗
	 * - AH: エラーコード(0x00=成功)
	 * - AL: 読み込んだセクタ数
	 * 備考
	 * - トラック番号: 0始まり
	 * - ヘッド番号: 0始まり
	 * - セクタ番号: 1始まり
	 * - セクタ数/トラック: 2HDは18
	 * - セクタ18の次は、別トラック(裏面)へ
	 * - 64KB境界を超えて読みだすことはできない
	 *   (その際は、2回に分ける)
	 */

	/* トラック0, ヘッド0, セクタ2以降
	 * src: トラック0, ヘッド0のセクタ2以降
	 *      (17セクタ = 8704バイト = 0x2200バイト)
	 * dst: 0x0000 7e00 〜 0x0000 bfff
	 */
load_track0_head0:
	movw	$0x0000, %ax
	movw	%ax, %es
	movw	$0x7e00, %bx
	movw	$0x0000, %dx
	movw	$0x0002, %cx
	movw	$0x0211, %ax
	int	$0x13
	jc	load_track0_head0

	/* トラック0, ヘッド1, 全セクタ
	 * src: トラック0, ヘッド1の全セクタ
	 *      (18セクタ = 9216バイト = 0x2400バイト)
	 * dst: 0x0000 a000 〜 0x0000 c3ff
	 */
load_track0_head1:
	movw	$0x0000, %ax
	movw	%ax, %es
	movw	$0xa000, %bx
	movw	$0x0100, %dx
	movw	$0x0001, %cx
	movw	$0x0212, %ax
	int	$0x13
	jc	load_track0_head1

	/* トラック1, ヘッド0, 全セクタ
	 * src: トラック1, ヘッド0の全セクタ
	 *      (18セクタ = 9216バイト = 0x2400バイト)
	 * dst: 0x0000 c400 〜 0x0000 e7ff
	 */
load_track1_head0:
	movw	$0x0000, %ax
	movw	%ax, %es
	movw	$0xc400, %bx
	movw	$0x0000, %dx
	movw	$0x0101, %cx
	movw	$0x0212, %ax
	int	$0x13
	jc	load_track1_head0

	/* トラック1, ヘッド1, セクタ1 - 12
	 * src: トラック1, ヘッド1の12セクタ
	 *      (12セクタ = 6144バイト = 0x1800バイト)
	 * dst: 0x0000 e800 〜 0x0000 ffff
	 */
load_track1_head1_1:
	movw	$0x0000, %ax
	movw	%ax, %es
	movw	$0xe800, %bx
	movw	$0x0100, %dx
	movw	$0x0101, %cx
	movw	$0x020c, %ax
	int	$0x13
	jc	load_track1_head1_1

	/* トラック1, ヘッド1, セクタ13 - 18
	 * src: トラック1, ヘッド1の6セクタ
	 *      (6セクタ = 3072バイト = 0xc00バイト)
	 * dst: 0x0001 0000 〜 0x0001 0bff
	 */
load_track1_head1_2:
	movw	$0x1000, %ax
	movw	%ax, %es
	movw	$0x0000, %bx
	movw	$0x0100, %dx
	movw	$0x010d, %cx
	movw	$0x0206, %ax
	int	$0x13
	jc	load_track1_head1_2

	/* トラック2, ヘッド0, 全セクタ
	 * src: トラック2, ヘッド0の全セクタ
	 *      (18セクタ = 9216バイト = 0x2400バイト)
	 * dst: 0x0001 0c00 〜 0x0001 2fff
	 */
load_track2_head0:
	movw	$0x1000, %ax
	movw	%ax, %es
	movw	$0x0c00, %bx
	movw	$0x0000, %dx
	movw	$0x0201, %cx
	movw	$0x0212, %ax
	int	$0x13
	jc	load_track2_head0

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

	/* GDTを0x0009 0000から配置 */
	movw	$0x07c0, %ax	/* src */
	movw	%ax, %ds
	movw	$gdt, %si
	movw	$0x9000, %ax	/* dst */
	movw	%ax, %es
	subw	%di, %di
	movw	$12, %cx	/* words */
	rep	movsw

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

	ljmp	$8, $0x7e00

print_msg:
	lodsb
	andb	%al, %al
	jz	print_msg_ret
	movb	$0xe, %ah
	movw	$7, %bx
	int	$0x10
	jmp	print_msg
print_msg_ret:
	ret
waitkbdout:
	inb	$0x64, %al
	andb	$0x02, %al
	inb	$0x60, %al
	jnz	waitkbdout
	ret

	.data
gdt_descr:
	.word	3*8-1
	.word	0x0000, 0x09
	/* .word gdt,0x07c0
	 * と設定しても、
	 * GDTRには、ベースアドレスが
	 * 0x00c0 [gdtの場所]
	 * と読み込まれてしまう
	 */
gdt:
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00cf9a000000ffff	/* 4GB(r-x:Code) */
	.quad	0x00cf92000000ffff	/* 4GB(rw-:Data) */

msg_welcome:
	.ascii	"Welcome to OS5!\r\n"
	.byte	0
msg_now_loading:
	.ascii	"Now Loading ... "
	.byte	0
msg_completed:
	.ascii	"Completed!\r\n"
	.byte	0
