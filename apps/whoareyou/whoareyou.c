#include <app.h>
#include <kernel.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if ((argc >= 2) && !str_compare(argv[1], "-v"))
		syscall(SYSCALL_CON_PUT_STR, (unsigned int)"Operating System 5\r\n", 0, 0);
	else
		syscall(SYSCALL_CON_PUT_STR, (unsigned int)"OS5\r\n", 0, 0);
	syscall(SYSCALL_EXIT, 0, 0, 0);

	return 0;
}
