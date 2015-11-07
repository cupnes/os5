#include <uptime.h>
#include <timer.h>
#include <console_io.h>
#include <sched.h>

void uptime_start(void)
{
	static unsigned int uptime;

	while (1) {
		uptime = global_counter / 100;
		if (cursor_pos.y < ROWS) {
			put_str_pos("uptime:", COLUMNS - (7 + 4), 0);
			dump_hex_pos(uptime, 4, COLUMNS - 4, 0);
		} else {
			put_str_pos("uptime:", COLUMNS - (7 + 4), cursor_pos.y - ROWS + 1);
			dump_hex_pos(uptime, 4, COLUMNS - 4, cursor_pos.y - ROWS + 1);
		}
		wakeup_after_msec(1000);
	}
}
