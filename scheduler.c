#include "scheduler.h"
#include "klog.h"

static task_t* current = 0;

/* ---------------- INIT ---------------- */

void scheduler_init()
{
    current = task_get_current();
    klog("Scheduler initialized");
}

/* ---------------- ROUND ROBIN ---------------- */

task_t* scheduler_next()
{
    if (!current)
        return 0;

    if (current->next)
        current = current->next;
    else
        current = task_get_current();  // back to first task

    return current;
}
