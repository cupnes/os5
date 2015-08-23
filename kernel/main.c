#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <console_io.h>
#include <shell.h>

int main(void)
{
	unsigned char mask;
	unsigned char i;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_NUM; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);

	/* Setup devices */
	con_init();

	/* Setup tasks */
	shell_init();

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
