#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* initialize IDT + PIC */

void idt_init();

void idt_load_now(void);

/* allow external modules (ISR installer) */
void idt_set_gate(
    uint8_t num,
    uint32_t base,
    uint16_t sel,
    uint8_t flags
);

#endif
