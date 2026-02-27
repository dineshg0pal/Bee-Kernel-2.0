#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "pit.h"
#include "isr.h"

void kernel_main() {

    /* ---------------- BASIC SCREEN ---------------- */

    clear_screen();
    set_color(15, 0);

    print("Hello from Bee Kernel!\n");

    /* ---------------- IDT + ISR ---------------- */

    idt_init();        // Setup IDT entries
    isr_install();     // Install CPU exceptions + IRQs
    idt_load_now();    // Load IDTR register

    /* ---------------- HARDWARE INIT ---------------- */

    keyboard_init();   // PS/2 config
     pit_init(100);  // Keep disabled for now (enable later)

    print("Interrupt system ready.\n");

    /* ---------------- ENABLE INTERRUPTS ---------------- */

    __asm__ volatile("sti");   // Enable AFTER everything ready

    print("Interrupts enabled.\n\n");

    /* ---------------- SHELL ---------------- */

    shell_init();

    /* ---------------- MAIN LOOP ---------------- */

    while (1) {
        char c = keyboard_getchar();
        if (!c)
            continue;

        shell_input(c);
    }
}
