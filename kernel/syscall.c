#include <syscall.h>
#include <kernel.h>
#include <timer.h>
#include <sched.h>
#include <fs.h>
#include <task.h>
#include <console_io.h>

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
	case SYSCALL_OPEN:
		result = (unsigned int)fs_open((char *)arg1);
		break;
	case SYSCALL_EXEC:
		task_init((struct file *)arg1);
		result = 0;
		break;
	}

	return result;
}
