OUTPUT_FORMAT("binary");

SECTIONS
{
	. = 0x0000000000110000;
	.text	: {*(.text)}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}
}
