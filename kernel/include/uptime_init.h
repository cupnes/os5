#ifndef _UPTIME_INIT_H_
#define _UPTIME_INIT_H_

#include <sched.h>

#define UPTIME_GDT_IDX	5
#define UPTIME_ID	2

extern unsigned char uptime_context_switch[CONTEXT_SWITCH_FN_SIZE];

void uptime_init(void);

#endif /* _UPTIME_INIT_H_ */
