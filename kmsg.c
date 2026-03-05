#include "kmsg.h"
#include <stdint.h>

/* ------------------------------------------------ */
/* Kernel message buffer                            */
/* ------------------------------------------------ */

#define KMSG_SIZE 4096

static char kmsg_buffer[KMSG_SIZE];
static uint32_t kmsg_index = 0;

void kmsg_init()
{
    kmsg_index = 0;
}

/* append message to buffer */

void kmsg_write(const char* msg)
{
    while (*msg)
    {
        if (kmsg_index >= KMSG_SIZE - 1)
            return;

        kmsg_buffer[kmsg_index++] = *msg++;
    }

    if (kmsg_index < KMSG_SIZE - 1)
        kmsg_buffer[kmsg_index++] = '\n';
}

/* return pointer to full log */

const char* kmsg_get_buffer()
{
    return kmsg_buffer;
}
