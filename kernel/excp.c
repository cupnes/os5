#include <excp.h>
#include <console_io.h>

void do_exception(void)
{
	put_str("exception\r\n");
	while (1);
}
