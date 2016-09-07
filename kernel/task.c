#include <task.h>
#include <memory.h>
#include <fs.h>
#include <sched.h>
#include <common.h>
#include <lock.h>
#include <kernel.h>

#define GDT_IDX_OFS	5
#define APP_ENTRY_POINT	0x20000020
#define APP_STACK_BASE	0x20002000
#define GDT_USER_CS_OFS	0x0018
#define GDT_USER_DS_OFS	0x0020

/*
00000000 <context_switch>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   ea 00 00 00 00 00 00    ljmp   $0x00,$0x0
   a:   5d                      pop    %ebp
   b:   c3                      ret
 */
unsigned char context_switch_template[CONTEXT_SWITCH_FN_SIZE] = {
	0x55,
	0x89, 0xe5,
	0xea, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x5d,
	0xc3
};

static unsigned short task_id_counter = 1;

void task_init(struct file *f)
{
	struct page_directory_entry *pd_base_addr, *pde;
	struct page_table_entry *pt_base_addr, *pte;
	struct task *new_task;
	unsigned int paging_base_addr, phys_stack_base;
	unsigned int i;

	/* Allocate task resources */
	pd_base_addr = (struct page_directory_entry *)mem_alloc();
	pt_base_addr = (struct page_table_entry *)mem_alloc();
	new_task = (struct task *)mem_alloc();
	phys_stack_base = (unsigned int)mem_alloc();

	/* Initialize task page directory */
	pde = pd_base_addr;
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
	pde->pt_base = (unsigned int)pt_base_addr >> 12;
	pde++;
	for (; i < 0x400; i++) {
		pde->all = 0;
		pde++;
	}

	/* Initialize task page table */
	pte = pt_base_addr;
	paging_base_addr = (unsigned int)f->data_base_addr >> 12;
	pte->all = 0;
	pte->p = 1;
	pte->r_w = 1;
	pte->u_s = 1;
	pte->page_base = paging_base_addr;
	pte++;
	paging_base_addr = phys_stack_base >> 12;
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

	/* Setup task_id */
	new_task->task_id = task_id_counter++;

	/* Setup context switch function */
	copy_mem(context_switch_template, new_task->context_switch_func, CONTEXT_SWITCH_FN_SIZE);
	new_task->context_switch_func[CONTEXT_SWITCH_FN_TSKNO_FIELD] = 8 * (new_task->task_id + GDT_IDX_OFS);
	new_task->context_switch = (void (*)(void))new_task->context_switch_func;

	/* Setup GDT for task_tss */
	init_gdt(new_task->task_id + GDT_IDX_OFS, (unsigned int)&new_task->tss, sizeof(struct tss), 3);

	/* Setup task_tss */
	new_task->tss.eip = APP_ENTRY_POINT;
	new_task->tss.esp = 0x20001800;
	new_task->tss.eflags = 0x00000200;
	new_task->tss.esp0 = APP_STACK_BASE;
	new_task->tss.ss0 = 0x0010;
	new_task->tss.es = GDT_USER_DS_OFS | 0x0003;
	new_task->tss.cs = GDT_USER_CS_OFS | 0x0003;
	new_task->tss.ss = GDT_USER_DS_OFS | 0x0003;
	new_task->tss.ds = GDT_USER_DS_OFS | 0x0003;
	new_task->tss.fs = GDT_USER_DS_OFS | 0x0003;
	new_task->tss.gs = GDT_USER_DS_OFS | 0x0003;
	new_task->tss.__cr3 = (unsigned int)pd_base_addr | 0x18;

	/* Add task to run_queue */
	sched_runq_enq(new_task);
}

void task_exit(struct task *t)
{
	unsigned char if_bit;

	kern_lock(&if_bit);

	sched_update_wakeupevq(EVENT_TYPE_EXIT);
	sched_runq_del(t);
	schedule();

	kern_unlock(&if_bit);
}
