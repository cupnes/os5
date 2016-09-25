#ifndef _APP_KERNEL_H_
#define _APP_KERNEL_H_

#include "../../kernel/include/kernel.h"
#include "../../kernel/include/io_port.h"
#include "../../kernel/include/console_io.h"

unsigned int syscall(unsigned int syscall_id, unsigned int arg1, unsigned int arg2, unsigned int arg3);
void exit(void);

#endif /* _APP_KERNEL_H_ */
