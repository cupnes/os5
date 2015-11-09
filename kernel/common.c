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

int str_get_len(const char *src)
{
	int len;
	for (len = 0; src[len] != '\0'; len++);
	return len + 1;
}

int str_compare(const char *src, const char *dst)
{
	char is_equal = 1;

	for (; (*src != '\0') && (*dst != '\0'); src++, dst++) {
		if (*src != *dst) {
			is_equal = 0;
			break;
		}
	}

	if (is_equal) {
		if (*src != '\0') {
			return 1;
		} else if (*dst != '\0') {
			return -1;
		} else {
			return 0;
		}
	} else {
		return (int)(*src - *dst);
	}
}
