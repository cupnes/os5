#include <intr.h>
#include <io_port.h>

void intr_set_mask_master(unsigned char mask)
{
	outb(mask, IOADR_MPIC_OCW1);
}

unsigned char intr_get_mask_master(void)
{
	return inb(IOADR_MPIC_OCW1);
}

void intr_set_mask_slave(unsigned char mask)
{
	outb(mask, IOADR_SPIC_OCW1);
}

unsigned char intr_get_mask_slave(void)
{
	return inb(IOADR_SPIC_OCW1);
}
