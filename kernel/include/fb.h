#ifndef _FB_H_
#define _FB_H_

#include <efi.h>

struct fb {
	unsigned long long base;
	unsigned long long size;
	unsigned int hr;
	unsigned int vr;
};

typedef struct EFI_GRAPHICS_OUTPUT_BLT_PIXEL fb_pixel;

extern struct fb fb;

void fb_init(struct fb *_fb);
void set_bg(unsigned char r, unsigned char g, unsigned char b);
inline void draw_px(unsigned int x, unsigned int y,
		    unsigned char r, unsigned char g, unsigned char b);
inline void fill_rect(unsigned int x, unsigned int y,
		      unsigned int w, unsigned int h,
		      unsigned char r, unsigned char g, unsigned char b);
void clear_screen(void);

#endif
