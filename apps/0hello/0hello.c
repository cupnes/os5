#include <app.h>
#include <kernel.h>
#include <console.h>

#define MAX_LINE_SIZE	512
#define COLOR_BG	0x7d
#define COLOR_FG	0x1c
#define VRAM_BASE_ADDR	0x203f0000
#define VRAM_SIZE	0x10000
#define RES_WIDTH	320
#define RES_HEIGHT	200

void fill_bg(unsigned char color)
{
	unsigned int addr;
	volatile unsigned char *p;

	for (addr = VRAM_BASE_ADDR; addr < VRAM_BASE_ADDR + VRAM_SIZE; addr++) {
		p = (volatile unsigned char *)addr;
		*p = color;
	}
}

void fill_rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char color)
{
	unsigned int i, j;
	unsigned int addr;
	volatile unsigned char *p;

	for (i = y; i < y + h; i++) {
		for (j = x; j < x + w; j++) {
			addr = VRAM_BASE_ADDR + (i * RES_WIDTH) + j;
			p = (volatile unsigned char *)addr;
			*p = color;
		}
	}
}

int main(int argc, char *argv[])
{
	unsigned char c = 0;
	char buf[MAX_LINE_SIZE];

	while (1) {
		fill_bg(COLOR_BG);
		fill_rect(0, RES_HEIGHT - 10, RES_WIDTH, 10, COLOR_FG);
		get_line(buf, MAX_LINE_SIZE);
	}

	return 0;
}
