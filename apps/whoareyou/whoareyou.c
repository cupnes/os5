#include <app.h>
#include "../../kernel/include/kernel.h"

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

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	unsigned int result;

	__asm__ (
		"\tint $0x80\n"
	:"=a"(result)
	:"a"(syscall_id), "b"(arg1), "c"(arg2), "d"(arg3));

	return result;
}

int main(int argc, char *argv[])
{
	if ((argc >= 2) && !str_compare(argv[1], "-v"))
		syscall(SYSCALL_CON_PUT_STR, (unsigned int)"Operating System 5\r\n", 0, 0);
	else
		syscall(SYSCALL_CON_PUT_STR, (unsigned int)"OS5\r\n", 0, 0);
	syscall(SYSCALL_EXIT, 0, 0, 0);

	return 0;
}
