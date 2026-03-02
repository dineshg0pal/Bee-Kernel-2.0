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
#include "paging.h"

#include "gdt.h"

extern void switch_to_user_mode();

/* Kernel stack (Ring 0) */
uint8_t kernel_stack[4096];
uint32_t kernel_stack_top = (uint32_t)&kernel_stack[4096];

/* User stack (Ring 3) */
uint8_t user_stack[4096];
uint32_t user_stack_top = (uint32_t)&user_stack[4096];
/* Global multiboot information pointer */
multiboot_info_t* g_multiboot_info = 0;

void kernel_main(uint32_t magic, uint32_t multiboot_addr)
{
    /* ---------------- BOOT VALIDATION ---------------- */

    if (magic != 0x2BADB002) {
        kernel_panic("Invalid Multiboot magic");
    }

    g_multiboot_info = (multiboot_info_t*)multiboot_addr;

    /* ---------------- BASIC SCREEN ---------------- */

    clear_screen();
    set_color(15, 0);
    print("Hello from Bee Kernel!\n");

    /* ---------------- IDT + ISR FIRST ---------------- */

    idt_init();
    isr_install();
    idt_load_now();

    /* ---------------- MEMORY SYSTEM ---------------- */

    pmm_init();

    /* Enable paging AFTER IDT is ready */
//    paging_init();

    /* ---------------- TEST PAGE FAULT ---------------- */
    
  /*  int* x = 0;
    *x = 5; */
    

    /* ---------------- HARDWARE INIT ---------------- */

    keyboard_init();
    pit_init(100);

    print("Interrupt system ready.\n");

    /* ---------------- ENABLE INTERRUPTS ---------------- */


    gdt_init();

    tss_set_kernel_stack(kernel_stack_top);

    __asm__ volatile("sti");

    print("Switching to user mode...\n");

    switch_to_user_mode();


    while (1)
       __asm__ volatile("hlt");
}
