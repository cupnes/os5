#include <app.h>
#include <kernel.h>

int main(int argc, char *argv[])
{
	static unsigned int uptime;
	unsigned int cursor_pos_y;

	while (1) {
		uptime = syscall(SYSCALL_TIMER_GET_GLOBAL_COUNTER, 0, 0, 0) / 1000;
		cursor_pos_y = syscall(SYSCALL_CON_GET_CURSOR_POS_Y, 0, 0, 0);
		if (cursor_pos_y < ROWS) {
			syscall(SYSCALL_CON_PUT_STR_POS, (unsigned int)"uptime:", COLUMNS - (7 + 4), 0);
			syscall(SYSCALL_CON_DUMP_HEX_POS, uptime, 4, ((COLUMNS - 4) << 16) | 0);
		} else {
			syscall(SYSCALL_CON_PUT_STR_POS, (unsigned int)"uptime:", COLUMNS - (7 + 4), cursor_pos_y - ROWS + 1);
			syscall(SYSCALL_CON_DUMP_HEX_POS, uptime, 4, ((COLUMNS - 4) << 16) | (cursor_pos_y - ROWS + 1));
		}
		syscall(SYSCALL_SCHED_WAKEUP_MSEC, 33, 0, 0);
	}
}
