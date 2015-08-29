#include <sched.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <shell.h>

void schedule(void)
{
	if (global_counter % 2) {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		__asm__("ljmp	$0x20, $0");
	} else {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		shell_context_switch();
	}
}
