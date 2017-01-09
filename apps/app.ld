OUTPUT_FORMAT("binary");

SECTIONS
{
	. = 0x20000030;
	.text	: {
		*(.entry)
		*(.text)
	}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}
}
