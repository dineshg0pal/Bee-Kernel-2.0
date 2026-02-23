// keyboard.c

#include <stdint.h>
#include "vga.h"

#define PIC1_COMMAND 0x20
#define PIC_EOI      0x20

#define KEYBOARD_DATA_PORT 0x60

/* ---------------- I/O ---------------- */

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* ---------------- SCANCODE TABLE ---------------- */
/* Set 1 keyboard scancodes */

static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,
    '\\',
    'z','x','c','v','b','n','m',',','.','/',
    0,
    '*',
    0,
    ' ',
};

/* ---------------- HANDLER ---------------- */

void irq1_handler()
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    /* Ignore key release (break codes) */
    if (scancode & 0x80) {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    char c = scancode_table[scancode];

    if (c) {
        put_char(c);
    }

    /* Send End Of Interrupt */
    outb(PIC1_COMMAND, PIC_EOI);
}
