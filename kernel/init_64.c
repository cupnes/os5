#include <efi.h>
#include <fb.h>

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	unsigned int x, y;
	fb_pixel *p = (fb_pixel *)_fb->base;

	for (y = 0; y < _fb->vr; y++) {
		for (x = 0; x < _fb->hr; x++) {
			p->Blue = x % 256;
			p->Green = y % 256;
			p->Red = (x + y) % 256;
			p->Reserved = 255;
			p++;
		}
	}

	while (1);

	return 0;
}
