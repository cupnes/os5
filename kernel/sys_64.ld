OUTPUT_FORMAT("binary");

MEMORY
{
	exec_file(r) : ORIGIN = 0x00000000, LENGTH = 1m
	ram(wx)      : ORIGIN = 0x00110000, LENGTH = 960k
}

SECTIONS
{
	.header : {
		QUAD(__bss_start)
		QUAD(__bss_end - __bss_start)
	} > exec_file

	.body : {
		*(.text)

		*(.rodata)

		*(.data)

		__bss_start = .;
		*(.bss)
		__bss_end = .;

		*(.eh_frame)
	} > ram AT> exec_file
}
