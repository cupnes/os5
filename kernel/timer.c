#include <timer.h>
#include <io_port.h>
#include <intr.h>
#include <sched.h>

unsigned int global_counter = 0;

void do_ir_timer(void)
{
	global_counter += TIMER_TICK_MS;
	sched_update_wakeupq();
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

unsigned int timer_get_global_counter(void)
{
	return global_counter;
}
