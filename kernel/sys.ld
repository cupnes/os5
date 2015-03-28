OUTPUT_FORMAT("binary");

SECTIONS
{
	.text	: {*(.text)}
	.data	: {*(.data)}
	.bss	: {*(.bss)}

	. = 8702;
	.sign	: {SHORT(0xbeef)}
}
