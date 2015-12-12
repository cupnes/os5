#ifndef _UPTIME_INIT_H_
#define _UPTIME_INIT_H_

#define UPTIME_GDT_IDX	5
#define UPTIME_ID	2

void uptime_context_switch(void);
void uptime_init(void);

#endif /* _UPTIME_INIT_H_ */
