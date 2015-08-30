#include <sched.h>
#include <io_port.h>
#include <intr.h>
#include <timer.h>
#include <shell.h>
#include <uptime.h>

void schedule(void)
{
	if (global_counter % 2) {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		uptime_context_switch();
	} else {
		outb_p(IOADR_MPIC_OCW2_BIT_MANUAL_EOI | INTR_IR_TIMER,
		       IOADR_MPIC_OCW2);
		shell_context_switch();
	}
}
