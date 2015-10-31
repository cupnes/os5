#include <kern_task.h>
#include <cpu.h>

#define KERN_TASK_GDT_IDX	5

static void kern_task_context_switch(void)
{
	__asm__("ljmp	$0x28, $0");
}

void kern_task_init(void)
{
	static struct tss kern_task_tss;
	unsigned int old_cr3, cr3 = 0x0008f018;
	unsigned short segment_selector = 8 * KERN_TASK_GDT_IDX;

	kern_task_tss.__cr3 = 0x0008f018;
	init_gdt(KERN_TASK_GDT_IDX, (unsigned int)&kern_task_tss, sizeof(kern_task_tss));
	__asm__("movl	%%cr3, %0":"=r"(old_cr3):);
	cr3 |= old_cr3 & 0x00000fe7;
	__asm__("movl	%0, %%cr3"::"r"(cr3));
	__asm__("ltr %0"::"r"(segment_selector));
}
