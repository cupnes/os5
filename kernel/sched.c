#include <sched.h>
#include <cpu.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <kernel.h>

struct task task_list[TASK_NUM];

static struct task *current_task = 0;
static struct {
	struct task *head;
	struct task *tail;
	unsigned int len;
} run_queue = {0, 0, 0};

void sched_init(void)
{
	current_task = run_queue.tail;
}

unsigned short sched_get_current(void)
{
	return x86_get_tr() / 8;
}

int sched_runq_enq(struct task *t)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (!run_queue.head)
		run_queue.head = t;
	t->next = run_queue.head;
	if (run_queue.tail)
		run_queue.tail->next = t;
	run_queue.tail = t;
	run_queue.len++;

	kern_unlock(&if_bit);

	return 0;
}

void schedule(void)
{
	current_task = current_task->next;
	outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
	       IOADR_MPIC_OCW2);
	current_task->context_switch();
}
