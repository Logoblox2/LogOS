ENTRY(start)

SECTIONS
{
	/* Load virtual address */
	. = 1M;

	/* Place multiboot header first in the image */
	.boot ALIGN(4) :
	{
		KEEP(*(.multiboot))
		KEEP(*(.multiboot_header))
	}

	/* Code */
	.text ALIGN(4K) :
	{
		*(.text*)
	}

	/* Read-only data */
	.rodata ALIGN(4K) :
	{
		*(.rodata*)
	}

	/* Initialized data */
	.data ALIGN(4K) :
	{
		*(.data*)
	}

	/* Zero-initialized data */
	.bss ALIGN(4K) :
	{
		*(COMMON)
		*(.bss*)
	}
}