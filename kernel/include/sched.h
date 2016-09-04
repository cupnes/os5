#ifndef _SCHED_H_
#define _SCHED_H_

#include <cpu.h>
#include <task.h>

#define TASK_NUM	3

extern struct task task_instance_table[TASK_NUM];
extern struct task *current_task;

unsigned short sched_get_current(void);
int sched_runq_enq(struct task *t);
void schedule(void);
int sched_update_wakeupq(void);
void wakeup_after_msec(unsigned int msec);
int sched_update_wakeupevq(unsigned char event_type);
void wakeup_after_event(unsigned char event_type);

#endif /* _SCHED_H_ */
