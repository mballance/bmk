
MEMORY
{
	rom(rx)	 : ORIGIN = 0x10000, LENGTH = 0x00010000
	ram(rwx) : ORIGIN = 0x40000000, LENGTH = 0x00001000
}

SECTIONS
{
    /* default entry point */
	.text : {
		CREATE_OBJECT_SYMBOLS
		. += 0x40;
        *(.text.start)
/*        *(.text .text.* .gnu.linkonce.t.*) */
        *(.text .gnu.linkonce.t.*)
        *(.plt)
        *(.gnu.warning)
        *(.glue_7t) *(.glue_7)

        . = ALIGN(0x4);

        /* Static constructor / destructors */
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        *(.rodata .rodata.* .gnu.linkonce.r.*)

        *(.init)
        *(.fini)
        . = ALIGN(0x4);
    } >rom
    
	_dtb = .; PROVIDE(dtb = .);
	
    .rodata.dtb : { 
    	*(.rodata.dtb) 
    } >rom

    .init_array : {
        __init_array_start = .;
        _init = .;
        *(.init_array)
        . = ALIGN(0x4);
    } >rom
    __init_array_end = .;

    _etext = .;

    .ramvec : {
        *(.ramvec)
        . = ALIGN(0x4);
    } >ram

    . = ALIGN(16);

    .data ALIGN(0x10): {
        . = ALIGN(0x4);
        __data_load = LOADADDR(.data);
        __data_start = .;
        KEEP(*(.jcr))
        *(.got.plt) *(.got)
        *(.shdata)
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(4);
        _edata = .;
    } >ram AT>rom

    .bss ALIGN(0x4): {
        _bss_start = .;
        *(.shbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } >ram

	.heap ALIGN(0x10): {
		. = ALIGN(0x4);
		_boot_heap_base = .; PROVIDE(boot_heap_base = .);
	} > ram
	
	. = 0x52001000;
	PROVIDE(_stack_top = .);

/*
    . = ROM_BASE;
    . = ROM_BASE + 0x40;
    .text.start : { *(.text.start) }
 */
/*   . = ROM_BASE + 0x80; */
	
}
