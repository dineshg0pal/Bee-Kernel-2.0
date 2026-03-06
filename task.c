#include "task.h"
#include "klog.h"
#include "vga.h"

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

#define MAX_TASKS 16

static task_t task_pool[MAX_TASKS];
static uint32_t task_count = 0;

task_t* task_create()
{
    if (task_count >= MAX_TASKS)
        return 0;

    task_t* new_task = &task_pool[task_count++];

    new_task->pid = next_pid++;
    new_task->state = TASK_READY;
    new_task->next = 0;

    return new_task;
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


/* ---------------- Debug: Print Task List ---------------- */

static const char* task_state_string(uint32_t state)
{
    switch(state)
    {
        case TASK_RUNNING: return "RUNNING";
        case TASK_READY:   return "READY";
        case TASK_BLOCKED: return "BLOCKED";
        default:           return "UNKNOWN";
    }
}

void task_list_print()
{
    task_t* t = task_list;

    print("PID   STATE\n");

    while (t)
    {
        print_dec(t->pid);
        print("    ");
        print(task_state_string(t->state));
        print("\n");

        t = t->next;
    }
}
