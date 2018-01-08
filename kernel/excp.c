#include <excp.h>
#include <intr.h>
#ifndef X86_64
#include <console_io.h>
#else
#include <cpu.h>
#include <fbcon.h>
#endif

#define EXCP_DUMP_STACK_DEPTH	30

void do_exception(void)
{
#ifndef X86_64
	put_str("exception\r\n");
#else
	puts("exception\r\n");
#endif
	while (1);
}

void do_divide_error_handler(unsigned long long sp)
{
	puts("DIVIDE ERROR\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_debug_handler(unsigned long long sp)
{
	puts("DEBUG\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_nmi_handler(unsigned long long sp)
{
	puts("NMI\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_breakpoint_handler(unsigned long long sp)
{
	puts("BREAKPOINT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_overflow_handler(unsigned long long sp)
{
	puts("OVERFLOW\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_bound_range_exceeded_handler(unsigned long long sp)
{
	puts("BOUND RANGE EXCEEDED\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_invalid_opcode_handler(unsigned long long sp)
{
	puts("INVALID OPCODE\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_device_not_available_handler(unsigned long long sp)
{
	puts("DEVICE NOT AVAILABLE\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_double_fault_handler(unsigned long long sp)
{
	puts("DOUBLE FAULT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_coprocessor_segment_overrun_handler(unsigned long long sp)
{
	puts("COPROCESSOR SEGMENT OVERRUN\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_invalid_tss_handler(unsigned long long sp)
{
	puts("INVALID TSS\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_segment_not_present_handler(unsigned long long sp)
{
	puts("SEGMENT NOT PRESENT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_stack_fault_handler(unsigned long long sp)
{
	puts("STACK FAULT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_general_protection_handler(unsigned long long sp)
{
	puts("GENERAL PROTECTION\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_page_fault_handler(unsigned long long sp)
{
	puts("PAGE FAULT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_x87_fpu_floating_point_error_handler(unsigned long long sp)
{
	puts("X87 FPU FLOATING POINT ERROR\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_alignment_check_handler(unsigned long long sp)
{
	puts("ALIGNMENT CHECK\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_machine_check_handler(unsigned long long sp)
{
	puts("MACHINE CHECK\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_simd_floating_point_handler(unsigned long long sp)
{
	puts("SIMD FLOATING POINT\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

void do_virtualization_handler(unsigned long long sp)
{
	puts("VIRTUALIZATION\r\n");
	dump_stack(EXCP_DUMP_STACK_DEPTH, sp);
	while (1);
}

/* void do_page_fault(unsigned int error_code, unsigned int address) */
/* { */
/* #ifndef X86_64 */
/* 	put_str("page fault\r\n"); */
/* 	put_str("error code: 0x"); */
/* 	dump_hex(error_code, 8); */
/* 	put_str("\r\n"); */
/* 	put_str("address   : 0x"); */
/* 	dump_hex(address, 8); */
/* 	put_str("\r\n"); */
/* #else */
/* 	(void)error_code; */
/* 	(void)address; */
/* 	puts("page fault\r\n"); */
/* #endif */
/* 	while (1); */
/* } */

void excp_init(void)
{
	intr_set_handler(EXCP_NUM_DE, (unsigned long long)divide_error_handler);
	intr_set_handler(EXCP_NUM_DB, (unsigned long long)debug_handler);
	intr_set_handler(EXCP_NUM_NMI, (unsigned long long)nmi_handler);
	intr_set_handler(EXCP_NUM_BP, (unsigned long long)breakpoint_handler);
	intr_set_handler(EXCP_NUM_OF, (unsigned long long)overflow_handler);
	intr_set_handler(EXCP_NUM_BR,
			 (unsigned long long)bound_range_exceeded_handler);
	intr_set_handler(EXCP_NUM_UD,
			 (unsigned long long)invalid_opcode_handler);
	intr_set_handler(EXCP_NUM_NM,
			 (unsigned long long)device_not_available_handler);
	intr_set_handler(EXCP_NUM_DF, (unsigned long long)double_fault_handler);
	intr_set_handler(EXCP_NUM_CSO,
		(unsigned long long)coprocessor_segment_overrun_handler);
	intr_set_handler(EXCP_NUM_TS, (unsigned long long)invalid_tss_handler);
	intr_set_handler(EXCP_NUM_NP,
			 (unsigned long long)segment_not_present_handler);
	intr_set_handler(EXCP_NUM_SS, (unsigned long long)stack_fault_handler);
	intr_set_handler(EXCP_NUM_GP,
			 (unsigned long long)general_protection_handler);
	intr_set_handler(EXCP_NUM_PF, (unsigned long long)page_fault_handler);
	intr_set_handler(EXCP_NUM_MF,
		(unsigned long long)x87_fpu_floating_point_error_handler);
	intr_set_handler(EXCP_NUM_AC,
			 (unsigned long long)alignment_check_handler);
	intr_set_handler(EXCP_NUM_MC,
			 (unsigned long long)machine_check_handler);
	intr_set_handler(EXCP_NUM_XM,
			 (unsigned long long)simd_floating_point_handler);
	intr_set_handler(EXCP_NUM_VE,
			 (unsigned long long)virtualization_handler);
}
