OUTPUT_FORMAT("binary");

MEMORY
{
	bin(wxa)  : o = 0x00000000, l = 1m
	body(wxa) : o = 0x00110000, l = 960k
}

SECTIONS
{
	.header : {
		QUAD(__bss_start)
		QUAD(__bss_end - __bss_start)
	} > bin

	.kernel : {
		*(.text)

		*(.rodata)

		*(.data)

		__bss_start = .;
		*(.bss)
		__bss_end = .;

		*(.eh_frame)
	} > body AT> bin
}
