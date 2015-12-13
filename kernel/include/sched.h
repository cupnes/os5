#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	3

enum {
	SCHED_CAUSE_TIMER,
	SCHED_CAUSE_SYSCALL
};

struct task {
	struct task *prev;
	struct task *next;
	unsigned short task_id;
	void (*context_switch)(void);
	char task_switched_in_time_slice;
	unsigned int wakeup_after_msec;
	unsigned char wakeup_after_event;
};

extern struct task task_list[TASK_NUM];

unsigned short sched_get_current(void);
int sched_runq_enq(struct task *t);
void schedule(unsigned char cause_id);
int sched_update_wakeupq(void);
void wakeup_after_msec(unsigned int msec);
int sched_update_wakeupevq(unsigned char event_type);
void wakeup_after_event(unsigned char event_type);

#endif /* _SCHED_H_ */
