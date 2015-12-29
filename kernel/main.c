#include <cpu.h>
#include <intr.h>
#include <excp.h>
#include <memory.h>
#include <console_io.h>
#include <timer.h>
#include <kernel.h>
#include <sched.h>
#include <kern_task.h>
#include <shell_init.h>
#include <uptime_init.h>

#define GDT_IDX_OFS	3
#define APP_ENTRY_POINT	0x20000000
#define APP_STACK_BASE	0x20002000

void kern_lock(unsigned char *if_bit)
{
	/* Save EFlags.IF */
	*if_bit = (x86_get_eflags() & X86_EFLAGS_IF) ? 1 : 0;

	/* if saved IF == true, then cli */
	if (*if_bit)
		cli();
}

void kern_unlock(unsigned char *if_bit)
{
	/* if saved IF == true, then sti */
	if (*if_bit)
		sti();
}

unsigned int do_syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3)
{
	unsigned int result = -1;

	switch (syscall_id) {
	case SYSCALL_TIMER_GET_GLOBAL_COUNTER:
		result = timer_get_global_counter();
		break;
	case SYSCALL_SCHED_WAKEUP_MSEC:
		wakeup_after_msec(arg1);
		result = 0;
		break;
	case SYSCALL_CON_GET_CURSOR_POS_Y:
		result = (unsigned int)cursor_pos.y;
		break;
	case SYSCALL_CON_PUT_STR:
		put_str((char *)arg1);
		result = 0;
		break;
	case SYSCALL_CON_PUT_STR_POS:
		put_str_pos((char *)arg1, (unsigned char)arg2, (unsigned char)arg3);
		result = 0;
		break;
	case SYSCALL_CON_DUMP_HEX:
		dump_hex(arg1, arg2);
		result = 0;
		break;
	case SYSCALL_CON_DUMP_HEX_POS:
		dump_hex_pos(arg1, arg2, (unsigned char)(arg3 >> 16), (unsigned char)(arg3 & 0x0000ffff));
		result= 0;
		break;
	case SYSCALL_CON_GET_LINE:
		result = get_line((char *)arg1, arg2);
		break;
	}

	return result;
}

static void copy_mem(const void *src, void *dst, unsigned int size)
{
	unsigned char *d = (unsigned char *)dst;
	unsigned char *s = (unsigned char *)src;

	for (; size > 0; size--) {
		*d = *s;
		d++;
		s++;
	}
}

static void task_init(unsigned short task_id,
		      struct page_directory_entry *pd_base_addr,
		      struct page_table_entry *pt_base_addr,
		      unsigned int phys_binary_base,
		      unsigned int phys_stack_base,
		      void (*context_switch_fn)(void))
{
	struct page_directory_entry *pde;
	struct page_table_entry *pte;
	unsigned int paging_base_addr;
	unsigned int i;

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
	paging_base_addr = phys_binary_base >> 12;
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

	/* Setup context switch function */
	copy_mem(context_switch_template, task_list[task_id].context_switch_func, CONTEXT_SWITCH_FN_SIZE);
	task_list[task_id].context_switch_func[CONTEXT_SWITCH_FN_TSKNO_FIELD] = 8 * (task_id - 1) + 0x20;
	task_list[task_id].context_switch = (void (*)(void))task_list[task_id].context_switch_func;

	/* Setup GDT for task_tss */
	init_gdt(task_id + GDT_IDX_OFS, (unsigned int)&task_list[task_id].tss, sizeof(struct tss));

	/* Setup task_tss */
	task_list[task_id].tss.eip = APP_ENTRY_POINT;
	task_list[task_id].tss.esp = 0x20001800;
	task_list[task_id].tss.eflags = 0x00000200;
	task_list[task_id].tss.esp0 = APP_STACK_BASE;
	task_list[task_id].tss.ss0 = 0x0010;
	task_list[task_id].tss.es = 0x0038 | 0x0003;
	task_list[task_id].tss.cs = 0x0030 | 0x0003;
	task_list[task_id].tss.ss = 0x0038 | 0x0003;
	task_list[task_id].tss.ds = 0x0038 | 0x0003;
	task_list[task_id].tss.fs = 0x0038 | 0x0003;
	task_list[task_id].tss.gs = 0x0038 | 0x0003;
	task_list[task_id].tss.__cr3 = (unsigned int)pd_base_addr | 0x18;

	/* Add task to run_queue */
	sched_runq_enq(&task_list[task_id]);
}

int main(void)
{
	extern unsigned char syscall_handler;

	unsigned char mask;
	unsigned char i;

	/* Setup console */
	cursor_pos.y += 2;
	update_cursor();

	/* Setup exception handler */
	for (i = 0; i < EXCEPTION_MAX; i++)
		intr_set_handler(i, (unsigned int)&exception_handler);
	intr_set_handler(EXCP_NUM_DE, (unsigned int)&divide_error_handler);
	intr_set_handler(EXCP_NUM_DB, (unsigned int)&debug_handler);
	intr_set_handler(EXCP_NUM_NMI, (unsigned int)&nmi_handler);
	intr_set_handler(EXCP_NUM_BP, (unsigned int)&breakpoint_handler);
	intr_set_handler(EXCP_NUM_OF, (unsigned int)&overflow_handler);
	intr_set_handler(EXCP_NUM_BR, (unsigned int)&bound_range_exceeded_handler);
	intr_set_handler(EXCP_NUM_UD, (unsigned int)&invalid_opcode_handler);
	intr_set_handler(EXCP_NUM_NM, (unsigned int)&device_not_available_handler);
	intr_set_handler(EXCP_NUM_DF, (unsigned int)&double_fault_handler);
	intr_set_handler(EXCP_NUM_CSO, (unsigned int)&coprocessor_segment_overrun_handler);
	intr_set_handler(EXCP_NUM_TS, (unsigned int)&invalid_tss_handler);
	intr_set_handler(EXCP_NUM_NP, (unsigned int)&segment_not_present_handler);
	intr_set_handler(EXCP_NUM_SS, (unsigned int)&stack_fault_handler);
	intr_set_handler(EXCP_NUM_GP, (unsigned int)&general_protection_handler);
	intr_set_handler(EXCP_NUM_PF, (unsigned int)&page_fault_handler);
	intr_set_handler(EXCP_NUM_MF, (unsigned int)&x87_fpu_floating_point_error_handler);
	intr_set_handler(EXCP_NUM_AC, (unsigned int)&alignment_check_handler);
	intr_set_handler(EXCP_NUM_MC, (unsigned int)&machine_check_handler);
	intr_set_handler(EXCP_NUM_XM, (unsigned int)&simd_floating_point_handler);
	intr_set_handler(EXCP_NUM_VE, (unsigned int)&virtualization_handler);

	/* Setup devices */
	con_init();
	timer_init();
	mem_init();

	/* Setup tasks */
	kern_task_init();
	task_init(SHELL_ID, (struct page_directory_entry *)0x00091000,
		  (struct page_table_entry *)0x00092000, 0x00011000, 0x00070000,
		  (void (*)(void))shell_context_switch);
	task_init(UPTIME_ID, (struct page_directory_entry *)0x00093000,
		  (struct page_table_entry *)0x00094000, 0x00012000, 0x00071000,
		  (void (*)(void))uptime_context_switch);

	/* Start paging */
	mem_page_start();

	/* Setup interrupt handler and mask register */
	intr_set_handler(INTR_NUM_TIMER, (unsigned int)&timer_handler);
	intr_set_handler(INTR_NUM_KB, (unsigned int)&keyboard_handler);
	intr_set_handler(INTR_NUM_USER128, (unsigned int)&syscall_handler);
	intr_init();
	mask = intr_get_mask_master();
	mask &= ~(INTR_MASK_BIT_TIMER | INTR_MASK_BIT_KB);
	intr_set_mask_master(mask);
	sti();

	/* End of kernel initialization process */
	while (1) {
		x86_halt();
	}

	return 0;
}
