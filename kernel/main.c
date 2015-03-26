#include <cpu.h>
#include <intr.h>
#include <console_io.h>

int main(void)
{
	unsigned char mask;

	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	while (1);

	return 0;
}
