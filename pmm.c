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
    total_memory = 0;
    total_frames = 0;
    used_frames  = 0;

    if (!(g_multiboot_info->flags & (1 << 6))) {
        kernel_panic("No memory map from bootloader");
    }

    multiboot_memory_map_t* mmap =
        (multiboot_memory_map_t*)g_multiboot_info->mmap_addr;

    /* ---- Calculate total memory ---- */
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

    /* ---- Place bitmap after kernel (aligned) ---- */

    /* ---- Correct bitmap sizing ---- */
    uint32_t bitmap_uint32 = (total_frames + 31) / 32;
    uint32_t bitmap_bytes  = bitmap_uint32 * sizeof(uint32_t);

    /* Place bitmap right after kernel end */
    bitmap = (uint32_t*)(((uint32_t)&end + 0xFFF) & 0xFFFFF000);

    /* Calculate how many frames bitmap occupies */
    uint32_t bitmap_frames =
       (bitmap_bytes + FRAME_SIZE - 1) / FRAME_SIZE;



    /* ---- Mark ALL frames used initially ---- */
    for (uint32_t i = 0; i < bitmap_uint32; i++)
        bitmap[i] = 0xFFFFFFFF;

    used_frames = total_frames;

    /* ---- Free usable regions ---- */
    mmap = (multiboot_memory_map_t*)g_multiboot_info->mmap_addr;

    while ((uint32_t)mmap <
           g_multiboot_info->mmap_addr + g_multiboot_info->mmap_length)
    {
        if (mmap->type == 1)
        {
            uint32_t start = (uint32_t)mmap->addr;
            uint32_t end_addr = (uint32_t)(mmap->addr + mmap->len);

            for (uint32_t addr = start;
                 addr < end_addr;
                 addr += FRAME_SIZE)
            {
                uint32_t frame = addr / FRAME_SIZE;

                if (frame < total_frames && test_frame(frame)) {
                    clear_frame(frame);
                    used_frames--;
                }
            }
        }

        mmap = (multiboot_memory_map_t*)
               ((uint32_t)mmap + mmap->size + sizeof(uint32_t));
    }

    /* ---- Reserve first 1MB ---- */
    for (uint32_t addr = 0; addr < 0x100000; addr += FRAME_SIZE)
    {
        uint32_t frame = addr / FRAME_SIZE;
        if (!test_frame(frame)) {
            set_frame(frame);
            used_frames++;
        }
    }

    /* ---- Reserve kernel ---- */
    uint32_t kernel_start = 0x100000;
    uint32_t kernel_end = (uint32_t)&end;

    for (uint32_t addr = kernel_start;
         addr < kernel_end;
         addr += FRAME_SIZE)
    {
        uint32_t frame = addr / FRAME_SIZE;
        if (!test_frame(frame)) {
            set_frame(frame);
            used_frames++;
        }
    }

    /* ---- Reserve bitmap itself ---- */
    uint32_t bitmap_start = (uint32_t)bitmap;
    uint32_t bitmap_end = bitmap_start + (bitmap_frames * FRAME_SIZE);

    for (uint32_t addr = bitmap_start;
         addr < bitmap_end;
         addr += FRAME_SIZE)
    {
        uint32_t frame = addr / FRAME_SIZE;
        if (!test_frame(frame)) {
            set_frame(frame);
            used_frames++;
        }
    }
}

/* ---------------- ALLOC ---------------- */

uint32_t pmm_alloc_frame(void)
{
    if (used_frames >= total_frames)
        return 0; // Out of memory

    for (uint32_t i = 0; i < total_frames; i++)
    {
        if (!test_frame(i))
        {
            set_frame(i);
            used_frames++;

            return i * FRAME_SIZE;
        }
    }

    return 0; // No free frame found
}


void pmm_free_frame(uint32_t addr)
{
    if (addr % FRAME_SIZE != 0)
        return;

    uint32_t frame = addr / FRAME_SIZE;

    if (frame >= total_frames)
        return;

    if (!test_frame(frame))
        return;  // prevent double free

    clear_frame(frame);
    used_frames--;
}


uint32_t pmm_get_used_memory(void)
{
    return used_frames * FRAME_SIZE;
}

uint32_t pmm_get_free_memory(void)
{
    return (total_frames - used_frames) * FRAME_SIZE;
}

uint32_t pmm_get_reserved_memory(void)
{
    return total_memory - pmm_get_free_memory() - pmm_get_used_memory();
}

uint32_t pmm_get_total_frames(void)
{
    return total_frames;
}

uint32_t pmm_get_used_frames(void)
{
    return used_frames;
}

uint32_t pmm_get_total_memory(void)
{
    return total_memory;
}
