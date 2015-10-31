#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <memory.h>
#include <console_io.h>
#include <timer.h>
#include <shell.h>
#include <uptime.h>

#include <sched.h>

#define KERN_TASK_GDT_IDX	5

static void kern_task_context_switch(void)
{
	__asm__("ljmp	$0x28, $0");
}

int main(void)
{
	unsigned char mask;
	unsigned char i;

	struct tss kern_task_tss;
	unsigned int old_cr3, cr3 = 0x0008f018;
	unsigned short segment_selector = 8 * KERN_TASK_GDT_IDX;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

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
	/* kernel task init */
	kern_task_tss.__cr3 = 0x0008f018;
	init_gdt(KERN_TASK_GDT_IDX, (unsigned int)&kern_task_tss, sizeof(kern_task_tss));
	__asm__("movl	%%cr3, %0":"=r"(old_cr3):);
	cr3 |= old_cr3 & 0x00000fe7;
	__asm__("movl	%0, %%cr3"::"r"(cr3));
	__asm__("ltr %0"::"r"(segment_selector));
	put_str("task loaded.\r\n");

	shell_init();
	cli();
	uptime_init();

	/* Start paging */
	mem_page_start();

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* End of kernel initialization process */
	while (1);

	return 0;
}
