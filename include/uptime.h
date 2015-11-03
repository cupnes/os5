#ifndef _UPTIME_H_
#define _UPTIME_H_

#define UPTIME_GDT_IDX	5
#define UPTIME_ID	1

void uptime_context_switch(void);
void uptime_init(void);
void uptime_start(void);

#endif /* _UPTIME_H_ */
