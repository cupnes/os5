#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	2

struct task {
	struct tss tss;
	unsigned short task_id;
	void (*context_switch)(void);
	struct task *next;
};

extern struct task task_list[TASK_NUM];

unsigned short sched_get_current(void);
void schedule(void);

#endif /* _SCHED_H_ */
