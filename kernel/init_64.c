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

	putc('A');

	while (1);

	return 0;
}
