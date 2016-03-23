#ifndef _TASK_H_
#define _TASK_H_

#include <cpu.h>
#include <fs.h>

#define CONTEXT_SWITCH_FN_SIZE	12
#define CONTEXT_SWITCH_FN_TSKNO_FIELD	8

struct task {
	/* ランキュー・ウェイクアップキュー(時間経過待ち・イベント待ち)の
	 * いずれかに繋がれる(同時に複数のキューに存在することが無いよう
	 * 運用する) */
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

extern unsigned char context_switch_template[CONTEXT_SWITCH_FN_SIZE];

void task_init(struct file *f);

#endif /* _TASK_H_ */
