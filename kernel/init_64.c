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

	unsigned char i;
	for (i = 0; i < 100; i++) {
		putc('A');
		putc('\r');
		putc('\n');
	}

	putc('\r');
	putc('\n');
	putc('B');

	while (1);

	return 0;
}
