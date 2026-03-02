#ifndef GDT_H
#define GDT_H

#include <stdint.h>

void gdt_init();
void tss_set_kernel_stack(uint32_t stack);

#endif
