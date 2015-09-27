#ifndef _UPTIME_H_
#define _UPTIME_H_

#define UPTIME_GDT_IDX	4
#define UPTIME_ID	1

extern unsigned char tmp_uptime_start;

void uptime_context_switch(void);
void uptime_init(void);
void uptime_start(void);

#endif /* _UPTIME_H_ */
