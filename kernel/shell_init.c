#include <shell_init.h>
#include <cpu.h>
#include <console_io.h>
#include <memory.h>
#include <sched.h>

#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x20002000

struct tss shell_tss;

/*
00000000 <shell_context_switch>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   ea 00 00 00 00 20 00    ljmp   $0x20,$0x0
   a:   5d                      pop    %ebp
   b:   c3                      ret
 */
unsigned char shell_context_switch[CONTEXT_SWITCH_FN_SIZE] = {
	0x55,
	0x89, 0xe5,
	0xea, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00,
	0x5d,
	0xc3
};
