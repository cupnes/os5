#include <app.h>
#include <kernel.h>

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
	syscall(SYSCALL_CON_PUT_STR, (unsigned int)"OS5\r\n", 0, 0);
	syscall(SYSCALL_EXIT, 0, 0, 0);

	return 0;
}
