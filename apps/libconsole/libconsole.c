#include <console.h>
#include <kernel.h>

unsigned int get_cursor_pos_y(void)
{
	return syscall(SYSCALL_CON_GET_CURSOR_POS_Y, 0, 0, 0);
}

void put_str(char *str)
{
	syscall(SYSCALL_CON_PUT_STR, (unsigned int)str, 0, 0);
}

void put_str_pos(char *str, unsigned char x, unsigned char y)
{
	syscall(SYSCALL_CON_PUT_STR_POS, (unsigned int)str, (unsigned int)x, (unsigned int)y);
}

void dump_hex(unsigned int val, unsigned int num_digits)
{
	syscall(SYSCALL_CON_DUMP_HEX, val, num_digits, 0);
}

void dump_hex_pos(unsigned int val, unsigned int num_digits, unsigned char x, unsigned char y)
{
	syscall(SYSCALL_CON_DUMP_HEX_POS, val, num_digits, (unsigned int)((x << 16) | y));
}

unsigned int get_line(char *buf, unsigned int buf_size)
{
	return syscall(SYSCALL_CON_GET_LINE, (unsigned int)buf, buf_size, 0);
}
