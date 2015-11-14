#include <common.h>

void copy_mem(const void *src, void *dst, unsigned int size)
{
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;

	for (; size > 0; size--) {
		*d = *s;
		d++;
		s++;
	}
}

int pow(int num, int multer)
{
	if (multer == 0) return 1;
	return pow(num, multer - 1) * num;
}

int str_find_char(const char *src, char key)
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
