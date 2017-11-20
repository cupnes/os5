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

#endif
