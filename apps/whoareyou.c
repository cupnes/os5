#include <kernel.h>

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3);

void entry(void)
{
	syscall(SYSCALL_CON_PUT_STR, (unsigned int)"OS5\r\n", 0, 0);
	while (1)
		syscall(SYSCALL_SCHED_WAKEUP_MSEC, 1000, 0, 0);
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
