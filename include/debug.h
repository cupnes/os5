#ifndef __DEBUG_H__
#define __DEBUG_H__

extern volatile unsigned char _flag;

void debug_init(void);
void test_excp_de(void);
void test_excp_pf(void);

#endif /* __DEBUG_H__ */
