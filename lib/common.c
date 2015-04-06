#include <common.h>
#include <console_io.h>

void get_line(char *buf, unsigned int buf_size)
{
	unsigned int i;

	for (i = 0; i < buf_size - 1; i++) {
		buf[i] = get_char();
		put_char(buf[i]);
		if (buf[i] == '\n') {
			put_char('\r');
			break;
		}
	}
	buf[i] = '\0';
}

void memcpy(void *dst, const void *src, unsigned int size)
{
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;

	for (; size > 0; size--) {
		*d = *s;
		d++;
		s++;
	}
}

int str_findchar(const char *src, char key)
{
	int i;

	for (i = 0; src[i] != key; i++) {
		if (src[i] == '\0') {
			i = -1;
			break;
		}
	}

	return i;
}

int str_getlen(const char *src)
{
	int len;
	for (len = 0; src[len] != '\0'; len++);
	return len + 1;
}
