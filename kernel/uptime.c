#include <uptime.h>
#include <cpu.h>
#include <timer.h>
#include <console_io.h>

struct tss uptime_tss;

void init_uptime(void)
{
	/* Setup GDT for uptime_tss */
	init_gdt(4, (unsigned int)&uptime_tss, sizeof(uptime_tss));

	/* Setup uptime_tss */
	uptime_tss.eip = (unsigned int)uptime;
	uptime_tss.esp = 0x00085000;
	uptime_tss.eflags = 0x00000200;
	uptime_tss.es = 0x0010;
	uptime_tss.cs = 0x0008;
	uptime_tss.ss = 0x0010;
	uptime_tss.ds = 0x0010;
	uptime_tss.fs = 0x0010;
	uptime_tss.gs = 0x0010;
}

void uptime(void)
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
