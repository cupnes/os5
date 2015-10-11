#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <console_io.h>
#include <timer.h>
#include <shell.h>
#include <uptime.h>

int main(void)
{
	unsigned char mask;
	unsigned int i;

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
	unsigned int cr0;

	/* Setup console */
	cli();
	cursor_pos.y += 2;
	update_cursor();
	cli();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_NUM; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);
	intr_set_handler(13, (unsigned int)&general_protection_handler);
	intr_set_handler(14, (unsigned int)&page_fault_handler);

	/* Setup devices */
	con_init();
	timer_init();
	/* mmu_init(); */
	pde = (struct page_directory_entry *)0x00090000;
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->pwt = 1;
	pde->pcd = 1;
	pde->pt_base = 0x00091;
	pde++;
	for (i = 0; i < 1023; i++) {
		pde->all = 0;
		pde++;
	}

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
	for (; i < 0x1000; i++) {
		pte->all = 0;
		pte++;
	}

	/* Setup tasks */
	shell_init();
	cli();
	uptime_init();

	/* Start paging */
	__asm__("movl	%%cr0, %0":"=r"(cr0):);
	cr0 |= 0x80000000;
	__asm__("movl	%0, %%cr0"::"r"(cr0));

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* Start main task */
	shell_start();

	return 0;
}
