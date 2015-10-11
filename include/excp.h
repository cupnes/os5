#ifndef _EXCP_H_
#define _EXCP_H_

#define EXCEPTION_MAX	20
#define EXCP_NUM_PF	14

extern unsigned char exception_handler;
extern unsigned char page_fault_handler;

void do_exception(void);

#endif /* _EXCP_H_ */
