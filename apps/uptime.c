#include <uptime.h>
#include <timer.h>
#include <console_io.h>
#include <kernel.h>
#include <sched.h>

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3);

void uptime_start(void)
{
	static unsigned int uptime;
	unsigned int cursor_pos_y;

	while (1) {
		uptime = syscall(SYSCALL_TIMER_GET_GLOBAL_COUNTER, 0, 0, 0) / 100;
		cursor_pos_y = syscall(SYSCALL_CON_GET_CURSOR_POS_Y, 0, 0, 0);
		if (cursor_pos_y < ROWS) {
			syscall(SYSCALL_CON_PUT_STR_POS, (unsigned int)"uptime:", COLUMNS - (7 + 4), 0);
			dump_hex_pos(uptime, 4, COLUMNS - 4, 0);
		} else {
			syscall(SYSCALL_CON_PUT_STR_POS, (unsigned int)"uptime:", COLUMNS - (7 + 4), cursor_pos_y - ROWS + 1);
			dump_hex_pos(uptime, 4, COLUMNS - 4, cursor_pos_y - ROWS + 1);
		}
		syscall(SYSCALL_SCHED_WAKEUP_MSEC, 33, 0, 0);
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
