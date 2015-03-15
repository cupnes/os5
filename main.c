#define SCREEN_START	0xb8000
#define COLUMNS	80
#define ATTR	0x07
void put_char(char c, unsigned char x, unsigned char y)
{
	unsigned char *pos;

	pos = (unsigned char *)(SCREEN_START + (((y * COLUMNS) + x) * 2));
	*(unsigned short *)pos = (unsigned short)((ATTR << 8) | c);
}

int main(void)
{
	put_char('B', 0, 0);

	while (1);

	return 0;
}
