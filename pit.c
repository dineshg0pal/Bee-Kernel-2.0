#include "pit.h"
#include <stdint.h>

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIC1_COMMAND 0x20
#define PIC_EOI      0x20

#define PIT_BASE_FREQUENCY 1193182

static volatile uint32_t ticks = 0;
static uint32_t pit_frequency = 1000;

/* ---------------- I/O ---------------- */

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* ---------------- IRQ HANDLER ---------------- */

void irq0_handler()
{
    ticks++;
    outb(PIC1_COMMAND, PIC_EOI);
}

/* ---------------- INIT ---------------- */

void pit_init(uint32_t frequency)
{
    pit_frequency = frequency;

    uint32_t divisor = PIT_BASE_FREQUENCY / frequency;

    outb(PIT_COMMAND, 0x36);              // Channel 0, lobyte/hibyte, mode 3
    outb(PIT_CHANNEL0, divisor & 0xFF);   // Low byte
    outb(PIT_CHANNEL0, (divisor >> 8));   // High byte
}

uint32_t pit_get_ticks()
{
    return ticks;
}

void pit_sleep(uint32_t milliseconds)
{
    uint32_t target = ticks + milliseconds;
    while (ticks < target);
}
