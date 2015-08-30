#include <sched.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <shell.h>
#include <uptime.h>

struct task task[TASK_NUM];

void schedule(void)
{
	if (global_counter % 2) {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		task[UPTIME_ID].context_switch();
	} else {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		task[SHELL_ID].context_switch();
	}
}
