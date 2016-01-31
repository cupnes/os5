OUTPUT_FORMAT("binary");

SECTIONS
{
	. = 0x20000020;
	.text	: {*(.text)}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}
}
