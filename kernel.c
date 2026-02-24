#include "vga.h"
#include "idt.h"

extern char keyboard_getchar();

void kernel_main()
{
    clear_screen();
    set_color(15, 0);

    print("Hello from Bee Kernel!\n");
    print("Initializing IDT...\n");

    idt_init();

    print("IDT Loaded Successfully.\n\n");
    print("Structured input active.\n\n");

    char line_buffer[256];
    uint32_t line_index = 0;

    while (1)
    {
        char c = keyboard_getchar();

        if (!c)
            continue;

        if (c == '\b') {
            if (line_index > 0) {
                line_index--;
                vga_backspace();
            }
        } else if (c == '\n') {
            put_char('\n');
            line_index = 0;
        } else {
            put_char(c);
            if (line_index < sizeof(line_buffer)-1) {
                line_buffer[line_index++] = c;
            }
        }
    }
}
