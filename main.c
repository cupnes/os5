#define SCREEN_START	0xb8000
#define COLUMNS	80
#define ATTR	0x07

struct {
	unsigned int x, y;
} cursor_pos;

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

int main(void)
{
	put_str("Hello OS5:main()\r\n");
	put_str("This is a test.\r\n");

	while (1);

	return 0;
}
