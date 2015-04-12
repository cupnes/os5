#include <cpu.h>
#include <intr.h>
#include <console_io.h>
#include <shell.h>

int main(void)
{
	unsigned char mask;

	cursor_pos.y += 2;
	update_cursor();

	con_init();
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	start_shell();

	return 0;
}
