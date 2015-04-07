#include <console_io.h>
#include <common.h>
#include <shell.h>

#define MAX_LINE_SIZE	256

static void exec_command(const char *command_line)
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

void shell(void)
{
	while (1) {
		char buf[MAX_LINE_SIZE];
		put_str("OS5> ");
		get_line(buf, MAX_LINE_SIZE);
		exec_command(buf);
	}
}
