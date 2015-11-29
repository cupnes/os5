#include <excp.h>
#include <console_io.h>

void do_exception(void)
{
	put_str("exception\r\n");
	while (1);
}

void do_page_fault(unsigned int error_code, unsigned int address)
{
	put_str("page fault\r\n");
	put_str("error code: 0x");
	dump_hex(error_code, 8);
	put_str("\r\n");
	put_str("address   : 0x");
	dump_hex(address, 8);
	put_str("\r\n");
	while (1);
}
