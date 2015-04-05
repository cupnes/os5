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

void memcpy(void *dst, const void *src, unsigned int size)
{
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;

	for (; size > 0; size--) {
		*d = *s;
		d++;
		s++;
	}
}

int str_findchar(const char *src, char key)
{
	int i;

	for (i = 0; src[i] != key; i++) {
		if (src[i] == '\0') {
			i = -1;
			break;
		}
	}

	return i;
}

int str_getlen(const char *src)
{
	int len;
	for (len = 0; src[len] != '\0'; len++);
	return len + 1;
}

void exec_command(const char *command_line)
{
	char command[256], args[256];
	int line_len, command_len, args_len;

	line_len = str_getlen(command_line);
	command_len = str_findchar(command_line, ' ');
	if (command_len < 0) {
		memcpy((void *)command, (void *)command_line, line_len);
		command_len = line_len;
		args_len = 0;
		args[args_len] = '\0';
	} else {
		memcpy((void *)command, (void *)command_line, command_len);
		command[command_len] = '\0';
		command_len++;
		args_len = line_len - command_len;
		memcpy((void *)args, (void *)(command_line + command_len), args_len);
	}

#ifdef DEBUG
	put_str(command_line);
	put_str("|");
	put_str(command);
	put_str(":");
	put_str(args);
	put_str("\r\n");
#endif /* DEBUG */

	switch (command[0]) {
	case 'e':
		put_str(args);
		put_str("\r\n");
		break;
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

	while (1) {
		char buf[MAX_LINE_SIZE];
		put_str("OS5> ");
		get_line(buf, MAX_LINE_SIZE);
		exec_command(buf);
	}

	return 0;
}
