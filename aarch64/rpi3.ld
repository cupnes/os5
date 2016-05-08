/* The original of this is in
 * https://github.com/swarren/rpi-3-aarch64-demo.git
 */

OUTPUT_FORMAT("elf64-littleaarch64", "elf64-littleaarch64", "elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
ENTRY(_start)
SECTIONS
{
	. = 0x00080000;

	. = ALIGN(8);
	.text :
	{
		boot.o (.text*)
		*(.text*)
	}

	. = ALIGN(8);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

	. = ALIGN(8);
	.data : {
		*(.data*)
	}

	. = ALIGN(8);

	.rel_dyn_start :
	{
		*(.__rel_dyn_start)
	}

	.rela.dyn : {
		*(.rela*)
	}

	.rel_dyn_end :
	{
		*(.__rel_dyn_end)
	}

	_end = .;

	. = ALIGN(8);

	.bss_start : {
		KEEP(*(.__bss_start));
	}

	.bss : {
		*(.bss*)
		 . = ALIGN(8);
	}

	.bss_end : {
		KEEP(*(.__bss_end));
	}

	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}
