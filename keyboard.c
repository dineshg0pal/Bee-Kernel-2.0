#include <stdint.h>
#include "vga.h"
#include "keyboard.h"

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

/* ---------------- MODIFIERS ---------------- */
static uint8_t shift = 0;
static uint8_t caps  = 0;
static uint8_t ctrl  = 0;

/* ---------------- SCANCODE TABLES ---------------- */
static const char scancode_table[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',
    0,'*',0,' '
};

static const char scancode_shifted[128] = {
    0, 27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',
    0,'*',0,' '
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

    /* Handle key release */
    uint8_t released = scancode & 0x80;
    uint8_t code = scancode & 0x7F;

    /* Modifier keys */
    switch(code) {
        case 0x2A: // Left Shift
        case 0x36: // Right Shift
            shift = !released;
            break;
        case 0x3A: // Caps Lock
            if(!released) caps = !caps;
            break;
        case 0x1D: // Ctrl
            ctrl = !released;
            break;
    }

    if(released) {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    /* Ignore unknown scancodes */
    if(code >= 128) {
        outb(PIC1_COMMAND, PIC_EOI);
        return;
    }

    /* Translate to character */
    char c;
    if(shift)
        c = scancode_shifted[code];
    else
        c = scancode_table[code];

    /* Apply Caps Lock for letters */
    if(c >= 'a' && c <= 'z') {
        if(shift ^ caps) c -= 32; // make uppercase
    }

    /* Ctrl combinations (example Ctrl+C prints ^C) */
    if(ctrl) {
        if(c >= 'a' && c <= 'z') c -= 96; // ASCII control code
    }

    if(c)
        buffer_push(c);

    /* Send EOI */
    outb(PIC1_COMMAND, PIC_EOI);
}
