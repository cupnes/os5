#include <cpu.h>
#include <excp.h>
#include <intr.h>
#include <efi.h>
#include <fb.h>
#include <fbcon.h>

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	fb_init(_fb);
	set_fg(255, 255, 255);
	set_bg(0, 70, 250);
	clear_screen();

	fbcon_init();

	gdt_init();

	unsigned char i;
	for (i = 0; i < EXCEPTION_MAX; i++)
		intr_set_handler(i, (unsigned long long)&exception_handler);

	intr_init();
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
