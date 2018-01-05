#include <common.h>
#include <lock.h>

unsigned char error_status;

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

void enqueue(struct queue *q, unsigned char data)
{
	unsigned char if_bit;

	if (q->is_full) {
		error_status = 1;
	} else {
		error_status = 0;
		kern_lock(&if_bit);
		q->buf[q->end] = data;
		q->end++;
		if (q->start == q->end) q->is_full = 1;
		kern_unlock(&if_bit);
	}
}

unsigned char dequeue(struct queue *q)
{
	unsigned char data = 0;
	unsigned char if_bit;

	kern_lock(&if_bit);
	if (!q->is_full && (q->start == q->end)) {
		error_status = 1;
	} else {
		error_status = 0;
		data = q->buf[q->start];
		q->start++;
		q->is_full = 0;
	}
	kern_unlock(&if_bit);

	return data;
}
