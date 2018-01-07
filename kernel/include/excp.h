#ifndef _EXCP_H_
#define _EXCP_H_

enum {
	EXCP_NUM_DE,
	EXCP_NUM_DB,
	EXCP_NUM_NMI,
	EXCP_NUM_BP,
	EXCP_NUM_OF,
	EXCP_NUM_BR,
	EXCP_NUM_UD,
	EXCP_NUM_NM,
	EXCP_NUM_DF,
	EXCP_NUM_CSO,
	EXCP_NUM_TS,
	EXCP_NUM_NP,
	EXCP_NUM_SS,
	EXCP_NUM_GP,
	EXCP_NUM_PF,
	EXCP_NUM_MF = 16,
	EXCP_NUM_AC,
	EXCP_NUM_MC,
	EXCP_NUM_XM,
	EXCP_NUM_VE,
	EXCEPTION_MAX
};

void exception_handler(void);
void divide_error_handler(void);
void debug_handler(void);
void nmi_handler(void);
void breakpoint_handler(void);
void overflow_handler(void);
void bound_range_exceeded_handler(void);
void invalid_opcode_handler(void);
void device_not_available_handler(void);
void double_fault_handler(void);
void coprocessor_segment_overrun_handler(void);
void invalid_tss_handler(void);
void segment_not_present_handler(void);
void stack_fault_handler(void);
void general_protection_handler(void);
void page_fault_handler(void);
void x87_fpu_floating_point_error_handler(void);
void alignment_check_handler(void);
void machine_check_handler(void);
void simd_floating_point_handler(void);
void virtualization_handler(void);

void do_exception(void);
void do_page_fault(unsigned int error_code, unsigned int address);

#endif /* _EXCP_H_ */
