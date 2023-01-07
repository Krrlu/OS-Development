ENTRY(main);
SECTIONS
{
    . = 0x40000;
    .text : AT(0x40000)
    {
        _text = .;
        *(.text);
    }
    .data : AT( ADDR(.text) + SIZEOF(.text) )
    {
        _data = .;
        *(.data);
    }
     .bss : AT( ADDR(.text) + SIZEOF(.text) + SIZEOF(.data) )
    {
  	    bss = .;
  	    *(.bss)
        *(COMMON)
  }
    
}