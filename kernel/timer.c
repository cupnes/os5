#include <timer.h>
#include <io_port.h>
#include <intr.h>
#include <console_io.h>
#include <sched.h>

unsigned int global_counter = 0;

void do_ir_timer(void)
{
	unsigned char i, d, c;
	for (i = keycode_queue.start; i != keycode_queue.end; i++) {
		d = keycode_queue.buf[i];
		c = keymap[d & ~IOADR_KBC_DATA_BIT_BRAKE];
		if (c != 'a')
			while (1);
	}
	/* if (keycode_queue.is_full || keycode_queue.start != keycode_queue.end) */
	/* 	while (1); */

	global_counter++;
	schedule();
}

void timer_init(void)
{
	/* Setup PIT */
	outb_p(IOADR_PIT_CONTROL_WORD_BIT_COUNTER0
	       | IOADR_PIT_CONTROL_WORD_BIT_16BIT_READ_LOAD
	       | IOADR_PIT_CONTROL_WORD_BIT_MODE2, IOADR_PIT_CONTROL_WORD);
	/* 割り込み周期11932(0x2e9c)サイクル(=100Hz、10ms毎)に設定 */
	outb_p(0x9c, IOADR_PIT_COUNTER0);
	outb_p(0x2e, IOADR_PIT_COUNTER0);
}
