#ifndef _CPU_H_
#define _CPU_H_

#include <asm/cpu.h>

#define X86_EFLAGS_IF	0x00000200
#define GDT_KERN_DS_OFS	0x0010

#define GDT_S_SYSTEM	0
#define GDT_S_CODE_OR_DATA	1
#define GDT_TYPE_SYS_TSS_AVAIL	9
#define GDT_TYPE_DATA_RW	2
#define GDT_TYPE_CODE_ER	10

#define SS_KERNEL_CODE	0x0008
#define SS_KERNEL_DATA	0x0010

#define KERN_INT_STACK_BASE	0x0000000000380000
#define KERN_STACK_BASE	0x0000000000400000

#define sti()	__asm__ ("sti"::)
#define cli()	__asm__ ("cli"::)
#define x86_get_eflags()	({			\
unsigned int _v;					\
__asm__ volatile ("\tpushf\n"				\
		  "\tpopl	%0\n":"=r"(_v):);	\
_v;							\
})
#define x86_get_rflags()	({			\
unsigned long long _v;					\
__asm__ volatile ("\tpushf\n"				\
		  "\tpopq	%0\n":"=r"(_v):);	\
_v;							\
})
#ifndef X86_64
#define x86_get_flags	x86_get_eflags
#else
#define x86_get_flags	x86_get_rflags
#endif
#define x86_get_tr()		({			\
unsigned short _v;					\
__asm__ volatile ("\tstr	%0\n":"=r"(_v):);	\
_v;							\
})
#define x86_get_rsp()		({			\
unsigned long long _v;					\
__asm__ volatile ("\tmovq	%%rsp, %0\n":"=r"(_v):);\
_v;							\
})
#define x86_halt()	__asm__ ("hlt"::)

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
			unsigned short limit1: 4, avl: 1, l: 1, d: 1, g: 1,
				base2: 8;
		};
	};
};

#ifndef X86_64
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
};
#else
struct tss {
	unsigned int	__reserved1;
	unsigned int	rsp0l;
	unsigned int	rsp0h;
	unsigned int	rsp1l;
	unsigned int	rsp1h;
	unsigned int	rsp2l;
	unsigned int	rsp2h;
	unsigned int	__reserved2;
	unsigned int	__reserved3;
	unsigned int	ist1l;
	unsigned int	ist1h;
	unsigned int	ist2l;
	unsigned int	ist2h;
	unsigned int	ist3l;
	unsigned int	ist3h;
	unsigned int	ist4l;
	unsigned int	ist4h;
	unsigned int	ist5l;
	unsigned int	ist5h;
	unsigned int	ist6l;
	unsigned int	ist6h;
	unsigned int	ist7l;
	unsigned int	ist7h;
	unsigned int	__reserved4;
	unsigned int	__reserved5;
	unsigned short	__reserved6, io_bitmap_base;
};
#endif

extern struct segment_descriptor gdt[GDT_SIZE];

#ifdef X86_64
void gdt_init(void);
#endif
void gdt_set(unsigned int idx, unsigned int base, unsigned int limit,
	     unsigned char g, unsigned char d, unsigned char l,
	     unsigned char dpl, unsigned char s, unsigned char type);
#ifdef X86_64
void dump_stack(unsigned int depth, unsigned long long sp_addr);
#endif

#endif /* _CPU_H_ */
