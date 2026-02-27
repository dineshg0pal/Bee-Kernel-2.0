#ifndef ISR_H
#define ISR_H

#include <stdint.h>

/*
    Unified interrupt stack frame.
    This matches exactly what our assembly stubs push.
*/

typedef struct interrupt_frame {
    /* pushed by pusha */
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;      // original esp before pusha
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    /* pushed manually in stub */
    uint32_t int_no;
    uint32_t err_code;

    /* pushed automatically by CPU */
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} interrupt_frame_t;

/* C dispatcher */
void interrupt_dispatch(interrupt_frame_t* frame);

/* IDT install helpers (already used in kernel.c) */
void isr_install(void);

#endif
