#include <console_io.h>

int main(void)
{
	move_cursor_rel(0, 2);

	put_str("Hello OS5:main()\r\n");

	while (1) {
		char tmp_char = get_char();
		put_char(tmp_char);
		if (tmp_char == '\n') put_char('\r');
	}

	return 0;
}
