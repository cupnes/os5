#include <asm/cpu.h>

	.code32

	.text

	.global	kern_init, idt, gdt, keyboard_handler, timer_handler, exception_handler, divide_error_handler, debug_handler, nmi_handler, breakpoint_handler, overflow_handler, bound_range_exceeded_handler, invalid_opcode_handler, device_not_available_handler, double_fault_handler, coprocessor_segment_overrun_handler, invalid_tss_handler, segment_not_present_handler, stack_fault_handler, general_protection_handler, page_fault_handler, x87_fpu_floating_point_error_handler, alignment_check_handler, machine_check_handler, simd_floating_point_handler, virtualization_handler, syscall_handler

	movl	$0x00080000, %esp

	lgdt	gdt_descr

	lea	ignore_int, %edx
	movl	$0x00080000, %eax
	movw	%dx, %ax
	movw	$0x8E00, %dx
	lea	idt, %edi
	mov	$256, %ecx
rp_sidt:
	movl	%eax, (%edi)
	movl	%edx, 4(%edi)
	addl	$8, %edi
	dec	%ecx
	jne	rp_sidt
	lidt	idt_descr

	pushl	$0
	pushl	$0
	pushl	$0
	pushl	$end
	pushl	$kern_init
	ret

end:
	jmp	end

keyboard_handler:
	pushal
	call	do_ir_keyboard
	popal
	iret

timer_handler:
	pushal
	call	do_ir_timer
	popal
	iret

exception_handler:
	popl	excp_error_code
	pushal
	call	do_exception
	popal
	iret

/* interrupt 0 (#DE) */
divide_error_handler:
	jmp	divide_error_handler
	iret

/* interrupt 1 (#DB) */
debug_handler:
	jmp	debug_handler
	iret

/* interrupt 2 */
nmi_handler:
	jmp	nmi_handler
	iret

/* interrupt 3 (#BP) */
breakpoint_handler:
	jmp	breakpoint_handler
	iret

/* interrupt 4 (#OF) */
overflow_handler:
	jmp	overflow_handler
	iret

/* interrupt 5 (#BR) */
bound_range_exceeded_handler:
	jmp	bound_range_exceeded_handler
	iret

/* interrupt 6 (#UD) */
invalid_opcode_handler:
	jmp	invalid_opcode_handler
	iret

/* interrupt 7 (#NM) */
device_not_available_handler:
	jmp	device_not_available_handler
	iret

/* interrupt 8 (#DF) */
double_fault_handler:
	jmp	double_fault_handler
	iret

/* interrupt 9 */
coprocessor_segment_overrun_handler:
	jmp	coprocessor_segment_overrun_handler
	iret

/* interrupt 10 (#TS) */
invalid_tss_handler:
	jmp	invalid_tss_handler
	iret

/* interrupt 11 (#NP) */
segment_not_present_handler:
	jmp	segment_not_present_handler
	iret

/* interrupt 12 (#SS) */
stack_fault_handler:
	jmp	stack_fault_handler
	iret

/* interrupt 13 (#GP) */
general_protection_handler:
	jmp	general_protection_handler
	iret

/* interrupt 14 (#PF) */
page_fault_handler:
	popl	excp_error_code
	pushal
	movl	%cr2, %eax
	pushl	%eax
	pushl	excp_error_code
	call	do_page_fault
	popl	%eax
	popl	%eax
	popal
	iret

/* interrupt 16 (#MF) */
x87_fpu_floating_point_error_handler:
	jmp	x87_fpu_floating_point_error_handler
	iret

/* interrupt 17 (#AC) */
alignment_check_handler:
	jmp	alignment_check_handler
	iret

/* interrupt 18 (#MC) */
machine_check_handler:
	jmp	machine_check_handler
	iret

/* interrupt 19 (#XM) */
simd_floating_point_handler:
	jmp	simd_floating_point_handler
	iret

/* interrupt 20 (#VE) */
virtualization_handler:
	jmp	virtualization_handler
	iret

/* interrupt 128 */
syscall_handler:
	pushl	%esp
	pushl	%ebp
	pushl	%esi
	pushl	%edi
	pushl	%edx
	pushl	%ecx
	pushl	%ebx
	pushl	%eax
	call	do_syscall
	popl	%ebx
	popl	%ebx
	popl	%ecx
	popl	%edx
	popl	%edi
	popl	%esi
	popl	%ebp
	popl	%esp
	iret

ignore_int:
	iret

	.data
idt_descr:
	.word	256*8-1		/* idt contains 256 entries */
	.long	idt

gdt_descr:
	.word	GDT_SIZE*8-1
	.long	gdt

	.balign	8
idt:
	.fill	256, 8, 0	/* idt is uninitialized */

gdt:
	.quad	0x0000000000000000	/* NULL descriptor */
	.quad	0x00cf9a000000ffff	/* 4GB(r-x:Code, DPL=0) */
	.quad	0x00cf92000000ffff	/* 4GB(rw-:Data, DPL=0) */
	.quad	0x00cffa000000ffff	/* 4GB(r-x:Code, DPL=3) */
	.quad	0x00cff2000000ffff	/* 4GB(rw-:Data, DPL=3) */
	.fill	GDT_SIZE-5, 8, 0

excp_error_code:
	.long	0x00000000
