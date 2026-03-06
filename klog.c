#include "klog.h"
#include "vga.h"
#include "pit.h"

/* ------------------------------------------------ */
/* Simple Bee Kernel Logger                         */
/* ------------------------------------------------ */

#define KLOG_BUFFER_SIZE 4096

static char klog_buffer[KLOG_BUFFER_SIZE];
static int klog_index = 0;

/* small helper: print hex ticks */
static void klog_print_ticks(uint32_t ticks)
{
    char hex[11] = "0x00000000";
    const char* digits = "0123456789ABCDEF";

    for (int i = 0; i < 8; i++)
    {
        hex[9 - i] = digits[(ticks >> (i * 4)) & 0xF];
    }

    print(hex);
}

/* store log inside buffer */
static void klog_store(uint32_t ticks, const char* msg)
{
    /* store timestamp */

    if (klog_index < KLOG_BUFFER_SIZE - 12)
    {
        klog_buffer[klog_index++] = '[';

        for (int i = 7; i >= 0; i--)
        {
            char digit = "0123456789ABCDEF"[(ticks >> (i * 4)) & 0xF];
            klog_buffer[klog_index++] = digit;
        }

        klog_buffer[klog_index++] = ']';
        klog_buffer[klog_index++] = ' ';
    }

    /* store message */

    while (*msg && klog_index < KLOG_BUFFER_SIZE - 1)
    {
        klog_buffer[klog_index++] = *msg++;
    }

    if (klog_index < KLOG_BUFFER_SIZE - 1)
        klog_buffer[klog_index++] = '\n';

    klog_buffer[klog_index] = 0;
}

void klog(const char* msg)
{
    uint32_t ticks = pit_get_ticks();

    print("[");
    klog_print_ticks(ticks);
    print("] ");

    print(msg);
    print("\n");

    klog_store(ticks, msg);
}

/* dump entire log buffer */
void klog_dump()
{
    print("=== Bee Kernel Log ===\n");
    print(klog_buffer);
}
