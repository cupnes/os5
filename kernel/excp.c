#include <excp.h>
#ifndef X86_64
#include <console_io.h>
#else
#include <fbcon.h>
#endif

void do_exception(void)
{
#ifndef X86_64
	put_str("exception\r\n");
#else
	puts("exception\r\n");
#endif
	while (1);
}

void do_page_fault(unsigned int error_code, unsigned int address)
{
#ifndef X86_64
	put_str("page fault\r\n");
	put_str("error code: 0x");
	dump_hex(error_code, 8);
	put_str("\r\n");
	put_str("address   : 0x");
	dump_hex(address, 8);
	put_str("\r\n");
#else
	(void)error_code;
	(void)address;
	puts("page fault\r\n");
#endif
	while (1);
}
