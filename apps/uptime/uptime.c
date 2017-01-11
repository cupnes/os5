#include <app.h>
#include <kernel.h>
#include <console.h>

int main(int argc __attribute__ ((unused)), char *argv[] __attribute__ ((unused)))
{
	static unsigned int uptime;
	unsigned int cursor_pos_y;

	while (1) {
		uptime = get_global_counter() / 1000;
		cursor_pos_y = get_cursor_pos_y();
		if (cursor_pos_y < ROWS) {
			put_str_pos("uptime:", COLUMNS - (7 + 4), 0);
			dump_hex_pos(uptime, 4, COLUMNS - 4, 0);
		} else {
			put_str_pos("uptime:", COLUMNS - (7 + 4), cursor_pos_y - ROWS + 1);
			dump_hex_pos(uptime, 4, COLUMNS - 4, cursor_pos_y - ROWS + 1);
		}
		syscall(SYSCALL_SCHED_WAKEUP_MSEC, 33, 0, 0);
	}
}
