#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "pit.h"
#include "isr.h"

void kernel_main() {

    clear_screen();
    set_color(15,0);

    print("Hello from Bee Kernel!\n");

    idt_init();
    isr_install();

    keyboard_init();
    pit_init(100);

    print("Interrupts Ready.\n\n");    
   
    shell_init();

    while (1) {
        char c = keyboard_getchar();
        if (!c) continue;
        shell_input(c);
    }
}
