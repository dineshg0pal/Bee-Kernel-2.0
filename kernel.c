#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "pit.h"

void kernel_main() {
    clear_screen();
    set_color(15, 0);

    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();
    keyboard_init();
    pit_init(100);   // 1000 Hz = 1ms per tick

    print("IDT Loaded Successfully.\n\n");

    shell_init();   // <-- start the shell

    while (1) {
        char c = keyboard_getchar();
        if (!c) continue;

        shell_input(c);   // <-- route input to shell
    }
}
