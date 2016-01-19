#include <stddef.h>
#include <memory.h>

#define CR4_BIT_PGE	(1U << 7)
#define MAX_HEAP_PAGES	11
#define HEAP_START_ADDR	0x00095000
#define PAGE_SIZE	0x1000

static char heap_alloc_table[MAX_HEAP_PAGES] = {0};

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
}

void mem_page_start(void)
{
	unsigned int cr0;

	__asm__("movl	%%cr0, %0":"=r"(cr0):);
	cr0 |= 0x80000000;
	__asm__("movl	%0, %%cr0"::"r"(cr0));
}

void *mem_alloc(void)
{
	unsigned int i;

	for (i = 0; heap_alloc_table[i] && (i < MAX_HEAP_PAGES); i++);

	if (i >= MAX_HEAP_PAGES)
		return (void *)NULL;

	heap_alloc_table[i] = 1;
	return (void *)(HEAP_START_ADDR + i * PAGE_SIZE);
}

void mem_free(void *page)
{
	unsigned int i = ((unsigned int)page - HEAP_START_ADDR) / PAGE_SIZE;
	heap_alloc_table[i] = 0;
}
