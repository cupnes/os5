#include <io_port.h>
#include <console_io.h>
#include <common.h>
#include <shell.h>

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

static int command_readw(char *args)
{
	char first[128], other[128];
	unsigned short *addr;

	str_getfirstentry(args, first, other);
	addr = (unsigned short *)str_ahextoint(first);
	dump_hex(*addr, 4);
	put_str("\r\n");

	return 0;
}

static int command_readl(char *args)
{
	char first[128], other[128];
	unsigned int *addr;

	str_getfirstentry(args, first, other);
	addr = (unsigned int *)str_ahextoint(first);
	dump_hex(*addr, 8);
	put_str("\r\n");

	return 0;
}

static int command_ioreadb(char *args)
{
	char first[128], other[128];
	unsigned short addr;

	str_getfirstentry(args, first, other);
	addr = (unsigned short)str_ahextoint(first);
	dump_hex(inb_p(addr), 2);
	put_str("\r\n");

	return 0;
}

static int command_writeb(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned char data, *addr;

	str_getfirstentry(args, first, other);
	str_getfirstentry(other, second, _other);
	data = (unsigned char)str_ahextoint(first);
	addr = (unsigned char *)str_ahextoint(second);
	*addr = data;

	return 0;
}

static int command_writew(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned short data, *addr;

	str_getfirstentry(args, first, other);
	str_getfirstentry(other, second, _other);
	data = (unsigned short)str_ahextoint(first);
	addr = (unsigned short *)str_ahextoint(second);
	*addr = data;

	return 0;
}

static int command_writel(char *args)
{
	char first[16], second[32], other[128], _other[128];
	unsigned int data, *addr;

	str_getfirstentry(args, first, other);
	str_getfirstentry(other, second, _other);
	data = (unsigned int)str_ahextoint(first);
	addr = (unsigned int *)str_ahextoint(second);
	*addr = data;

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
		default:
			put_str("Command not found.\r\n");
			break;
		}
	}
}
