	.text

	.global	keyboard_handler
keyboard_handler:
	push	%rax
	push	%rcx
	push	%rdx
	push	%rbx
	push	%rbp
	push	%rsi
	push	%rdi
	push	%rsp
	call	do_ir_keyboard
	pop	%rsp
	pop	%rdi
	pop	%rsi
	pop	%rbp
	pop	%rbx
	pop	%rdx
	pop	%rcx
	pop	%rax
	iretq

	.global	exception_handler
exception_handler:
	jmp	exception_handler
	iretq

	.global	divide_error_handler
divide_error_handler:
	movq	%rsp, %rdi
	call	do_divide_error_handler
	iretq

	.global	debug_handler
debug_handler:
	movq	%rsp, %rdi
	call	do_debug_handler
	iretq

	.global	nmi_handler
nmi_handler:
	movq	%rsp, %rdi
	call	do_nmi_handler
	iretq

	.global	breakpoint_handler
breakpoint_handler:
	movq	%rsp, %rdi
	call	do_breakpoint_handler
	iretq

	.global	overflow_handler
overflow_handler:
	movq	%rsp, %rdi
	call	do_overflow_handler
	iretq

	.global	bound_range_exceeded_handler
bound_range_exceeded_handler:
	movq	%rsp, %rdi
	call	do_bound_range_exceeded_handler
	iretq

	.global	invalid_opcode_handler
invalid_opcode_handler:
	movq	%rsp, %rdi
	call	do_invalid_opcode_handler
	iretq

	.global	device_not_available_handler
device_not_available_handler:
	movq	%rsp, %rdi
	call	do_device_not_available_handler
	iretq

	.global	double_fault_handler
double_fault_handler:
	movq	%rsp, %rdi
	call	do_double_fault_handler
	iretq

	.global	coprocessor_segment_overrun_handler
coprocessor_segment_overrun_handler:
	movq	%rsp, %rdi
	call	do_coprocessor_segment_overrun_handler
	iretq

	.global	invalid_tss_handler
invalid_tss_handler:
	movq	%rsp, %rdi
	call	do_invalid_tss_handler
	iretq

	.global	segment_not_present_handler
segment_not_present_handler:
	movq	%rsp, %rdi
	call	do_segment_not_present_handler
	iretq

	.global	stack_fault_handler
stack_fault_handler:
	movq	%rsp, %rdi
	call	do_stack_fault_handler
	iretq

	.global	general_protection_handler
general_protection_handler:
	movq	%rsp, %rdi
	call	do_general_protection_handler
	iretq

	.global	page_fault_handler
page_fault_handler:
	movq	%rsp, %rdi
	call	do_page_fault_handler
	iretq

	.global	x87_fpu_floating_point_error_handler
x87_fpu_floating_point_error_handler:
	movq	%rsp, %rdi
	call	do_x87_fpu_floating_point_error_handler
	iretq

	.global	alignment_check_handler
alignment_check_handler:
	movq	%rsp, %rdi
	call	do_alignment_check_handler
	iretq

	.global	machine_check_handler
machine_check_handler:
	movq	%rsp, %rdi
	call	do_machine_check_handler
	iretq

	.global	simd_floating_point_handler
simd_floating_point_handler:
	movq	%rsp, %rdi
	call	do_simd_floating_point_handler
	iretq

	.global	virtualization_handler
virtualization_handler:
	movq	%rsp, %rdi
	call	do_virtualization_handler
	iretq
