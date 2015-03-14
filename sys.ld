OUTPUT_FORMAT("binary");

SECTIONS
{
	.text	: {*(.text)}
	.data	: {*(.data)}

	. = 4094;
	.sign	: {SHORT(0x0000)}
}
