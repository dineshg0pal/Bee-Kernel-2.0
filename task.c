#include "task.h"
#include "klog.h"

/* ---------------- Globals ---------------- */

static uint32_t next_pid = 1;

static task_t kernel_task;

/* task list head */
static task_t* task_list = 0;

/* current running task */
static task_t* current_task = 0;


/* ---------------- Init ---------------- */

void task_init()
{
    kernel_task.pid = next_pid++;
    kernel_task.state = TASK_RUNNING;
    kernel_task.next = 0;

    task_list = &kernel_task;
    current_task = &kernel_task;

    klog("Task system initialized");
}


/* ---------------- Create Task ---------------- */

task_t* task_create()
{
    static task_t new_task;

    new_task.pid = next_pid++;
    new_task.state = TASK_READY;
    new_task.next = 0;

    return &new_task;
}


/* ---------------- Add Task ---------------- */

void task_add(task_t* task)
{
    task_t* t = task_list;

    while (t->next)
        t = t->next;

    t->next = task;

    klog("New task added");
}


/* ---------------- Get Current ---------------- */

task_t* task_get_current()
{
    return current_task;
}
