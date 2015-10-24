OUTPUT_FORMAT("binary");

SECTIONS
{
	. = 0x20000000;
	.text	: {*(.text)}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}

	. = 0x20000000 + 4094;
	.sign	: {SHORT(0xcafe)}
}
