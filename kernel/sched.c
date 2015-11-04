#include <sched.h>
#include <cpu.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>

struct task task_list[TASK_NUM];

static struct task *current_task;
static struct {
	struct task *head;
	struct task *tail;
} run_queue = {0, 0};

unsigned short sched_get_current(void)
{
	return x86_get_tr() / 8;
}

void schedule(void)
{
	outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
	       IOADR_MPIC_OCW2);
	task_list[global_counter % TASK_NUM].context_switch();
}
