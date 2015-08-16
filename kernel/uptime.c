#include <uptime.h>
#include <cpu.h>
#include <timer.h>
#include <console_io.h>

struct tss task1_tss;

void task1(void)
{
	static unsigned int uptime;
	volatile unsigned int cnt;
	while (1) {
		uptime = global_counter / 100;
		if (cursor_pos.y < ROWS) {
			put_str_pos("uptime:", COLUMNS - (7 + 4), 0);
			dump_hex_pos(uptime, 4, COLUMNS - 4, 0);
		} else {
			put_str_pos("uptime:", COLUMNS - (7 + 4), cursor_pos.y - ROWS + 1);
			dump_hex_pos(uptime, 4, COLUMNS - 4, cursor_pos.y - ROWS + 1);
		}
		for (cnt = 0; cnt < 1000000; cnt++);
	}
}
