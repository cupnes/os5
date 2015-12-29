#include <stddef.h>
#include <sched.h>
#include <cpu.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <kernel.h>
#include <kern_task.h>

struct task task_list[TASK_NUM];

static struct task *current_task = NULL;
static struct {
	struct task *head;
	unsigned int len;
} run_queue = {NULL, 0};
static struct {
	struct task *head;
	unsigned int len;
} wakeup_queue = {NULL, 0};
static struct {
	struct task *head;
	unsigned int len;
} wakeup_event_queue = {NULL, 0};
static struct task dummy_task;

/*
00000000 <context_switch>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   ea 00 00 00 00 00 00    ljmp   $0x00,$0x0
   a:   5d                      pop    %ebp
   b:   c3                      ret
 */
unsigned char context_switch_template[CONTEXT_SWITCH_FN_SIZE] = {
	0x55,
	0x89, 0xe5,
	0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x5d,
	0xc3
};

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

int sched_runq_del(struct task *t)
{
	unsigned char if_bit;

	if (!run_queue.head)
		return -1;

	kern_lock(&if_bit);

	if (run_queue.head->next != run_queue.head) {
		if (run_queue.head == t)
			run_queue.head = run_queue.head->next;
		t->prev->next = t->next;
		t->next->prev = t->prev;
	} else
		run_queue.head = NULL;
	run_queue.len--;

	kern_unlock(&if_bit);

	return 0;
}

void schedule(unsigned char cause_id)
{
	if ((cause_id == SCHED_CAUSE_TIMER) && current_task
	    && current_task->task_switched_in_time_slice) {
		current_task->task_switched_in_time_slice = 0;
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		return;
	}

	if (current_task)
		current_task->task_switched_in_time_slice = 0;

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
			if (cause_id == SCHED_CAUSE_SYSCALL)
				current_task->task_switched_in_time_slice = 1;
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
			current_task->context_switch();
		} else
			outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
			       IOADR_MPIC_OCW2);
	} else {
		current_task = run_queue.head;
		if (cause_id == SCHED_CAUSE_SYSCALL)
			current_task->task_switched_in_time_slice = 1;
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		current_task->context_switch();
	}
}

int sched_wakeupq_enq(struct task *t)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (wakeup_queue.head) {
		t->prev = wakeup_queue.head->prev;
		t->next = wakeup_queue.head;
		wakeup_queue.head->prev->next = t;
		wakeup_queue.head->prev = t;
	} else {
		t->prev = t;
		t->next = t;
		wakeup_queue.head = t;
	}
	wakeup_queue.len++;

	kern_unlock(&if_bit);

	return 0;
}

int sched_wakeupq_del(struct task *t)
{
	unsigned char if_bit;

	if (!wakeup_queue.head)
		return -1;

	kern_lock(&if_bit);

	if (wakeup_queue.head->next != wakeup_queue.head) {
		if (wakeup_queue.head == t)
			wakeup_queue.head = wakeup_queue.head->next;
		t->prev->next = t->next;
		t->next->prev = t->prev;
	} else
		wakeup_queue.head = NULL;
	wakeup_queue.len--;

	kern_unlock(&if_bit);

	return 0;
}

int sched_update_wakeupq(void)
{
	struct task *t, *next;
	unsigned char if_bit;

	if (!wakeup_queue.head)
		return -1;

	kern_lock(&if_bit);

	t = wakeup_queue.head;
	do {
		next = t->next;
		if (t->wakeup_after_msec > TIMER_TICK_MS) {
			t->wakeup_after_msec -= TIMER_TICK_MS;
		} else {
			t->wakeup_after_msec = 0;
			sched_wakeupq_del(t);
			sched_runq_enq(t);
		}
		t = next;
	} while (wakeup_queue.head && t != wakeup_queue.head);

	kern_unlock(&if_bit);

	return 0;
}

void wakeup_after_msec(unsigned int msec)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (current_task->next != current_task)
		dummy_task.next = current_task->next;
	current_task->wakeup_after_msec = msec;
	sched_runq_del(current_task);
	sched_wakeupq_enq(current_task);
	current_task = &dummy_task;
	schedule(SCHED_CAUSE_SYSCALL);

	kern_unlock(&if_bit);
}

int sched_wakeupevq_enq(struct task *t)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (wakeup_event_queue.head) {
		t->prev = wakeup_event_queue.head->prev;
		t->next = wakeup_event_queue.head;
		wakeup_event_queue.head->prev->next = t;
		wakeup_event_queue.head->prev = t;
	} else {
		t->prev = t;
		t->next = t;
		wakeup_event_queue.head = t;
	}
	wakeup_event_queue.len++;

	kern_unlock(&if_bit);

	return 0;
}

int sched_wakeupevq_del(struct task *t)
{
	unsigned char if_bit;

	if (!wakeup_event_queue.head)
		return -1;

	kern_lock(&if_bit);

	if (wakeup_event_queue.head->next != wakeup_event_queue.head) {
		if (wakeup_event_queue.head == t)
			wakeup_event_queue.head = wakeup_event_queue.head->next;
		t->prev->next = t->next;
		t->next->prev = t->prev;
	} else
		wakeup_event_queue.head = NULL;
	wakeup_event_queue.len--;

	kern_unlock(&if_bit);

	return 0;
}

int sched_update_wakeupevq(unsigned char event_type)
{
	struct task *t, *next;
	unsigned char if_bit;

	if (!wakeup_event_queue.head)
		return -1;

	kern_lock(&if_bit);

	t = wakeup_event_queue.head;
	do {
		next = t->next;
		if (t->wakeup_after_event == event_type) {
			t->wakeup_after_event = 0;
			sched_wakeupevq_del(t);
			sched_runq_enq(t);
		}
		t = next;
	} while (wakeup_event_queue.head && t != wakeup_event_queue.head);

	kern_unlock(&if_bit);

	return 0;
}

void wakeup_after_event(unsigned char event_type)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	if (current_task->next != current_task)
		dummy_task.next = current_task->next;
	current_task->wakeup_after_event = event_type;
	sched_runq_del(current_task);
	sched_wakeupevq_enq(current_task);
	current_task = &dummy_task;
	schedule(SCHED_CAUSE_SYSCALL);

	kern_unlock(&if_bit);
}
