#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"

void kernel_main() {
    clear_screen();
    set_color(15, 0);

    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();
    keyboard_init();

    print("IDT Loaded Successfully.\n\n");

    shell_init();   // <-- start the shell

    while (1) {
        char c = keyboard_getchar();
        if (!c) continue;

        shell_input(c);   // <-- route input to shell
    }
}
