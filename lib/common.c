#include <common.h>

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

int pow(int num, int multer)
{
	if (multer == 0) return 1;
	return pow(num, multer - 1) * num;
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

int str_ahextoint(const char *hex_str)
{
	int len = str_getlen(hex_str);
	int val = 0, i;

	for (i = 0; hex_str[i] != '\0'; i++) {
		if (('0' <= hex_str[i]) && (hex_str[i] <= '9')) {
			val += (hex_str[i] - '0') * pow(16, len - 2 - i);
		} else {
			val += (hex_str[i] - 'a' + 10) * pow(16, len - 2 - i);
		}
	}

	return val;
}

void str_getfirstentry(const char *line, char *first, char *other)
{
	int line_len, first_len, other_len;

	line_len = str_getlen(line);
	first_len = str_findchar(line, ' ');
	if (first_len < 0) {
		memcpy((void *)first, (void *)line, line_len);
		first_len = line_len;
		other_len = 0;
		other[other_len] = '\0';
	} else {
		memcpy((void *)first, (void *)line, first_len);
		first[first_len] = '\0';
		first_len++;
		other_len = line_len - first_len;
		memcpy((void *)other, (void *)(line + first_len), other_len);
	}

#ifdef DEBUG
	put_str(line);
	put_str("|");
	put_str(first);
	put_str(":");
	put_str(other);
	put_str("\r\n");
#endif /* DEBUG */
}
