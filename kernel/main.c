#include <cpu.h>
#include <intr.h>
#include <console_io.h>

int main(void)
{
	unsigned char mask;

	move_cursor_rel(0, 2);

	con_init();
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	put_str("Hello OS5:main()\r\n");

	while (1) {
		char tmp = get_char();
		put_char(tmp);
		if (tmp == '\n') put_char('\r');
	}

	return 0;
}
