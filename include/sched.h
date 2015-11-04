#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	2

struct task {
	struct task *next;
	unsigned short task_id;
	void (*context_switch)(void);
};

extern struct task task_list[TASK_NUM];

void sched_init(void);
unsigned short sched_get_current(void);
int sched_runq_enq(struct task *t);
void schedule(void);

#endif /* _SCHED_H_ */
