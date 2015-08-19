#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <console_io.h>
#include <timer.h>
#include <shell.h>
#include <io_port.h>
#include <uptime.h>

struct tss shell_tss;

int main(void)
{
	unsigned char mask;
	unsigned char i;
	extern unsigned char timer_handler;
	unsigned short segment_selector;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup GDT for shell_tss */
	init_gdt(3, (unsigned int)&shell_tss, sizeof(shell_tss));

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_NUM; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);

	/* Setup Task Register */
	segment_selector = 8 * 3;
	/* __asm__("movw %1, %0; ltr %1":"r":"r"(segment_selector)); */
	__asm__("ltr %0"::"r"(segment_selector));
	put_str("task loaded.\r\n");

	/* Setup uptime task */
	init_uptime();

	/* Setup interrupt handler and mask register */
	con_init();
	intr_set_handler(32, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(0x01 | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* Setup PIT */
	outb_p(0x34, 0x0043);
	/* 割り込み周期11932(0x2e9c)サイクル(=100Hz、10ms毎)に設定 */
	outb_p(0x9c, 0x0040);
	outb_p(0x2e, 0x0040);
	put_str("Timer initialized.\r\n");

	/* Start main task */
	start_shell();

	return 0;
}
