#include <cpu.h>
#include <intr.h>
#include <console_io.h>

int main(void)
{
	unsigned char mask;
	extern unsigned char keyboard_handler;
	extern unsigned char idt;
	unsigned int intr_dscr_top_half, intr_dscr_bottom_half;
	unsigned int *idt_ptr;

	idt_ptr = (unsigned int *)&idt;
	intr_dscr_bottom_half = (unsigned int)&keyboard_handler;
	intr_dscr_top_half = 0x00080000;
	intr_dscr_top_half = (intr_dscr_top_half & 0xffff0000)
		| (intr_dscr_bottom_half & 0x0000ffff);
	intr_dscr_bottom_half = (intr_dscr_bottom_half & 0xffff0000) | 0x00008e00;
	idt_ptr += 33 * 2;
	*idt_ptr = intr_dscr_top_half;
	*(idt_ptr + 1) = intr_dscr_bottom_half;

	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	while (1);

	return 0;
}
