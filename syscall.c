#include <stdint.h>
#include "syscall.h"

uint32_t syscall_dispatch(uint32_t num, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    switch(num)
    {
        case SYS_PRINT:
            // kprint((char*)arg1);
            break;

        case SYS_GETCHAR:
            // return keyboard_getchar();
            break;

        case SYS_MEMINFO:
            // meminfo_print();
            break;

        case SYS_ALLOC:
            // return kmalloc(arg1);
            break;

        case SYS_FREE:
            // kfree((void*)arg1);
            break;

        case SYS_UPTIME:
            // return timer_ticks();
            break;

        case SYS_KMSG:
            // kernel_log((char*)arg1);
            break;

        default:
            return 0;
    }

    return 0;
}
