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
	put_char_pos(c, cursor_pos.x, cursor_pos.y);
	if (cursor_pos.x < COLUMNS - 1) {
		cursor_pos.x++;
	} else {
		cursor_pos.x = 0;
		cursor_pos.y++;
	}
}

int main(void)
{
	unsigned char i;

	for (i = 0; i < COLUMNS - 1; i++) put_char('A');
	for (i = 0; i < COLUMNS - 2; i++) put_char('B');

	while (1);

	return 0;
}
