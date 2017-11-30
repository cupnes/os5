#include <efi.h>
#include <fb.h>
#include <font.h>

unsigned int cursor_x = 0, cursor_y = 0;

void putc(char c);

int kern_init(struct EFI_SYSTEM_TABLE *st __attribute__ ((unused)),
	      struct fb *_fb)
{
	/* unsigned int y = 1, x = 3; */
	/* volatile unsigned int a; */
	/* if (font_bitmap['A'][y * FONT_WIDTH + x]) { */
	/* 	a = 0xbeefcafe; */
	/* } else { */
	/* 	a = 0xbabefee1; */
	/* } */

	fb_init(_fb);
	set_fg(255, 255, 255);
	set_bg(0, 70, 250);
	clear_screen();

	putc('A');

	while (1);

	return 0;
}

void putc(char c)
{
	unsigned int x, y;

	for (y = 0; y < FONT_HEIGHT; y++)
		for (x = 0; x < FONT_WIDTH; x++)
			if (font_bitmap[(unsigned int)c][y * FONT_WIDTH + x])
				draw_px_fg(cursor_x + x, cursor_y + y);

	cursor_x += FONT_WIDTH;
	if ((cursor_x + FONT_WIDTH) >= fb.hr) {
		cursor_x = 0;
		cursor_y += FONT_HEIGHT;
	}
}
