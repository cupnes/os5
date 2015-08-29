#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

struct task {
	struct tss tss;
	void (*context_switch)(void);
};

void schedule(void);

#endif /* _SCHED_H_ */
