#ifndef _EXCP_H_
#define _EXCP_H_

#define EXCEPTION_NUM	20

extern unsigned char exception_handler;

void do_exception(void);

#endif /* _EXCP_H_ */