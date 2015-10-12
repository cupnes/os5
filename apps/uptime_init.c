#include <uptime.h>
#include <cpu.h>
#include <console_io.h>
#include <sched.h>

#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x90000000

struct tss uptime_tss;

void uptime_context_switch(void)
{
	__asm__("ljmp	$0x20, $0");
}

void uptime_init(void)
{
	/* Setup context switch function */
	run_queue[UPTIME_ID].context_switch = uptime_context_switch;

	/* Setup GDT for uptime_tss */
	init_gdt(UPTIME_GDT_IDX, (unsigned int)&uptime_tss, sizeof(uptime_tss));

	/* Setup uptime_tss */
	uptime_tss.eip = APP_ENTRY_POINT;
	uptime_tss.esp = APP_STACK_BASE;
	uptime_tss.eflags = 0x00000200;
	uptime_tss.es = 0x0010;
	uptime_tss.cs = 0x0008;
	uptime_tss.ss = 0x0010;
	uptime_tss.ds = 0x0010;
	uptime_tss.fs = 0x0010;
	uptime_tss.gs = 0x0010;
	uptime_tss.__cr3 = 0x00090018;
}
