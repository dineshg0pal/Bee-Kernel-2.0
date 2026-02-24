#include "vga.h"
#include "idt.h"
#include "keyboard.h"

void kernel_main() {
    clear_screen();
    set_color(15, 0);

    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();
    keyboard_init();   // IMPORTANT

    print("IDT Loaded Successfully.\n\n");
    print("Type something:\n\n");
    print("new version\n"); 
    while (1) {
      char c = keyboard_getchar();
      if (!c) continue;

      if (c == '\b') {
        vga_backspace();
      } else {
        put_char(c);
      }
    }
}
