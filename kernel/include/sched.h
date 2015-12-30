#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>

#define TASK_NUM	3
#define CONTEXT_SWITCH_FN_SIZE	12
#define CONTEXT_SWITCH_FN_TSKNO_FIELD	8

enum {
	SCHED_CAUSE_TIMER,
	SCHED_CAUSE_SYSCALL
};

struct task {
	struct task *prev;
	struct task *next;
	unsigned short task_id;
	struct tss tss;
	void (*context_switch)(void);
	unsigned char context_switch_func[CONTEXT_SWITCH_FN_SIZE];
	char task_switched_in_time_slice;
	unsigned int wakeup_after_msec;
	unsigned char wakeup_after_event;
};

extern struct task task_instance_table[TASK_NUM];
extern unsigned char context_switch_template[CONTEXT_SWITCH_FN_SIZE];

unsigned short sched_get_current(void);
int sched_runq_enq(struct task *t);
void schedule(unsigned char cause_id);
int sched_update_wakeupq(void);
void wakeup_after_msec(unsigned int msec);
int sched_update_wakeupevq(unsigned char event_type);
void wakeup_after_event(unsigned char event_type);

#endif /* _SCHED_H_ */
