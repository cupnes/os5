OUTPUT_FORMAT("binary");

SECTIONS
{
	.text	: {*(.text)}
	.data	: {*(.data)}

	. = 510;
	.sign	: {SHORT(0xaa55)}
}
