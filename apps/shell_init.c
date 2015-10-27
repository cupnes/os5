#include <shell.h>
#include <cpu.h>
#include <console_io.h>
#include <sched.h>

#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x20002000

struct tss shell_tss;

void shell_context_switch(void)
{
	__asm__("ljmp	$0x18, $0");
}

void shell_init(void)
{
	/* Setup context switch function */
	run_queue[2].context_switch = shell_context_switch;

	/* Setup GDT for uptime_tss */
	init_gdt(SHELL_GDT_IDX, (unsigned int)&shell_tss, sizeof(shell_tss));

	/* Setup uptime_tss */
	shell_tss.eip = APP_ENTRY_POINT;
	shell_tss.esp = APP_STACK_BASE;
	shell_tss.eflags = 0x00000200;
	shell_tss.es = 0x0010;
	shell_tss.cs = 0x0008;
	shell_tss.ss = 0x0010;
	shell_tss.ds = 0x0010;
	shell_tss.fs = 0x0010;
	shell_tss.gs = 0x0010;
	shell_tss.__cr3 = 0x00091018;
}
