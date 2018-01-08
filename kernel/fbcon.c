#include <fbcon.h>
#include <fb.h>
#include <font.h>
#include <kbc.h>

#define MAX_STR_BUF	100

unsigned int cursor_x, cursor_y;

void fbcon_init(void)
{
	cursor_x = cursor_y = 0;
}

void putc(char c)
{
	unsigned int x, y;

	switch(c) {
	case '\r':
		cursor_x = 0;
		break;

	case '\n':
		cursor_y += FONT_HEIGHT;
		if ((cursor_y + FONT_HEIGHT) >= fb.vr) {
			cursor_x = cursor_y = 0;
			clear_screen();
		}
		break;

	default:
		for (y = 0; y < FONT_HEIGHT; y++)
			for (x = 0; x < FONT_WIDTH; x++)
				if (font_bitmap[(unsigned int)c][y][x])
					draw_px_fg(cursor_x + x, cursor_y + y);

		cursor_x += FONT_WIDTH;
		if ((cursor_x + FONT_WIDTH) >= fb.hr) {
			cursor_x = 0;
			cursor_y += FONT_HEIGHT;
			if ((cursor_y + FONT_HEIGHT) >= fb.vr) {
				cursor_x = cursor_y = 0;
				clear_screen();
			}
		}

		break;
	}
}

void puts(char *s)
{
	while (*s != '\0')
		putc(*s++);
}

void puth(unsigned long long val, unsigned char num_digits)
{
	int i;
	unsigned short unit_val;
	char str[MAX_STR_BUF];

	for (i = num_digits - 1; i >= 0; i--) {
		unit_val = (unsigned short)(val & 0x0f);
		if (unit_val < 0xa)
			str[i] = '0' + unit_val;
		else
			str[i] = 'A' + (unit_val - 0xa);
		val >>= 4;
	}
	str[num_digits] = '\0';

	puts(str);
}

char getc(void)
{
	return keymap[get_keycode_pressed()];
}
