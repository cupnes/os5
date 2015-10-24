#include <shell.h>
#include <cpu.h>
#include <console_io.h>
#include <sched.h>

struct tss shell_tss;

void shell_context_switch(void)
{
	__asm__("ljmp	$0x18, $0");
}

void shell_init(void)
{
	unsigned short segment_selector = 8 * SHELL_GDT_IDX;
	unsigned int old_cr3, cr3 = 0x00091018;

	/* Setup context switch function */
	run_queue[SHELL_ID].context_switch = shell_context_switch;

	/* Setup GDT for shell_tss */
	shell_tss.__cr3 = 0x00091018;
	init_gdt(SHELL_GDT_IDX, (unsigned int)&shell_tss, sizeof(shell_tss));

	/* Setup CR3(PDBR) */
	__asm__("movl	%%cr3, %0":"=r"(old_cr3):);
	cr3 |= old_cr3 & 0x00000fe7;
	__asm__("movl	%0, %%cr3"::"r"(cr3));

	/* Setup Task Register */
	__asm__("ltr %0"::"r"(segment_selector));
	put_str("task loaded.\r\n");
}
