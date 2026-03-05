#include "klog.h"
#include "vga.h"

/* ------------------------------------------------ */
/* Internal helper                                  */
/* ------------------------------------------------ */

static void print_prefix(int level)
{
    switch (level)
    {
        case KLOG_INFO:
            print("[INFO] ");
            break;

        case KLOG_MEM:
            print("[MEM ] ");
            break;

        case KLOG_INT:
            print("[INT ] ");
            break;

        case KLOG_USER:
            print("[USER] ");
            break;

        case KLOG_ERR:
            print("[ERR ] ");
            break;

        default:
            print("[LOG ] ");
            break;
    }
}

/* ------------------------------------------------ */
/* Compatibility logger (old system)                */
/* ------------------------------------------------ */

void klog(const char* msg)
{
    print("[BEE] ");
    print(msg);
    print("\n");
}

/* ------------------------------------------------ */
/* Level based logger                               */
/* ------------------------------------------------ */

void klog_level(int level, const char* msg)
{
    print_prefix(level);
    print(msg);
    print("\n");
}
