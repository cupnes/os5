#include <app.h>
#include <kernel.h>
#include <common.h>
#include <string.h>

#define MAX_LINE_SIZE	512

enum {
	ECHO,
	READB,
	READW,
	READL,
	IOREADB,
	WRITEB,
	WRITEW,
	WRITEL,
	IOWRITEB,
	BG,
	TEST,
	COMMAND_NUM
} _COMMAND_SET;

static void shell_put_str(char *str)
{
	syscall(SYSCALL_CON_PUT_STR, (unsigned int)str, 0, 0);
}

static void shell_dump_hex(unsigned int val, unsigned int num_digits)
{
	syscall(SYSCALL_CON_DUMP_HEX, val, num_digits, 0);
}

static int command_echo(char *args)
{
	shell_put_str(args);
	shell_put_str("\r\n");

	return 0;
}

static int command_readb(char *args)
{
	char first[128], other[128];
	unsigned char *addr;

	str_get_first_entry(args, first, other);
	addr = (unsigned char *)str_conv_ahex_int(first);
	shell_dump_hex(*addr, 2);
	shell_put_str("\r\n");

	return 0;
}

static int command_readw(char *args)
{
	char first[128], other[128];
	unsigned short *addr;

	str_get_first_entry(args, first, other);
	addr = (unsigned short *)str_conv_ahex_int(first);
	shell_dump_hex(*addr, 4);
	shell_put_str("\r\n");

	return 0;
}

static int command_readl(char *args)
{
	char first[128], other[128];
	unsigned int *addr;

	str_get_first_entry(args, first, other);
	addr = (unsigned int *)str_conv_ahex_int(first);
	shell_dump_hex(*addr, 8);
	shell_put_str("\r\n");

	return 0;
}

static int command_ioreadb(char *args)
{
	char first[128], other[128];
	unsigned short addr;

	str_get_first_entry(args, first, other);
	addr = (unsigned short)str_conv_ahex_int(first);
	shell_dump_hex(inb_p(addr), 2);
	shell_put_str("\r\n");

	return 0;
}

static int command_writeb(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned char data, *addr;

	str_get_first_entry(args, first, other);
	str_get_first_entry(other, second, _other);
	data = (unsigned char)str_conv_ahex_int(first);
	addr = (unsigned char *)str_conv_ahex_int(second);
	*addr = data;

	return 0;
}

static int command_writew(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned short data, *addr;

	str_get_first_entry(args, first, other);
	str_get_first_entry(other, second, _other);
	data = (unsigned short)str_conv_ahex_int(first);
	addr = (unsigned short *)str_conv_ahex_int(second);
	*addr = data;

	return 0;
}

static int command_writel(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned int data, *addr;

	str_get_first_entry(args, first, other);
	str_get_first_entry(other, second, _other);
	data = (unsigned int)str_conv_ahex_int(first);
	addr = (unsigned int *)str_conv_ahex_int(second);
	*addr = data;

	return 0;
}

static int command_iowriteb(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned char data;
	unsigned short addr;

	str_get_first_entry(args, first, other);
	str_get_first_entry(other, second, _other);
	data = (unsigned char)str_conv_ahex_int(first);
	addr = (unsigned short)str_conv_ahex_int(second);
	outb_p(data, addr);

	return 0;
}

static int command_test(char *args)
{
	shell_put_str("test\r\n");

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

	if (!str_compare(command, "readw")) {
		return READW;
	}

	if (!str_compare(command, "readl")) {
		return READL;
	}

	if (!str_compare(command, "ioreadb")) {
		return IOREADB;
	}

	if (!str_compare(command, "writeb")) {
		return WRITEB;
	}

	if (!str_compare(command, "writew")) {
		return WRITEW;
	}

	if (!str_compare(command, "writel")) {
		return WRITEL;
	}

	if (!str_compare(command, "iowriteb")) {
		return IOWRITEB;
	}

	if (!str_compare(command, "bg")) {
		return BG;
	}

	if (!str_compare(command, "test")) {
		return TEST;
	}

	return COMMAND_NUM;
}

int main(int argc, char *argv[])
{
	while (1) {
		char buf[MAX_LINE_SIZE];
		char command[256], args[256];
		unsigned char command_id, is_background = 0;
		unsigned int fp;

		shell_put_str("OS5> ");
		if (syscall(SYSCALL_CON_GET_LINE, (unsigned int)buf, MAX_LINE_SIZE, 0) <= 0) {
			continue;
		}

		while (1) {
			str_get_first_entry(buf, command, args);
			command_id = get_command_id(command);
			if (command_id != BG)
				break;
			else {
				is_background = 1;
				copy_mem(args, buf, (unsigned int)str_get_len(args));
			}
		}

		switch (command_id) {
		case ECHO:
			command_echo(args);
			break;
		case READB:
			command_readb(args);
			break;
		case READW:
			command_readw(args);
			break;
		case READL:
			command_readl(args);
			break;
		case IOREADB:
			command_ioreadb(args);
			break;
		case WRITEB:
			command_writeb(args);
			break;
		case WRITEW:
			command_writew(args);
			break;
		case WRITEL:
			command_writel(args);
			break;
		case IOWRITEB:
			command_iowriteb(args);
			break;
		case TEST:
			command_test(args);
			break;
		default:
			fp = syscall(SYSCALL_OPEN, (unsigned int)command, 0, 0);
			if (fp) {
				unsigned int argc = 0, i;
				char *argv[256];
				char *start;

				argv[argc++] = command;

				start = &args[0];
				for (i = 0; ; i++) {
					if ((i == 0) && (args[i] == '\0')) {
						break;
					} else if ((args[i] == ' ') || (args[i] == '\0')) {
						argv[argc++] = start;
						start = &args[i + 1];
						if (args[i] == ' ')
							args[i] = '\0';
						else
							break;
					}
				}

				syscall(SYSCALL_EXEC, fp, argc, (unsigned int)argv);

				if (!is_background)
					syscall(SYSCALL_SCHED_WAKEUP_EVENT, EVENT_TYPE_EXIT, 0, 0);
			} else
				shell_put_str("Command not found.\r\n");
			break;
		}
	}
}
