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
