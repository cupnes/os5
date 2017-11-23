#include <efi.h>
#include <fb.h>

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	fb_init(_fb);
	set_bg(0, 70, 250);
	clear_screen();

	while (1);

	return 0;
}
