// idt.c

#include "idt.h"
#include <stdint.h>

#define IDT_ENTRIES 256

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2+1)

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void idt_load(uint32_t);
extern void irq1_stub();

extern void irq0_stub();

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr   idtp;

/* ---------------- I/O ---------------- */

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait()
{
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

/* ---------------- GET CURRENT CS ---------------- */

static uint16_t get_cs()
{
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs;
}

/* ---------------- IDT ---------------- */

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].type_attr = flags;
}

/* ---------------- PIC REMAP ---------------- */

void pic_remap()
{
    uint8_t a1, a2;

    a1 = 0xFF;
    a2 = 0xFF;

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 4);
    io_wait();
    outb(PIC2_DATA, 2);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Unmask only IRQ1
    outb(PIC1_DATA, 0xFC);
    outb(PIC2_DATA, 0xFF);
}

/* ---------------- INIT ---------------- */

void idt_init()
{
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base  = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++)
        idt_set_gate(i, 0, 0, 0);

    pic_remap();

    uint16_t cs = get_cs();

    idt_set_gate(32, (uint32_t)irq0_stub, cs, 0x8E);
    idt_set_gate(33, (uint32_t)irq1_stub, cs, 0x8E);

    idt_load((uint32_t)&idtp);

    __asm__ volatile ("sti");
}
