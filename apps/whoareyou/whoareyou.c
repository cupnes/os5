#include <app.h>
#include <kernel.h>
#include <string.h>
#include <console.h>

int main(int argc, char *argv[])
{
	if ((argc >= 2) && !str_compare(argv[1], "-v"))
		put_str("Operating System 5\r\n");
	else
		put_str("OS5\r\n");
	syscall(SYSCALL_EXIT, 0, 0, 0);

	return 0;
}
