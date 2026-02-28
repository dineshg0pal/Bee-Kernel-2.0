#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "pit.h"
#include "isr.h"
#include "panic.h"
#include "multiboot.h"
#include <stdint.h>
#include "pmm.h"


/* Global multiboot info pointer (read-only after init) */
multiboot_info_t* g_multiboot_info = 0;

void kernel_main(uint32_t magic, uint32_t multiboot_addr)
{
    /* ---------------- BOOT VALIDATION ---------------- */

    if (magic != 0x2BADB002) {
        kernel_panic("Invalid Multiboot magic");
    }

    g_multiboot_info = (multiboot_info_t*)multiboot_addr;

    pmm_init();
    /* ---------------- BASIC SCREEN ---------------- */

    clear_screen();
    set_color(15, 0);
    print("Hello from Bee Kernel!\n");

    /* ---------------- IDT + ISR ---------------- */

    idt_init();
    isr_install();
    idt_load_now();

    /* ---------------- HARDWARE INIT ---------------- */

    keyboard_init();
    pit_init(100);

    print("Interrupt system ready.\n");

    /* ---------------- ENABLE INTERRUPTS ---------------- */

    __asm__ volatile("sti");
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
