#include <stdint.h>
#include "vga.h"
#include "keyboard.h"

#define PIC1_COMMAND 0x20
#define PIC_EOI      0x20
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define INPUT_BUFFER_SIZE 256

/* ---------------- I/O ---------------- */

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* ---------------- ENABLE PS/2 TRANSLATION ---------------- */

static void keyboard_enable_translation() {

    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    outb(KEYBOARD_STATUS_PORT, 0x20);

    while (!(inb(KEYBOARD_STATUS_PORT) & 0x01));
    uint8_t config = inb(KEYBOARD_DATA_PORT);

    config |= (1 << 6);  // Enable translation

    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    outb(KEYBOARD_STATUS_PORT, 0x60);

    while (inb(KEYBOARD_STATUS_PORT) & 0x02);
    outb(KEYBOARD_DATA_PORT, config);
}

void keyboard_init() {
    keyboard_enable_translation();
}

/* ---------------- MODIFIERS ---------------- */

static uint8_t shift = 0;

/* ---------------- SCANCODE TABLE ---------------- */

static const char scancode_table[128] = {
    [0x02]='1',[0x03]='2',[0x04]='3',[0x05]='4',
    [0x06]='5',[0x07]='6',[0x08]='7',[0x09]='8',
    [0x0A]='9',[0x0B]='0',
    [0x0C]='-',[0x0D]='=',
    [0x0E]='\b',
    [0x0F]='\t',
    [0x10]='q',[0x11]='w',[0x12]='e',[0x13]='r',
    [0x14]='t',[0x15]='y',[0x16]='u',[0x17]='i',
    [0x18]='o',[0x19]='p',
    [0x1A]='[',[0x1B]=']',
    [0x1C]='\n',
    [0x1E]='a',[0x1F]='s',[0x20]='d',[0x21]='f',
    [0x22]='g',[0x23]='h',[0x24]='j',[0x25]='k',
    [0x26]='l',
    [0x27]=';',[0x28]='\'',[0x29]='`',
    [0x2B]='\\',
    [0x2C]='z',[0x2D]='x',[0x2E]='c',[0x2F]='v',
    [0x30]='b',[0x31]='n',[0x32]='m',
    [0x33]=',',[0x34]='.',[0x35]='/',
    [0x39]=' '
};

static const char scancode_shifted[128] = {
    [0x02]='!',[0x03]='@',[0x04]='#',[0x05]='$',
    [0x06]='%',[0x07]='^',[0x08]='&',[0x09]='*',
    [0x0A]='(',[0x0B]=')',
    [0x0C]='_',[0x0D]='+',
    [0x10]='Q',[0x11]='W',[0x12]='E',[0x13]='R',
    [0x14]='T',[0x15]='Y',[0x16]='U',[0x17]='I',
    [0x18]='O',[0x19]='P',
    [0x1A]='{',[0x1B]='}',
    [0x1E]='A',[0x1F]='S',[0x20]='D',[0x21]='F',
    [0x22]='G',[0x23]='H',[0x24]='J',[0x25]='K',
    [0x26]='L',
    [0x27]=':',[0x28]='"',[0x29]='~',
    [0x2B]='|',
    [0x2C]='Z',[0x2D]='X',[0x2E]='C',[0x2F]='V',
    [0x30]='B',[0x31]='N',[0x32]='M',
    [0x33]='<',[0x34]='>',[0x35]='?',
    [0x39]=' '
};

/* ---------------- BUFFER ---------------- */

static char buffer[INPUT_BUFFER_SIZE];
static uint32_t head = 0;
static uint32_t tail = 0;

static void buffer_push(char c) {
    uint32_t next = (head + 1) % INPUT_BUFFER_SIZE;
    if (next != tail) {
        buffer[head] = c;
        head = next;
    }
}

char keyboard_getchar() {
    if (head == tail) return 0;
    char c = buffer[tail];
    tail = (tail + 1) % INPUT_BUFFER_SIZE;
    return c;
}

/* ---------------- IRQ HANDLER ---------------- */

void keyboard_callback(void) {

    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    uint8_t released = scancode & 0x80;
    uint8_t code     = scancode & 0x7F;

    /* SHIFT press/release */
    if (code == 0x2A || code == 0x36) {
        shift = !released;
  
        return;
    }

    /* Ignore key releases */
    if (released) {
    
        return;
    }

    char c = shift ? scancode_shifted[code] : scancode_table[code];

    if (c)
        buffer_push(c);

 
}
