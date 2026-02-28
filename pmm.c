#include "pmm.h"
#include "multiboot.h"
#include "panic.h"
#include <stdint.h>

/* Provided by linker */
extern uint32_t end;

/* From kernel.c */
extern multiboot_info_t* g_multiboot_info;

#define FRAME_SIZE 4096
#define MAX_MEMORY (1024 * 1024 * 1024) // 1GB max supported

static uint32_t total_memory = 0;
static uint32_t total_frames = 0;
static uint32_t used_frames = 0;

/* Bitmap stored right after kernel */
static uint32_t* bitmap = 0;

#define INDEX_FROM_BIT(a) (a / 32)
#define OFFSET_FROM_BIT(a) (a % 32)

/* ---------------- BITMAP HELPERS ---------------- */

static void set_frame(uint32_t frame)
{
    bitmap[INDEX_FROM_BIT(frame)] |= (1 << OFFSET_FROM_BIT(frame));
}

static void clear_frame(uint32_t frame)
{
    bitmap[INDEX_FROM_BIT(frame)] &= ~(1 << OFFSET_FROM_BIT(frame));
}

static int test_frame(uint32_t frame)
{
    return bitmap[INDEX_FROM_BIT(frame)] & (1 << OFFSET_FROM_BIT(frame));
}

/* ---------------- INIT ---------------- */

void pmm_init(void)
{
    if (!(g_multiboot_info->flags & (1 << 6))) {
        kernel_panic("No memory map from bootloader");
    }

    multiboot_memory_map_t* mmap =
        (multiboot_memory_map_t*)g_multiboot_info->mmap_addr;

    /* Calculate total memory */
    while ((uint32_t)mmap <
           g_multiboot_info->mmap_addr + g_multiboot_info->mmap_length)
    {
        if (mmap->type == 1) {
            uint32_t region_end = (uint32_t)(mmap->addr + mmap->len);
            if (region_end > total_memory)
                total_memory = region_end;
        }

        mmap = (multiboot_memory_map_t*)
               ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    if (total_memory > MAX_MEMORY)
        total_memory = MAX_MEMORY;

    total_frames = total_memory / FRAME_SIZE;

    /* Place bitmap right after kernel */
    bitmap = (uint32_t*)&end;

    uint32_t bitmap_size = INDEX_FROM_BIT(total_frames) + 1;

    for (uint32_t i = 0; i < bitmap_size; i++)
        bitmap[i] = 0xFFFFFFFF;  // mark all used initially

    used_frames = total_frames;

    /* Free usable regions */
    mmap = (multiboot_memory_map_t*)g_multiboot_info->mmap_addr;

    while ((uint32_t)mmap <
           g_multiboot_info->mmap_addr + g_multiboot_info->mmap_length)
    {
        if (mmap->type == 1) {

            uint32_t start = (uint32_t)mmap->addr;
            uint32_t end_addr = (uint32_t)(mmap->addr + mmap->len);

            for (uint32_t addr = start;
                 addr < end_addr;
                 addr += FRAME_SIZE)
            {
                uint32_t frame = addr / FRAME_SIZE;

                if (frame < total_frames) {
                    clear_frame(frame);
                    used_frames--;
                }
            }
        }

        mmap = (multiboot_memory_map_t*)
               ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    /* Reserve first 1MB */
    for (uint32_t addr = 0; addr < 0x100000; addr += FRAME_SIZE)
        set_frame(addr / FRAME_SIZE);

    /* Reserve kernel */
    uint32_t kernel_start = 0x100000;
    uint32_t kernel_end = (uint32_t)&end;

    for (uint32_t addr = kernel_start;
         addr < kernel_end;
         addr += FRAME_SIZE)
    {
        set_frame(addr / FRAME_SIZE);
    }
}

/* ---------------- ALLOC ---------------- */

uint32_t pmm_alloc_frame(void)
{
    for (uint32_t i = 0; i < total_frames; i++) {
        if (!test_frame(i)) {
            set_frame(i);
            used_frames++;
            return i * FRAME_SIZE;
        }
    }

    return 0; // out of memory
}

void pmm_free_frame(uint32_t addr)
{
    uint32_t frame = addr / FRAME_SIZE;
    clear_frame(frame);
    used_frames--;
}

uint32_t pmm_get_total_memory(void)
{
    return total_memory;
}
