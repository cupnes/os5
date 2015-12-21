#include <memory.h>

#define CR4_BIT_PGE	(1U << 7)

void mem_init(void)
{
	struct page_directory_entry *pde;
	struct page_table_entry *pte;
	unsigned int paging_base_addr;
	unsigned int i;
	unsigned int cr4;

	/* Enable PGE(Page Global Enable) flag of CR4*/
	__asm__("movl	%%cr4, %0":"=r"(cr4):);
	cr4 |= CR4_BIT_PGE;
	__asm__("movl	%0, %%cr4"::"r"(cr4));

	/* Initialize kernel page directory */
	pde = (struct page_directory_entry *)0x0008f000;
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->pt_base = 0x00090;
	pde++;
	for (i = 1; i < 0x400; i++) {
		pde->all = 0;
		pde++;
	}

	/* Initialize kernel page table */
	pte = (struct page_table_entry *)0x00090000;
	for (i = 0x000; i < 0x007; i++) {
		pte->all = 0;
		pte++;
	}
	paging_base_addr = 0x00007;
	for (; i <= 0x085; i++) {
		pte->all = 0;
		pte->p = 1;
		pte->r_w = 1;
		pte->g = 1;
		pte->page_base = paging_base_addr;
		paging_base_addr += 0x00001;
		pte++;
	}
	for (; i < 0x0b8; i++) {
		pte->all = 0;
		pte++;
	}
	paging_base_addr = 0x000b8;
	for (; i <= 0x0bf; i++) {
		pte->all = 0;
		pte->p = 1;
		pte->r_w = 1;
		pte->pwt = 1;
		pte->pcd = 1;
		pte->g = 1;
		pte->page_base = paging_base_addr;
		paging_base_addr += 0x00001;
		pte++;
	}
	for (; i < 0x400; i++) {
		pte->all = 0;
		pte++;
	}

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
}

void mem_page_start(void)
{
	unsigned int cr0;

	__asm__("movl	%%cr0, %0":"=r"(cr0):);
	cr0 |= 0x80000000;
	__asm__("movl	%0, %%cr0"::"r"(cr0));
}
