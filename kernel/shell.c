#include <console_io.h>
#include <common.h>
#include <shell.h>

#define MAX_LINE_SIZE	512

enum {
	ECHO,
	COMMAND_NUM
} _COMMAND_SET;

static int command_echo(char *args)
{
	put_str(args);
	put_str("\r\n");

	return 0;
}

static void parse_line(const char *line, char *command, char *args)
{
	int line_len, command_len, args_len;

	line_len = str_getlen(line);
	command_len = str_findchar(line, ' ');
	if (command_len < 0) {
		memcpy((void *)command, (void *)line, line_len);
		command_len = line_len;
		args_len = 0;
		args[args_len] = '\0';
	} else {
		memcpy((void *)command, (void *)line, command_len);
		command[command_len] = '\0';
		command_len++;
		args_len = line_len - command_len;
		memcpy((void *)args, (void *)(line + command_len), args_len);
	}

#ifdef DEBUG
	put_str(line);
	put_str("|");
	put_str(command);
	put_str(":");
	put_str(args);
	put_str("\r\n");
#endif /* DEBUG */
}

static unsigned char get_command_id(const char *command)
{
	if (!str_compare(command, "echo")) {
		return ECHO;
	}

	return COMMAND_NUM;
}

void shell(void)
{
	while (1) {
		char buf[MAX_LINE_SIZE];
		char command[256], args[256];
		unsigned char command_id;

		put_str("OS5> ");
		get_line(buf, MAX_LINE_SIZE);
		parse_line(buf, command, args);
		command_id = get_command_id(command);
		switch (command_id) {
		case ECHO:
			command_echo(args);
			break;
		default:
			put_str("Command not found.\r\n");
			break;
		}
	}
}
