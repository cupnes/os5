#include <cpu.h>
#include <intr.h>
#include <console_io.h>
#include <shell.h>

int main(void)
{
	unsigned char mask;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup devices */
	con_init();

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	/* Start main task */
	shell_start();

	return 0;
}
