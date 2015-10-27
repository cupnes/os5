#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	3

struct task {
	struct tss tss;
	void (*context_switch)(void);
};

extern struct task run_queue[TASK_NUM];

void schedule(void);

#endif /* _SCHED_H_ */
