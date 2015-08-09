#include <cpu.h>
#include <intr.h>
#include <console_io.h>
#include <shell.h>

struct segment_descriptor {
	union {
		struct {
			unsigned int a;
			unsigned int b;
		};
		struct {
			unsigned short limit0;
			unsigned short base0;
			unsigned short base1: 8, type: 4, s: 1, dpl: 2, p: 1;
			unsigned short limit1: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
		};
	};
};

void load_task_register(void);

extern struct segment_descriptor gdt[8];

struct tss {
	unsigned short		back_link, __blh;
	unsigned int		esp0;
	unsigned short		ss0, __ss0h;
	unsigned int		esp1;
	unsigned short		ss1, __ss1h;
	unsigned int		esp2;
	unsigned short		ss2, __ss2h;
	unsigned int		__cr3;
	unsigned int		eip;
	unsigned int		eflags;
	unsigned int		eax;
	unsigned int		ecx;
	unsigned int		edx;
	unsigned int		ebx;
	unsigned int		esp;
	unsigned int		ebp;
	unsigned int		esi;
	unsigned int		edi;
	unsigned short		es, __esh;
	unsigned short		cs, __csh;
	unsigned short		ss, __ssh;
	unsigned short		ds, __dsh;
	unsigned short		fs, __fsh;
	unsigned short		gs, __gsh;
	unsigned short		ldt, __ldth;
	unsigned short		trace;
	unsigned short		io_bitmap_base;
} shell_tss, task1_tss;

void do_exception(void)
{
	put_str("exception\r\n");
	while (1);
}

extern unsigned char exception_handler;

void task1(void)
{
	while(1);
}

#define EXCEPTION_NUM	20
int main(void)
{
	unsigned char mask;
	unsigned int limit, base;
	unsigned char i;

	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup GDT for shell_tss */
	limit = sizeof(shell_tss);
	gdt[3].limit0 = limit & 0x0000ffff;
	gdt[3].limit1 = (limit & 0x000f0000) >> 16;

	base = (unsigned int)&shell_tss;
	gdt[3].base0 = base & 0x0000ffff;
	gdt[3].base1 = (base & 0x00ff0000) >> 16;
	gdt[3].base2 = (base & 0xff000000) >> 24;

	gdt[3].type = 9;
	gdt[3].p = 1;

	for (i = 0; i < EXCEPTION_NUM; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);

	/* Setup shell_tss */
	load_task_register();
	put_str("task loaded.\r\n");

	/* Setup GDT for shell_tss */
	limit = sizeof(task1_tss);
	gdt[4].limit0 = limit & 0x0000ffff;
	gdt[4].limit1 = (limit & 0x000f0000) >> 16;

	base = (unsigned int)&task1_tss;
	gdt[4].base0 = base & 0x0000ffff;
	gdt[4].base1 = (base & 0x00ff0000) >> 16;
	gdt[4].base2 = (base & 0xff000000) >> 24;

	gdt[4].type = 9;
	gdt[4].p = 1;

	/* Setup task1_tss */
	task1_tss.eip = (unsigned int)task1;
	task1_tss.esp = 0x00085000;
	task1_tss.es = 0x0010;
	task1_tss.cs = 0x0008;
	task1_tss.ss = 0x0010;
	task1_tss.ds = 0x0010;
	task1_tss.fs = 0x0010;
	task1_tss.gs = 0x0010;

	con_init();
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~INTR_MASK_BIT_KB;
	intr_set_mask_master(mask);
	sti();

	__asm__("ljmp	$0x20, $0");

	start_shell();

	return 0;
}
