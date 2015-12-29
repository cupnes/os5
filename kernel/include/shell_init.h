#ifndef _SHELL_INIT_H_
#define _SHELL_INIT_H_

#include <sched.h>

#define SHELL_GDT_IDX	4
#define SHELL_ID	1

extern unsigned char shell_context_switch[CONTEXT_SWITCH_FN_SIZE];

void shell_init(void);

#endif /* _SHELL_INIT_H_ */
