#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <list.h>

void queue_init(struct list *head);
void queue_enq(struct list *entry, struct list *head);
void queue_del(struct list *entry);
void queue_dump(struct list *head);

#endif /* _QUEUE_H_ */
