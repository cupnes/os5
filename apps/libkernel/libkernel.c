#include <kernel.h>

unsigned int syscall(unsigned int syscall_id, unsigned int arg1,
		     unsigned int arg2, unsigned int arg3)
{
	unsigned int result;

	__asm__ (
		"\tint $0x80\n"
	:"=a"(result)
	:"a"(syscall_id), "b"(arg1), "c"(arg2), "d"(arg3));

	return result;
}

unsigned int get_global_counter(void)
{
	return syscall(SYSCALL_TIMER_GET_GLOBAL_COUNTER, 0, 0, 0);
}

void exit(void)
{
	syscall(SYSCALL_EXIT, 0, 0, 0);
}
