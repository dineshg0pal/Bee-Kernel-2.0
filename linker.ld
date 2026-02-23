ENTRY(kernel_main)

SECTIONS
{
    . = 0x100000;   /* load address for GRUB */

    .multiboot : { *(.multiboot) }  /* GRUB header first */
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss) *(COMMON) }
}
