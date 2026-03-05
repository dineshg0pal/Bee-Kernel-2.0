#include "klog.h"
#include "vga.h"

/* ------------------------------------------------ */
/* Simple Bee Kernel Logger                         */
/* ------------------------------------------------ */

#define KLOG_BUFFER_SIZE 4096

static char klog_buffer[KLOG_BUFFER_SIZE];
static int klog_index = 0;

/* very small string copy */
static void klog_store(const char* msg)
{
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
    print("[BEE] ");
    print(msg);
    print("\n");

    klog_store(msg);
}

/* dump entire log buffer */
void klog_dump()
{
    print("=== Bee Kernel Log ===\n");
    print(klog_buffer);
}
