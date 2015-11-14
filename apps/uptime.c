#include <uptime.h>
#include <timer.h>
#include <console_io.h>
#include <kernel.h>
#include <sched.h>

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3);

void uptime_start(void)
{
	static unsigned int uptime;

	while (1) {
		uptime = syscall(SYSCALL_TIMER_GET_GLOBAL_COUNTER, 0, 0, 0) / 100;
		if (cursor_pos.y < ROWS) {
			put_str_pos("uptime:", COLUMNS - (7 + 4), 0);
			dump_hex_pos(uptime, 4, COLUMNS - 4, 0);
		} else {
			put_str_pos("uptime:", COLUMNS - (7 + 4), cursor_pos.y - ROWS + 1);
			dump_hex_pos(uptime, 4, COLUMNS - 4, cursor_pos.y - ROWS + 1);
		}
		wakeup_after_msec(33);
	}
}

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	unsigned int result;

	__asm__ (
		"\tint $0x80\n"
	:"=a"(result)
	:"a"(syscall_id), "b"(arg1), "c"(arg2), "d"(arg3));

	return result;
}
