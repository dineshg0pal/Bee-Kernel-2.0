// kernel.c

#include "vga.h"
#include "idt.h"

void kernel_main() {
    clear_screen();

    set_color(15, 0);
    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();

    print("IDT Loaded Successfully.\n\n");

    while (1) {
        // idle
    }
}
