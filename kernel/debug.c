#include <debug.h>

volatile unsigned char _flag;

void debug_init(void)
{
	_flag = 0;
}

/* Test divide by zero exception */
void test_excp_de(void)
{
	__asm__("\tmovw	$8, %%ax\n" \
		"\tmovb	$0, %%bl\n" \
		"\tdivb	%%bl"::);
}

/* Test page fault exception */
void test_excp_pf(void)
{
	volatile unsigned char tmp;
	__asm__("movb	0x000b8000, %0":"=r"(tmp):);
}
