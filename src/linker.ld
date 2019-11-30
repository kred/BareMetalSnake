MEMORY
{
    ram : ORIGIN = 0x8000, LENGTH = 0x200000
}

ENTRY(_start)

SECTIONS
{
    . = 0x0;
    .text : 
    {
        *(.text.boot) 
        *(.text*)
        *(.bss*)
    } >ram

    .bss :
    {
        *(.bss*)
    } >ram

    /DISCARD/ : 
    {
        *(.comment) 
        *(.gnu*) 
        *(.note*) 
        *(.eh_frame*)
        *(.ARM*)
        *(.debug*)
    }
}