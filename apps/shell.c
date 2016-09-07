#include <io_port.h>
#include <kernel.h>

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

static void shell_main(void);

void shell_start(void)
{
	shell_main();
}

static unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	unsigned int result;

	__asm__ (
		"\tint $0x80\n"
	:"=a"(result)
	:"a"(syscall_id), "b"(arg1), "c"(arg2), "d"(arg3));

	return result;
}

static void shell_put_str(char *str)
{
	syscall(SYSCALL_CON_PUT_STR, (unsigned int)str, 0, 0);
}

static void shell_dump_hex(unsigned int val, unsigned int num_digits)
{
	syscall(SYSCALL_CON_DUMP_HEX, val, num_digits, 0);
}

static int pow(int num, int multer)
{
	if (multer == 0) return 1;
	return pow(num, multer - 1) * num;
}

static void copy_mem(const void *src, void *dst, unsigned int size)
{
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;

	for (; size > 0; size--) {
		*d = *s;
		d++;
		s++;
	}
}

static int str_get_len(const char *src)
{
	int len;
	for (len = 0; src[len] != '\0'; len++);
	return len + 1;
}

static int str_find_char(const char *src, char key)
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

static void str_get_first_entry(const char *line, char *first, char *other)
{
	int line_len, first_len, other_len;

	line_len = str_get_len(line);
	first_len = str_find_char(line, ' ');
	if (first_len < 0) {
		copy_mem((void *)line, (void *)first, line_len);
		first_len = line_len;
		other_len = 0;
		other[other_len] = '\0';
	} else {
		copy_mem((void *)line, (void *)first, first_len);
		first[first_len] = '\0';
		first_len++;
		other_len = line_len - first_len;
		copy_mem((void *)(line + first_len), (void *)other, other_len);
	}

#ifdef DEBUG
	shell_put_str(line);
	shell_put_str("|");
	shell_put_str(first);
	shell_put_str(":");
	shell_put_str(other);
	shell_put_str("\r\n");
#endif /* DEBUG */
}

static int str_conv_ahex_int(const char *hex_str)
{
	int len = str_get_len(hex_str);
	int val = 0, i;

	for (i = 0; hex_str[i] != '\0'; i++) {
		if (('0' <= hex_str[i]) && (hex_str[i] <= '9')) {
			val += (hex_str[i] - '0') * pow(16, len - 2 - i);
		} else {
			val += (hex_str[i] - 'a' + 10) * pow(16, len - 2 - i);
		}
	}

	return val;
}

static int str_compare(const char *src, const char *dst)
{
	char is_equal = 1;

	for (; (*src != '\0') && (*dst != '\0'); src++, dst++) {
		if (*src != *dst) {
			is_equal = 0;
			break;
		}
	}

	if (is_equal) {
		if (*src != '\0') {
			return 1;
		} else if (*dst != '\0') {
			return -1;
		} else {
			return 0;
		}
	} else {
		return (int)(*src - *dst);
	}
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

static void shell_main(void)
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
				syscall(SYSCALL_EXEC, fp, 0, 0);
				if (!is_background)
					syscall(SYSCALL_SCHED_WAKEUP_EVENT, EVENT_TYPE_EXIT, 0, 0);
			} else
				shell_put_str("Command not found.\r\n");
			break;
		}
	}
}
