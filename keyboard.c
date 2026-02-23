// keyboard.c

#include <stdint.h>
#include "vga.h"

#define PIC1_COMMAND 0x20
#define PIC_EOI      0x20

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

void irq1_handler()
{
    uint8_t scancode = inb(0x60);

    put_char('K');   // DEBUG MARKER

    outb(PIC1_COMMAND, PIC_EOI);
}
