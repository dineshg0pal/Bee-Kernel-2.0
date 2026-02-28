#include "isr.h"
#include "panic.h"
#include "keyboard.h"
#include "pit.h"
#include <stdint.h>

/* PIC ports */
#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI      0x20

/* ============================= */
/* LOW LEVEL PORT WRITE          */
/* ============================= */
static inline void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

/* ============================= */
/* GET CURRENT CODE SEGMENT      */
/* ============================= */
static uint16_t get_cs()
{
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r"(cs));
    return cs;
}

/* ============================= */
/* EXTERNAL ISR SYMBOLS          */
/* ============================= */
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

/* ============================= */
/* EXTERNAL IRQ SYMBOLS          */
/* ============================= */
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

/* ============================= */
/* IDT SETTER (from idt.c)       */
/* ============================= */
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

/* ============================= */
/* SAFE PAGE FAULT HANDLER       */
/* ============================= */
static void page_fault_handler(interrupt_frame_t* frame)
{
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

    /* DO NOT PRINT HERE */
    /* DO NOT CALL PANIC */
    /* Just freeze safely */

    while (1)
        __asm__ volatile("hlt");
}

/* ============================= */
/* INTERRUPT DISPATCHER          */
/* ============================= */
void interrupt_dispatch(interrupt_frame_t* frame)
{
    uint32_t int_no = frame->int_no;

    /* -------- PAGE FAULT -------- */
    if (int_no == 14) {
        page_fault_handler(frame);
        return;
    }

    /* -------- OTHER CPU EXCEPTIONS -------- */
    if (int_no < 32) {
        while (1)
            __asm__ volatile("hlt");
    }

    /* -------- HARDWARE IRQs -------- */
    if (int_no >= 32 && int_no <= 47) {

        switch (int_no) {

            case 32:  /* PIT */
                pit_callback();
                break;

            case 33:  /* Keyboard */
                keyboard_callback();
                break;

            default:
                break;
        }

        /* Send End Of Interrupt */
        if (int_no >= 40)
            outb(PIC2_COMMAND, PIC_EOI);

        outb(PIC1_COMMAND, PIC_EOI);
    }
}

static void pic_remap(void)
{
    uint8_t a1, a2;

    a1 = 0;  // we don't care about masks for now
    a2 = 0;

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);  // Master PIC vector offset (32)
    outb(0xA1, 0x28);  // Slave PIC vector offset (40)

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, a1);
    outb(0xA1, a2);
}



/* ============================= */
/* INSTALL ISRs + IRQs INTO IDT  */
/* ============================= */
void isr_install(void)
{
    pic_remap();

    uint16_t cs = get_cs();

    /* Exceptions 0–31 */
    idt_set_gate(0,  (uint32_t)isr0,  cs, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  cs, 0x8E);
    idt_set_gate(2,  (uint32_t)isr2,  cs, 0x8E);
    idt_set_gate(3,  (uint32_t)isr3,  cs, 0x8E);
    idt_set_gate(4,  (uint32_t)isr4,  cs, 0x8E);
    idt_set_gate(5,  (uint32_t)isr5,  cs, 0x8E);
    idt_set_gate(6,  (uint32_t)isr6,  cs, 0x8E);
    idt_set_gate(7,  (uint32_t)isr7,  cs, 0x8E);
    idt_set_gate(8,  (uint32_t)isr8,  cs, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  cs, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, cs, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, cs, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, cs, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, cs, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, cs, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, cs, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, cs, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, cs, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, cs, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, cs, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, cs, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, cs, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, cs, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, cs, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, cs, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, cs, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, cs, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, cs, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, cs, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, cs, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, cs, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, cs, 0x8E);

    /* IRQs 32–47 */
    idt_set_gate(32, (uint32_t)irq0,  cs, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  cs, 0x8E);
    idt_set_gate(34, (uint32_t)irq2,  cs, 0x8E);
    idt_set_gate(35, (uint32_t)irq3,  cs, 0x8E);
    idt_set_gate(36, (uint32_t)irq4,  cs, 0x8E);
    idt_set_gate(37, (uint32_t)irq5,  cs, 0x8E);
    idt_set_gate(38, (uint32_t)irq6,  cs, 0x8E);
    idt_set_gate(39, (uint32_t)irq7,  cs, 0x8E);
    idt_set_gate(40, (uint32_t)irq8,  cs, 0x8E);
    idt_set_gate(41, (uint32_t)irq9,  cs, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, cs, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, cs, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, cs, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, cs, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, cs, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, cs, 0x8E);
}
