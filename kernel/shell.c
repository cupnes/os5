#include <console_io.h>
#include <common.h>
#include <shell.h>

#define MAX_LINE_SIZE	512

enum {
	ECHO,
	READB,
	COMMAND_NUM
} _COMMAND_SET;

static int command_echo(char *args)
{
	put_str(args);
	put_str("\r\n");

	return 0;
}

static int command_readb(char *args)
{
	char first[128], other[128];
	unsigned char *addr;

	str_getfirstentry(args, first, other);
	addr = (unsigned char *)str_ahextoint(first);
	dump_hex(*addr, 2);
	put_str("\r\n");

	return 0;
}

static unsigned char get_command_id(const char *command)
{
	if (!str_compare(command, "echo")) {
		return ECHO;
	}

	if (!str_compare(command, "readb")) {
		return READB;
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
		str_getfirstentry(buf, command, args);
		command_id = get_command_id(command);
		switch (command_id) {
		case ECHO:
			command_echo(args);
			break;
		case READB:
			command_readb(args);
			break;
		default:
			put_str("Command not found.\r\n");
			break;
		}
	}
}
