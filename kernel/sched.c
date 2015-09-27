#include <sched.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <debug.h>

struct task run_queue[TASK_NUM];

void schedule(void)
{
	/* void (*f)(void); */
	outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
	       IOADR_MPIC_OCW2);
	/* f = run_queue[global_counter % TASK_NUM].context_switch; */
	/* f(); */
	run_queue[global_counter % TASK_NUM].context_switch();
}
