#ifndef _EXCP_H_
#define _EXCP_H_

#define EXCEPTION_MAX	21
#define EXCP_NUM_DE	0
#define EXCP_NUM_DB	1
#define EXCP_NUM_NMI	2
#define EXCP_NUM_BP	3
#define EXCP_NUM_OF	4
#define EXCP_NUM_BR	5
#define EXCP_NUM_UD	6
#define EXCP_NUM_NM	7
#define EXCP_NUM_DF	8
#define EXCP_NUM_CSO	9
#define EXCP_NUM_TS	10
#define EXCP_NUM_NP	11
#define EXCP_NUM_SS	12
#define EXCP_NUM_GP	13
#define EXCP_NUM_PF	14
#define EXCP_NUM_MF	16
#define EXCP_NUM_AC	17
#define EXCP_NUM_MC	18
#define EXCP_NUM_XM	19
#define EXCP_NUM_VE	20

extern unsigned char exception_handler;
extern unsigned char divide_error_handler;
extern unsigned char debug_handler;
extern unsigned char nmi_handler;
extern unsigned char breakpoint_handler;
extern unsigned char overflow_handler;
extern unsigned char bound_range_exceeded_handler;
extern unsigned char invalid_opcode_handler;
extern unsigned char device_not_available_handler;
extern unsigned char double_fault_handler;
extern unsigned char coprocessor_segment_overrun_handler;
extern unsigned char invalid_tss_handler;
extern unsigned char segment_not_present_handler;
extern unsigned char stack_fault_handler;
extern unsigned char general_protection_handler;
extern unsigned char page_fault_handler;
extern unsigned char x87_fpu_floating_point_error_handler;
extern unsigned char alignment_check_handler;
extern unsigned char machine_check_handler;
extern unsigned char simd_floating_point_handler;
extern unsigned char virtualization_handler;

void do_exception(void);

#endif /* _EXCP_H_ */
