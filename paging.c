#include "paging.h"
#include "pmm.h"
#include "panic.h"
#include <stdint.h>

#define PAGE_PRESENT  0x1
#define PAGE_RW       0x2
#define PAGE_SIZE     4096

static uint32_t* page_directory;
static uint32_t* first_page_table;

extern void load_page_directory(uint32_t);
extern void enable_paging(void);

void paging_init(void)
{
    page_directory = (uint32_t*)pmm_alloc_frame();
    first_page_table = (uint32_t*)pmm_alloc_frame();

    if (!page_directory || !first_page_table)
        while (1) __asm__ volatile("hlt");

    /* Clear them */
    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0;
        first_page_table[i] = 0;
    }

    /* Identity map first 4MB */
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] =
            (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    /* Protect null page */
    first_page_table[0] = 0;

    /* Link page table */
    page_directory[0] =
        ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    load_page_directory((uint32_t)page_directory);
    enable_paging();
}
