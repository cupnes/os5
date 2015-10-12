OUTPUT_FORMAT("binary");

SECTIONS
{
	.text	: {*(.text)}
	.rodata	: {
		*(.strings)
		*(.rodata)
		*(.rodata.*)
	}
	.data	: {*(.data)}
	.bss	: {*(.bss)}

	. = 510;
	.sign	: {SHORT(0xaa55)}
}
