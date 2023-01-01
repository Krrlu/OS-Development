ENTRY(main);
SECTIONS
{
    . = 0x7C00;
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
    .sig : AT(0x7DFE)
    {
        SHORT(0xaa55);
    }
    /DISCARD/ :
    {
        *(.note*);
        *(.iplt*);
        *(.igot*);
        *(.rel*);
        *(.comment);
/* add any unwanted sections spewed out by your version of gcc and flags here */
    }
}