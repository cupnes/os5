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
