#include "isr.h"
#include "panic.h"
#include "keyboard.h"
#include "pit.h"
#include <stdint.h>

/* PIC ports */
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI      0x20

static uint16_t get_cs()
{
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs;
}


/* low-level port output */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* External assembly ISR/IRQ symbols */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* IDT setter (already implemented in idt.c) */
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

/* Install ISRs + IRQs into IDT */
void isr_install(void)
{
    uint16_t cs = get_cs();

    /* Exceptions 0–31 */
    idt_set_gate(0,  (uint32_t)isr0,  cs, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  cs, 0x8E);
    
    idt_set_gate(31, (uint32_t)isr31, cs, 0x8E);

    /* IRQs 32–47 */
    idt_set_gate(32, (uint32_t)irq0,  cs, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  cs, 0x8E);
    
    idt_set_gate(47, (uint32_t)irq15, cs, 0x8E);


}

/* Unified interrupt dispatcher */
void interrupt_dispatch(interrupt_frame_t* frame)
{
    uint32_t int_no = frame->int_no;

    /* CPU Exceptions */
    if (int_no < 32) {
        kernel_panic("CPU Exception");
        return;
    }

    /* Hardware IRQs */
    if (int_no >= 32 && int_no <= 47) {

        switch (int_no) {
            case 32:  // PIT
                pit_callback();
                break;

            case 33:  // Keyboard
                keyboard_callback();
                break;

            default:
                break;
        }

        /* Send EOI */
        if (int_no >= 40) {
            outb(PIC2_COMMAND, PIC_EOI);
        }

        outb(PIC1_COMMAND, PIC_EOI);
    }
}
