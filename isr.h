#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // pusha
    uint32_t int_no;
    uint32_t err_code;
} interrupt_frame_t;

void isr_install(void);
void interrupt_dispatch(interrupt_frame_t* frame);

#endif
