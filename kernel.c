#include "vga.h"
#include "idt.h"
#include "keyboard.h"
#include "pit.h"
#include "isr.h"
#include "panic.h"
#include "multiboot.h"
#include <stdint.h>
#include "pmm.h"
#include "paging.h"
#include "gdt.h"
#include "klog.h"
#include "kmsg.h"
#include "task.h"
#include "scheduler.h"

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

    kmsg_init();
    klog("Kernel booting");

    /* ---------------- IDT + ISR FIRST ---------------- */

    idt_init();
    isr_install();
    idt_load_now();

    klog("IDT initialized");

    /* ---------------- MEMORY SYSTEM ---------------- */

    pmm_init();
    klog("Physical memory manager initialized");

    /* ---------------- TASK SYSTEM ---------------- */

    task_init();

    /* Kernel bootstrap task */
    task_t* kernel_task = task_create();
    task_add(kernel_task);

    /* Example worker tasks */
    task_t* t1 = task_create();
    task_add(t1);

    task_t* t2 = task_create();
    task_add(t2);

    task_list_print();

    scheduler_init();

    klog("Task system initialized");

    /* ---------------- HARDWARE INIT ---------------- */

    keyboard_init();
    pit_init(100);

    klog("Hardware initialized");

    print("Interrupt system ready.\n");

    /* ---------------- ENABLE INTERRUPTS ---------------- */

    gdt_init();

    tss_set_kernel_stack(kernel_stack_top);

    __asm__ volatile("sti");

    klog("Interrupts enabled");

    print("Switching to user mode...\n");
    klog("Switching to user mode");

    switch_to_user_mode();

    while (1)
        __asm__ volatile("hlt");
}
