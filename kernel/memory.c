#include <memory.h>

void mem_init(void)
{
	struct page_directory_entry {
		union {
			struct {
				unsigned int all;
			};
			struct {
				unsigned int p: 1, r_w: 1, u_s: 1, pwt: 1, pcd: 1, a: 1, reserved: 1, ps: 1, g: 1, usable: 3, pt_base: 20;
			};
		};
	} *pde;
	struct page_table_entry {
		union {
			struct {
				unsigned int all;
			};
			struct {
				unsigned int p: 1, r_w: 1, u_s: 1, pwt: 1, pcd: 1, a: 1, d: 1, pat: 1, g: 1, usable: 3, page_base: 20;
			};
		};
	} *pte;
	unsigned int paging_base_addr;
	unsigned int i;

	/* Initialize page directory */
	pde = (struct page_directory_entry *)0x00090000;
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->pwt = 1;
	pde->pcd = 1;
	pde->pt_base = 0x00091;
	pde++;

	for (i = 1; i < 0x080; i++) {
		pde->all = 0;
		pde++;
	}

	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->pwt = 1;
	pde->pcd = 1;
	pde->pt_base = 0x00092;
	pde++;

	for (; i < 0x400; i++) {
		pde->all = 0;
		pde++;
	}

	/* Initialize kernel page table */
	pte = (struct page_table_entry *)0x00091000;
	for (i = 0x000; i < 0x007; i++) {
		pte->all = 0;
		pte++;
	}
	paging_base_addr = 0x00007;
	for (; i <= 0x085; i++) {
		pte->all = 0;
		pte->p = 1;
		pte->r_w = 1;
		pte->pwt = 1;
		pte->pcd = 1;
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
		pte->page_base = paging_base_addr;
		paging_base_addr += 0x00001;
		pte++;
	}
	for (; i < 0x400; i++) {
		pte->all = 0;
		pte++;
	}

	/* Initialize uptime page table */
	pte = (struct page_table_entry *)0x00092000;
	paging_base_addr = 0x00011;
	pte->all = 0;
	pte->p = 1;
	pte->r_w = 1;
	pte->pwt = 1;
	pte->pcd = 1;
	pte->page_base = paging_base_addr;
	paging_base_addr += 0x00001;
	pte++;

	for (i = 1; i < 0x400; i++) {
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
