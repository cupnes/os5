#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	3

struct task {
	struct task *prev;
	struct task *next;
	unsigned short task_id;
	void (*context_switch)(void);
	unsigned int wakeup_after_msec;
};

extern struct task task_list[TASK_NUM];

unsigned short sched_get_current(void);
int sched_runq_enq(struct task *t);
void schedule(void);
void wakeup_after_msec(unsigned int msec);

#endif /* _SCHED_H_ */
