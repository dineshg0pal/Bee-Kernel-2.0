// kernel.c
#include "vga.h"

void kernel_main() {
    clear_screen();

    set_color(15, 0); // White on black
    print("Hello from Bee Kernel!\n");
    print("Welcome to the VGA console.\n\n");

    int line = 0;

    while (1) {
        print("Line ");
        char c = '0' + (line % 10);
        put_char(c);
        print("\n");
        line++;
    }
}
