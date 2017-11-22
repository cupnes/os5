#include <fb.h>

struct fb fb;

void fb_init(struct fb *_fb)
{
	fb.base = _fb->base;
	fb.size = _fb->size;
	fb.hr = _fb->hr;
	fb.vr = _fb->vr;
}

inline void draw_px(unsigned int x, unsigned int y,
		    unsigned char r, unsigned char g, unsigned char b)
{
	fb_pixel *p = (fb_pixel *)fb.base;
	p += y * fb.hr + x;

	p->Blue = b;
	p->Green = g;
	p->Red = r;
	p->Reserved = 255;
}
