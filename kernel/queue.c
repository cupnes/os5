#include <queue.h>
#include <list.h>
#include <console_io.h>

void queue_init(struct list *head)
{
	head->next = head;
	head->prev = head;
}

void queue_enq(struct list *entry, struct list *head)
{
	entry->prev = head->prev;
	entry->next = head;
	head->prev->next = entry;
	head->prev = entry;
}

void queue_del(struct list *entry)
{
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

void queue_dump(struct list *head)
{
	unsigned int n;
	struct list *entry;

	put_str("h : p=");
	dump_hex((unsigned int)head->prev, 8);
	put_str(", n=");
	dump_hex((unsigned int)head->next, 8);
	put_str("\r\n");

	for (entry = head->next, n = 0; entry != head; entry = entry->next, n++) {
		dump_hex(n, 2);
		put_str(": p=");
		dump_hex((unsigned int)entry->prev, 8);
		put_str(", n=");
		dump_hex((unsigned int)entry->next, 8);
		put_str("\r\n");
	}
}
