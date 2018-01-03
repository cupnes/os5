#include <cpu.h>
#include <intr.h>
#include <io_port.h>

#define MAX_IDT	256

#ifdef X86_64
unsigned long long idt[MAX_IDT * 2];
unsigned long long idtr[2];
#endif

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

#ifdef X86_64
	idtr[0] = ((unsigned long long)idt << 16) | (MAX_IDT * 16 - 1);
	idtr[1] = ((unsigned long long)idt >> 48);
	__asm__ ("lidt idtr");
#endif
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

#ifndef X86_64
void intr_set_handler(unsigned char intr_num, unsigned int handler_addr)
{
	extern unsigned char idt;
	unsigned int intr_dscr_top_half, intr_dscr_bottom_half;
	unsigned int *idt_ptr;

	idt_ptr = (unsigned int *)&idt;
	intr_dscr_bottom_half = handler_addr;
	intr_dscr_top_half = 0x00080000;
	intr_dscr_top_half = (intr_dscr_top_half & 0xffff0000)
		| (intr_dscr_bottom_half & 0x0000ffff);
	intr_dscr_bottom_half = (intr_dscr_bottom_half & 0xffff0000) | 0x00008e00;
	if (intr_num == INTR_NUM_USER128)
		intr_dscr_bottom_half |= 3 << 13;
	idt_ptr += intr_num * 2;
	*idt_ptr = intr_dscr_top_half;
	*(idt_ptr + 1) = intr_dscr_bottom_half;
}
#else
void intr_set_handler(unsigned char intr_num, unsigned long long handler_addr)
{
	unsigned int intr_dscr[3];
	unsigned int *idt_ptr;
	unsigned char i;

	idt_ptr = (unsigned int *)idt;

	intr_dscr[0] = (SS_KERNEL_CODE << 16) | (handler_addr & 0x0000ffff);
	intr_dscr[1] = (handler_addr & 0xffff0000) | 0x00008e00;
	intr_dscr[2] = handler_addr >> 32;

	idt_ptr += intr_num * 4;
	for (i = 0; i < 3; i++)
		*idt_ptr++ = intr_dscr[i];
}
#endif
