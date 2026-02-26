#include "panic.h"
#include "vga.h"

void kernel_panic(const char* message)
{
    set_color(15, 4);   // white on red
    clear_screen();

    print("=========== KERNEL PANIC ===========\n\n");
    print(message);
    print("\n\nSystem Halted.");

    __asm__ volatile("cli");

    while (1)
        __asm__ volatile("hlt");
}
