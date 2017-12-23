#include <fb.h>

struct fb fb;
fb_pixel color_fg;
fb_pixel color_bg;

void fb_init(struct fb *_fb)
{
	fb.base = _fb->base;
	fb.size = _fb->size;
	fb.hr = _fb->hr;
	fb.vr = _fb->vr;
}

void set_fg(unsigned char r, unsigned char g, unsigned char b)
{
	color_fg.Blue = b;
	color_fg.Green = g;
	color_fg.Red = r;
	color_fg.Reserved = 255;
}

void set_bg(unsigned char r, unsigned char g, unsigned char b)
{
	color_bg.Blue = b;
	color_bg.Green = g;
	color_bg.Red = r;
	color_bg.Reserved = 255;
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

inline void draw_px_fg(unsigned int x, unsigned int y)
{
	fb_pixel *p = (fb_pixel *)fb.base;
	p += y * fb.hr + x;

	p->Blue = color_fg.Blue;
	p->Green = color_fg.Green;
	p->Red = color_fg.Red;
	p->Reserved = color_fg.Reserved;
}

inline void fill_rect(unsigned int x, unsigned int y,
		      unsigned int w, unsigned int h,
		      unsigned char r, unsigned char g, unsigned char b)
{
	unsigned int i, j;
	for (i = y; i < (y + h); i++)
		for (j = x; j < (x + w); j++)
			draw_px(j, i, r, g, b);
}

void clear_screen(void)
{
	fill_rect(0, 0, fb.hr, fb.vr,
		  color_bg.Red, color_bg.Green, color_bg.Blue);
}
