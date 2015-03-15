OUTPUT_FORMAT("binary");

SECTIONS
{
	.text	: {*(.text)}
	.data	: {*(.data)}
	.bss	: {*(.bss)}

	. = 4094;
	.sign	: {SHORT(0x0000)}
}
