ENTRY(main);
SECTIONS
{
    . = 0x40000;
    .text : AT(0x40000)
    {
        _text = .;
        *(.text);
        _text_end = .;
    }
    .data : AT( ADDR(.text) + SIZEOF(.text) )
    {
        _data = .;
        *(.bss);
        *(.bss*);
        *(.data);
        *(.rodata*);
        *(COMMON)
        _data_end = .;
    }
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
    }
}