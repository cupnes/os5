#include <cpu.h>

#ifdef X86_64
#include <asm/cpu.h>

struct segment_descriptor gdt[GDT_SIZE];
unsigned long long gdtr[2];

void gdt_init(void)
{
	unsigned char i;

	for (i = 0; i < GDT_SIZE; i++)
		gdt[i].a = gdt[i].b = 0;

	gdt_set(1, 0, 0xfffff, 1, 0, 1, 0, GDT_S_CODE_OR_DATA,
		GDT_TYPE_CODE_ER);
	gdt_set(2, 0, 0xfffff, 1, 1, 0, 0, GDT_S_CODE_OR_DATA,
		GDT_TYPE_DATA_RW);

	gdtr[0] = ((unsigned long long)gdt << 16) | (GDT_SIZE * 8 - 1);
	gdtr[1] = ((unsigned long long)gdt >> 48);

	__asm__ ("lgdt	gdtr\n"
		 "mov	$2*8, %ax\n"
		 "mov	%ax, %ds\n"
		 "mov	%ax, %es\n"
		 "mov	%ax, %fs\n"
		 "mov	%ax, %gs\n"
		 "mov	%ax, %ss\n");

	unsigned short selector = 8;
	unsigned long long dummy;
	__asm__ ("pushq %[selector];"
		 "leaq ret_label(%%rip), %[dummy];"
		 "pushq %[dummy];"
		 "lretq;"
		 "ret_label:"
		 : [dummy]"=r"(dummy)
		 : [selector]"m"(selector));
}
#endif

void gdt_set(unsigned int idx, unsigned int base, unsigned int limit,
	     unsigned char g, unsigned char d, unsigned char l,
	     unsigned char dpl, unsigned char s, unsigned char type)
{
	gdt[idx].a = gdt[idx].b = 0;

	gdt[idx].limit0 = limit & 0x0000ffff;
	gdt[idx].limit1 = (limit & 0x000f0000) >> 16;

	gdt[idx].base0 = base & 0x0000ffff;
	gdt[idx].base1 = (base & 0x00ff0000) >> 16;
	gdt[idx].base2 = (base & 0xff000000) >> 24;

	gdt[idx].g = g;
	gdt[idx].d = d;
	gdt[idx].l = l;

	gdt[idx].dpl = dpl;

	gdt[idx].s = s;
	gdt[idx].type = type;
	gdt[idx].p = 1;
}
