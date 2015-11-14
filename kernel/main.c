#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <memory.h>
#include <console_io.h>
#include <timer.h>
#include <kernel.h>
#include <sched.h>
#include <kern_task.h>
#include <shell.h>
#include <uptime.h>

#define INTR_NUM_USER128	0x80

void kern_lock(unsigned char *if_bit)
{
	/* Save EFlags.IF */
	*if_bit = (x86_get_eflags() & X86_EFLAGS_IF) ? 1 : 0;

	/* if saved IF == true, then cli */
	if (*if_bit)
		cli();
}

void kern_unlock(unsigned char *if_bit)
{
	/* if saved IF == true, then sti */
	if (*if_bit)
		sti();
}

unsigned int do_syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	switch (syscall_id) {
	case SYSCALL_TIMER_GET_GLOBAL_COUNTER:
		return timer_get_global_counter();
		break;
	case SYSCALL_SCHED_WAKEUP_MSEC:
		wakeup_after_msec(arg1);
		return 0;
	case SYSCALL_CON_GET_CURSOR_POS_Y:
		return (unsigned int)cursor_pos.y;
	case SYSCALL_CON_PUT_STR_POS:
		put_str_pos((char *)arg1, (unsigned char)arg2, (unsigned char)arg3);
		return 0;
	}

	return -1;
}

int main(void)
{
	extern unsigned char syscall_handler;

	unsigned char mask;
	unsigned char i;

	/* Setup console */
	cursor_pos.y += 2;
	update_cursor();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_MAX; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);
	intr_set_handler(EXCP_NUM_GP, (unsigned int)&general_protection_handler);
	intr_set_handler(EXCP_NUM_PF, (unsigned int)&page_fault_handler);

	/* Setup devices */
	con_init();
	timer_init();
	mem_init();

	/* Setup tasks */
	kern_task_init();
	shell_init();
	uptime_init();

	/* Start paging */
	mem_page_start();

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_set_handler(INTR_NUM_USER128, (unsigned int)&syscall_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* End of kernel initialization process */
	while (1) {
		x86_halt();
	}

	return 0;
}
