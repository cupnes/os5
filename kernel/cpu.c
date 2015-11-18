#include <cpu.h>
#include <uptime.h>

void init_gdt(unsigned int idx, unsigned int base, unsigned int limit)
{
	gdt[idx].limit0 = limit & 0x0000ffff;
	gdt[idx].limit1 = (limit & 0x000f0000) >> 16;

	gdt[idx].base0 = base & 0x0000ffff;
	gdt[idx].base1 = (base & 0x00ff0000) >> 16;
	gdt[idx].base2 = (base & 0xff000000) >> 24;

	if (idx == UPTIME_GDT_IDX)
		gdt[idx].dpl = 3;

	gdt[idx].type = 9;
	gdt[idx].p = 1;
}
