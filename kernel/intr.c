#include <intr.h>
#include <io_port.h>

void intr_init(void)
{
	/* マスタPICの初期化 */
	outb_p(0x11, IOADR_MPIC_ICW1);
	outb_p(0x20, IOADR_MPIC_ICW2);
	outb_p(0x04, IOADR_MPIC_ICW3);
	outb_p(0x01, IOADR_MPIC_ICW4);
	outb_p(0xff, IOADR_MPIC_OCW1);

	/* スレーブPICの初期化 */
	outb_p(0x11, IOADR_SPIC_ICW1);
	outb_p(0x28, IOADR_SPIC_ICW2);
	outb_p(0x02, IOADR_SPIC_ICW3);
	outb_p(0x01, IOADR_SPIC_ICW4);
	outb_p(0xff, IOADR_SPIC_OCW1);
}

void intr_set_mask_master(unsigned char mask)
{
	outb_p(mask, IOADR_MPIC_OCW1);
}

unsigned char intr_get_mask_master(void)
{
	return inb_p(IOADR_MPIC_OCW1);
}

void intr_set_mask_slave(unsigned char mask)
{
	outb_p(mask, IOADR_SPIC_OCW1);
}

unsigned char intr_get_mask_slave(void)
{
	return inb_p(IOADR_SPIC_OCW1);
}
