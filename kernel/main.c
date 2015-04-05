#include <cpu.h>
#include <intr.h>
#include <console_io.h>

#define MAX_LINE_SIZE	256

void get_line(char *buf, unsigned int buf_size)
{
	unsigned int i;

	for (i = 0; i < buf_size - 1; i++) {
		buf[i] = get_char();
		put_char(buf[i]);
		if (buf[i] == '\n') {
			put_char('\r');
			break;
		}
	}
	buf[i] = '\0';
}

void exec_command(const char *command_line)
{
	switch (*command_line) {
	default:
		put_str("Command not found.\r\n");
		break;
	}
}

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
		char buf[MAX_LINE_SIZE];
		put_str("OS5> ");
		get_line(buf, MAX_LINE_SIZE);
		exec_command(buf);
	}

	return 0;
}
