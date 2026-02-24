// kernel.c

#include "vga.h"
#include "idt.h"

extern char keyboard_getchar();

void kernel_main()
{
    clear_screen();

    set_color(15, 0);
    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();

    print("IDT Loaded Successfully.\n\n");
    print("Structured input active.\n\n");

    while (1)
    {
        char c = keyboard_getchar();

        if (c)
            put_char(c);
    }
}
