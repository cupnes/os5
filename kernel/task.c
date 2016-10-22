#include <task.h>
#include <memory.h>
#include <fs.h>
#include <sched.h>
#include <common.h>
#include <lock.h>
#include <kernel.h>
#include <cpu.h>

#define GDT_IDX_OFS	5
#define APP_ENTRY_POINT	0x20000020
#define APP_STACK_BASE_USER	0xffffe800
#define APP_STACK_BASE_KERN	0xfffff000
#define APP_STACK_SIZE	4096
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

static int str_get_len(const char *src)
{
	int len;
	for (len = 0; src[len] != '\0'; len++);
	return len + 1;
}

void task_init(struct file *f, int argc, char *argv[])
{
	struct page_directory_entry *pd_base_addr, *pde;
	struct page_table_entry *pt_base_addr, *pt_stack_base_addr, *pte;
	struct task *new_task;
	unsigned int paging_base_addr, phys_stack_base;
	unsigned int i;
	unsigned int len = 0;
	unsigned int argv_space_num, vsp, arg_size;
	unsigned char *sp, *sp2;
	char *t;

	/* Allocate task resources */
	pd_base_addr = (struct page_directory_entry *)mem_alloc();
	pt_base_addr = (struct page_table_entry *)mem_alloc();
	pt_stack_base_addr = (struct page_table_entry *)mem_alloc();
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
	for (i++; i < 0x3ff; i++) {
		pde->all = 0;
		pde++;
	}
	pde->all = 0;
	pde->p = 1;
	pde->r_w = 1;
	pde->u_s = 1;
	pde->pt_base = (unsigned int)pt_stack_base_addr >> 12;
	pde++;

	/* Initialize task page table */
	pte = pt_base_addr;
	paging_base_addr = (unsigned int)f->data_base_addr >> 12;
	for (i = 0; i < f->head->block_num; i++) {
		pte->all = 0;
		pte->p = 1;
		pte->r_w = 1;
		pte->u_s = 1;
		pte->page_base = paging_base_addr++;
		pte++;
	}
	for (; i < 0x400; i++) {
		pte->all = 0;
		pte++;
	}

	/* Initialize stack page table */
	pte = pt_stack_base_addr;
	for (i = 0; i < 0x3fe; i++) {
		pte->all = 0;
		pte++;
	}
	paging_base_addr = phys_stack_base >> 12;
	pte->all = 0;
	pte->p = 1;
	pte->r_w = 1;
	pte->u_s = 1;
	pte->page_base = paging_base_addr;
	pte++;
	pte->all = 0;
	pte++;

	/* Setup task_id */
	new_task->task_id = task_id_counter++;

	/* Setup context switch function */
	copy_mem(context_switch_template, new_task->context_switch_func, CONTEXT_SWITCH_FN_SIZE);
	new_task->context_switch_func[CONTEXT_SWITCH_FN_TSKNO_FIELD] = 8 * (new_task->task_id + GDT_IDX_OFS);
	new_task->context_switch = (void (*)(void))new_task->context_switch_func;

	/* Setup GDT for task_tss */
	init_gdt(new_task->task_id + GDT_IDX_OFS, (unsigned int)&new_task->tss, sizeof(struct tss), 3);

	/* Setup task stack */
	/* スタックにint argcとchar *argv[]を積み、
	 * call命令でジャンプした直後を再現する。
	 *
	 * 例) argc=3, argv={"HOGE", "P", "FUGAA"}
	 * | VA          | 内容              | 備考                               |
	 * |-------------+-------------------+------------------------------------|
	 * | 0x2000 17d0 |                   |                                    |
	 * | 0x2000 17d4 | (Don't Care)      | ESPはここを指した状態にしておく(*) |
	 * | 0x2000 17d8 | 3                 | argc                               |
	 * | 0x2000 17dc | 0x2000 17e4       | argv                               |
	 * | 0x2000 17e0 | (Don't Care)      |                                    |
	 * | 0x2000 17e4 | 0x2000 17f0       | argv[0]                            |
	 * | 0x2000 17e8 | 0x2000 17f5       | argv[1]                            |
	 * | 0x2000 17ec | 0x2000 17f7       | argv[2]                            |
	 * | 0x2000 17f0 | 'H'  'O' 'G'  'E' |                                    |
	 * | 0x2000 17f4 | '\0' 'P' '\0' 'F' |                                    |
	 * | 0x2000 17f8 | 'U'  'G' 'A'  'A' |                                    |
	 * | 0x2000 17fc | '\0'              |                                    |
	 * |-------------+-------------------+------------------------------------|
	 * | 0x2000 1800 |                   |                                    |
	 * (*) call命令はnearジャンプ時、call命令の次の命令のアドレスを
	 * 復帰時のEIPとしてスタックに積むため。
	 */
	for (i = 0; i < (unsigned int)argc; i++) {
		len += str_get_len(argv[i]);
	}
	argv_space_num = (len / 4) + 1;
	arg_size = 4 * (4 + argc + argv_space_num);

	sp = (unsigned char *)(phys_stack_base + (APP_STACK_SIZE / 2));
	sp -= arg_size;

	sp += 4;

	*(int *)sp = argc;
	sp += 4;

	*(unsigned int *)sp = APP_STACK_BASE_USER - (4 * (argc + argv_space_num));
	sp += 4;

	sp += 4;

	vsp = APP_STACK_BASE_USER - (4 * argv_space_num);
	sp2 = sp + (4 * argc);
	for (i = 0; i < (unsigned int)argc; i++) {
		*(unsigned int *)sp = vsp;
		sp += 4;
		t = argv[i];
		for (; *t != '\0'; t++) {
			vsp++;
			*sp2++ = *t;
		}
		*sp2++ = '\0';
		vsp++;
	}

	/* Setup task_tss */
	new_task->tss.eip = APP_ENTRY_POINT;
	new_task->tss.esp = APP_STACK_BASE_USER - arg_size;
	new_task->tss.eflags = 0x00000200;
	new_task->tss.esp0 = APP_STACK_BASE_KERN;
	new_task->tss.ss0 = GDT_KERN_DS_OFS;
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
	struct page_directory_entry *pd_base_addr, *pde;
	struct page_table_entry *pt_base_addr, *pte;
	unsigned int phys_stack_base;

	kern_lock(&if_bit);

	sched_update_wakeupevq(EVENT_TYPE_EXIT);
	sched_runq_del(t);

	pd_base_addr = (struct page_directory_entry *)(t->tss.__cr3 & PAGE_ADDR_MASK);
	pde = pd_base_addr + 0x080;
	pt_base_addr = (struct page_table_entry *)(pde->pt_base << 12);
	pte = pt_base_addr + 0x001;
	phys_stack_base = pte->page_base << 12;

	mem_free((void *)phys_stack_base);
	mem_free(t);
	mem_free(pt_base_addr);
	mem_free(pd_base_addr);

	schedule();

	kern_unlock(&if_bit);
}
