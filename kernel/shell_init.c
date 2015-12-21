#include <shell_init.h>
#include <cpu.h>
#include <console_io.h>
#include <memory.h>
#include <sched.h>

#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x20002000

struct tss shell_tss;

void shell_context_switch(void)
{
	__asm__("ljmp	$0x20, $0");
}

void shell_init(void)
{
	struct page_directory_entry *pde;
	struct page_table_entry *pte;
	unsigned int paging_base_addr;
	unsigned int i;

	/* Initialize shell page directory */
	pde = (struct page_directory_entry *)0x00091000;
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->pt_base = 0x00090;
	pde++;
	for (i = 1; i < 0x080; i++) {
		pde->all = 0;
		pde++;
	}
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->u_s = 1;
	pde->pt_base = 0x00092;
	pde++;
	for (; i < 0x400; i++) {
		pde->all = 0;
		pde++;
	}

	/* Initialize shell page table */
	pte = (struct page_table_entry *)0x00092000;
	paging_base_addr = 0x00011;
	pte->all = 0;
	pte->p = 1;
	pte->r_w = 1;
	pte->u_s = 1;
	pte->page_base = paging_base_addr;
	pte++;
	paging_base_addr = 0x00070;
	pte->all = 0;
	pte->p = 1;
	pte->r_w = 1;
	pte->u_s = 1;
	pte->page_base = paging_base_addr;
	pte++;
	for (i = 2; i < 0x400; i++) {
		pte->all = 0;
		pte++;
	}

	/* Setup context switch function */
	task_list[SHELL_ID].context_switch = shell_context_switch;

	/* Setup GDT for shell_tss */
	init_gdt(SHELL_GDT_IDX, (unsigned int)&shell_tss, sizeof(shell_tss));

	/* Setup shell_tss */
	shell_tss.eip = APP_ENTRY_POINT;
	shell_tss.esp = 0x20001800;
	shell_tss.eflags = 0x00000200;
	shell_tss.esp0 = APP_STACK_BASE;
	shell_tss.ss0 = 0x0010;
	shell_tss.es = 0x0038 | 0x0003;
	shell_tss.cs = 0x0030 | 0x0003;
	shell_tss.ss = 0x0038 | 0x0003;
	shell_tss.ds = 0x0038 | 0x0003;
	shell_tss.fs = 0x0038 | 0x0003;
	shell_tss.gs = 0x0038 | 0x0003;
	shell_tss.__cr3 = 0x00091018;

	/* Add shell to run_queue */
	sched_runq_enq(&task_list[SHELL_ID]);
}
