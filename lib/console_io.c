#include <io_port.h>
#include <console_io.h>

const char keymap[] = {
	0x00, 0x1b, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '^', 0x08, 0x09,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '@', '[', '\n', 0x00, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	':', 0x00, 0x00, ']', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0x00, '*',
	0x00, ' ', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, '_', 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, '\\', 0x00, 0x00
};

struct {
	unsigned int x, y;
} cursor_pos;

void do_ir_keyboard(void)
{
	volatile unsigned char *video_mem = (unsigned char *)0xb8000;
	volatile unsigned char tmp;

	tmp = *(video_mem + 160);
	*(video_mem + 160) = ++tmp;
	*(video_mem + 161) = 2;
	outb_p(0x61, 0x0020);
	tmp = inb_p(0x0060);
}

void move_cursor_rel(unsigned int x, unsigned int y)
{
	cursor_pos.x += x;
	cursor_pos.y += y;
}

void put_char_pos(char c, unsigned char x, unsigned char y)
{
	unsigned char *pos;

	pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x) * 2));
	*(unsigned short *)pos = (unsigned short)((ATTR << 8) | c);
}

void put_char(char c)
{
	switch (c) {
	case '\r':
		cursor_pos.x = 0;
		break;

	case '\n':
		cursor_pos.y++;
		break;

	default:
		put_char_pos(c, cursor_pos.x, cursor_pos.y);
		if (cursor_pos.x < COLUMNS - 1) {
			cursor_pos.x++;
		} else {
			cursor_pos.x = 0;
			cursor_pos.y++;
		}
		break;
	}
}

void put_str(char *str)
{
	while (*str != '\0') {
		put_char(*str);
		str++;
	}
}

void dump_hex(unsigned int val, unsigned int num_digits)
{
	unsigned int new_x = cursor_pos.x + num_digits;
	unsigned int dump_digit = new_x - 1;

	while (num_digits) {
		unsigned char tmp_val = val & 0x0000000f;
		if (tmp_val < 10) {
			put_char_pos('0' + tmp_val, dump_digit, cursor_pos.y);
		} else {
			put_char_pos('A' + tmp_val - 10, dump_digit, cursor_pos.y);
		}
		val >>= 4;
		dump_digit--;
		num_digits--;
	}

	cursor_pos.x = new_x;
}

unsigned char get_keydata(void)
{
	while (!(inb_p(IOADR_KBC_STATUS) & IOADR_KBC_STATUS_BIT_OBF));
	return inb_p(IOADR_KBC_DATA);
}

unsigned char get_keycode(void)
{
	return get_keydata() & ~IOADR_KBC_DATA_BIT_BRAKE;
}

unsigned char get_keycode_pressed(void)
{
	unsigned char keycode;
	while ((keycode = get_keydata()) & IOADR_KBC_DATA_BIT_BRAKE);
	return keycode & ~IOADR_KBC_DATA_BIT_BRAKE;
}

unsigned char get_keycode_released(void)
{
	unsigned char keycode;
	while (!((keycode = get_keydata()) & IOADR_KBC_DATA_BIT_BRAKE));
	return keycode & ~IOADR_KBC_DATA_BIT_BRAKE;
}

char get_char(void)
{
	return keymap[get_keycode_released()];
}
