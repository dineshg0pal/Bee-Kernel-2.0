// keyboard.c

#include <stdint.h>
#include "vga.h"

#define PIC1_COMMAND 0x20
#define PIC_EOI      0x20
#define KEYBOARD_DATA_PORT 0x60

#define INPUT_BUFFER_SIZE 256

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

/* ---------------- RING BUFFER ---------------- */

static char input_buffer[INPUT_BUFFER_SIZE];
static uint32_t head = 0;
static uint32_t tail = 0;

static void buffer_push(char c)
{
    uint32_t next = (head + 1) % INPUT_BUFFER_SIZE;
    if (next != tail) {
        input_buffer[head] = c;
        head = next;
    }
}

char keyboard_getchar()
{
    if (head == tail)
        return 0;

    char c = input_buffer[tail];
    tail = (tail + 1) % INPUT_BUFFER_SIZE;
    return c;
}

/* ---------------- IRQ HANDLER ---------------- */

void irq1_handler()
{
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    char c = scancode_table[scancode];

    if (c)
        buffer_push(c);

    outb(PIC1_COMMAND, PIC_EOI);
}
