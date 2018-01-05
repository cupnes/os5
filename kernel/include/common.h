#ifndef _COMMON_H_
#define _COMMON_H_

#define QUEUE_BUF_SIZE	256

struct queue {
	unsigned char buf[QUEUE_BUF_SIZE];
	unsigned char start, end;
	unsigned char is_full;
};

extern unsigned char error_status;

int str_compare(const char *src, const char *dst);
void copy_mem(const void *src, void *dst, unsigned int size);
void enqueue(struct queue *q, unsigned char data);
unsigned char dequeue(struct queue *q);

#endif /* _COMMON_H_ */
