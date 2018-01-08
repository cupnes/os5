#include <cpu.h>
#include <excp.h>
#include <intr.h>
#include <efi.h>
#include <fb.h>
#include <fbcon.h>
#include <kbc.h>
#include <kern_task.h>

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	fb_init(_fb);
	set_fg(255, 255, 255);
	set_bg(0, 70, 250);
	clear_screen();

	fbcon_init();

	gdt_init();

	kbc_init();

	kern_task_init();

	excp_init();

	intr_set_handler(INTR_NUM_KB, (unsigned long long)&keyboard_handler);
	intr_init();
	unsigned char mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	while (1) {
		char c = getc();
		if (('a' <= c) && (c <= 'z'))
			c = c - 'a' + 'A';
		else if (c == '\n')
			putc('\r');
		putc(c);
	}

	return 0;
}
