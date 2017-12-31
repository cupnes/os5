#include <cpu.h>

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
