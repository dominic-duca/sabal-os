
ENTRY(_start)

PAGE_SIZE = CONSTANT(MAXPAGESIZE);

SECTIONS {
	. = 2M;

	.text 	:	ALIGN(PAGE_SIZE) {
		*(.multiboot)
		*(.text)
	}

	.rodata	:	ALIGN(PAGE_SIZE) {
		*(.rodata)
	}

	.data 	:	ALIGN(PAGE_SIZE) {
		*(.data)
	}
    
	.bss	:	ALIGN(PAGE_SIZE) {
		*(COMMON)
		*(.bss)
	}
}
