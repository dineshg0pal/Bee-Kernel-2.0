#include "scheduler.h"
#include "task.h"
#include "klog.h"

/* external assembly switcher */
extern void switch_task(uint32_t* old_esp, uint32_t new_esp);


/* ---------------- INIT ---------------- */

void scheduler_init()
{
    klog("Scheduler initialized");
}

/* ---------------- ROUND ROBIN ---------------- */

task_t* scheduler_next()
{
    task_t* current = task_get_current();

    if (!current)
        return 0;

    task_t* start = current;

    do
    {
        if (current->next)
            current = current->next;
        else
            current = task_get_head();

        if (current->state == TASK_READY)
            return current;

    } while (current != start);

    return start;
}
