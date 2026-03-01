#ifndef PMM_H
#define PMM_H

#include <stdint.h>

void pmm_init(void);

uint32_t pmm_alloc_frame(void);
void pmm_free_frame(uint32_t addr);

/* Memory awareness */
uint32_t pmm_get_total_memory(void);
uint32_t pmm_get_used_memory(void);
uint32_t pmm_get_free_memory(void);
uint32_t pmm_get_reserved_memory(void);

uint32_t pmm_get_total_frames(void);
uint32_t pmm_get_used_frames(void);

#endif
