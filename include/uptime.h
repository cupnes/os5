#ifndef _UPTIME_H_
#define _UPTIME_H_

#define UPTIME_GDT_IDX	4
#define UPTIME_ID	UPTIME_GDT_IDX

void uptime_context_switch(void);
void uptime_init(void);
void uptime_start(void);

#endif /* _UPTIME_H_ */
