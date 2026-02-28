#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_init(void);
uint32_t pmm_alloc_frame(void);
void pmm_free_frame(uint32_t addr);
uint32_t pmm_get_total_memory(void);

#endif
