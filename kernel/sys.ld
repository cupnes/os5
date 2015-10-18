OUTPUT_FORMAT("binary");

SECTIONS
{
	. = 0x7e00;
	.text	: {*(.text)}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}

	. = 0x7e00 + 0x91fe;
	.sign	: {SHORT(0xbeef)}
}
