#include <cpu.h>
#include <io_port.h>
#include <console_io.h>

int main(void)
{
	outb(0xfd, IOADR_MPIC_OCW1);
	sti();

	while (1);

	return 0;
}
