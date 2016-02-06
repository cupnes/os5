#ifndef _LOCK_H_
#define _LOCK_H_

void kern_lock(unsigned char *if_bit);
void kern_unlock(unsigned char *if_bit);

#endif /* _LOCK_H_ */
