#define sti()	__asm__ ("sti"::)
#define cli()	__asm__ ("cli"::)

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

extern struct segment_descriptor gdt[8];

void load_task_register(void);
void init_gdt(unsigned int idx, unsigned int base, unsigned int limit);
