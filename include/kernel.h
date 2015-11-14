#ifndef _KERNEL_H_
#define _KERNEL_H_

enum {
	SYSCALL_TIMER_GET_GLOBAL_COUNTER = 1,
	SYSCALL_SCHED_WAKEUP_MSEC,
	SYSCALL_CON_GET_CURSOR_POS_Y,
	SYSCALL_CON_PUT_STR,
	SYSCALL_CON_PUT_STR_POS,
	SYSCALL_CON_DUMP_HEX_POS
};

void kern_lock(unsigned char *if_bit);
void kern_unlock(unsigned char *if_bit);

#endif /* _KERNEL_H_ */
