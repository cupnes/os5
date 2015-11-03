#include <sched.h>
#include <cpu.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>

struct task run_queue[TASK_NUM];

unsigned short sched_get_current(void)
{
	return x86_get_tr() / 8;
}

void schedule(void)
{
	outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
	       IOADR_MPIC_OCW2);
	run_queue[global_counter % TASK_NUM].context_switch();
}
