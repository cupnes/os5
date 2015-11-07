#include <stddef.h>
#include <sched.h>
#include <cpu.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <kernel.h>
#include <kern_task.h>

struct task task_list[TASK_NUM];
struct {
	struct task *head;
	unsigned int len;
} wakeup_queue = {NULL, 0};

static struct task *current_task = NULL;
static struct {
	struct task *head;
	unsigned int len;
} run_queue = {NULL, 0};

unsigned short sched_get_current(void)
{
	return x86_get_tr() / 8;
}

int sched_runq_enq(struct task *t)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (run_queue.head) {
		t->prev = run_queue.head->prev;
		t->next = run_queue.head;
		run_queue.head->prev->next = t;
		run_queue.head->prev = t;
	} else {
		t->prev = t;
		t->next = t;
		run_queue.head = t;
	}
	run_queue.len++;

	kern_unlock(&if_bit);

	return 0;
}

void schedule(void)
{
	if (!run_queue.head) {
		if (!current_task) {
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
		} else {
			current_task = NULL;
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
			task_list[KERN_TASK_ID].context_switch();
		}
	} else if (current_task) {
		if (current_task != current_task->next) {
			current_task = current_task->next;
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
			current_task->context_switch();
		} else
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
	} else {
		current_task = run_queue.head;
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		current_task->context_switch();
	}
}
