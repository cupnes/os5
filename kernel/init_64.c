#include <efi.h>
#include <fb.h>

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	unsigned int x, y;

	fb_init(_fb);

	for (y = 0; y < fb.vr; y++)
		for (x = 0; x < fb.hr; x++)
			draw_px(x, y, (x + y) % 256, y % 256, x % 256);

	while (1);

	return 0;
}
