#include <uptime_init.h>
#include <cpu.h>
#include <console_io.h>
#include <memory.h>
#include <sched.h>

#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x20002000

struct tss uptime_tss;

void uptime_context_switch(void)
{
	__asm__("ljmp	$0x28, $0");
}
