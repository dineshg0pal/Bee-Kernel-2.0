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

extern void switch_task(uint32_t* old_esp, uint32_t new_esp);

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

#define TASK_STACK_SIZE 4096

static uint8_t task_stacks[MAX_TASKS][TASK_STACK_SIZE];

task_t* task_create(void(*entry)())
{
    if (task_count >= MAX_TASKS)
        return 0;

    task_t* new_task = &task_pool[task_count];

    new_task->pid = next_pid++;
    new_task->state = TASK_READY;
    new_task->next = 0;

    /* setup task stack */
    uint32_t stack_top = (uint32_t)&task_stacks[task_count][TASK_STACK_SIZE];

    stack_top -= 4;
    *(uint32_t*)stack_top = (uint32_t)entry;

    new_task->esp = stack_top;
    new_task->ebp = stack_top;
    new_task->eip = (uint32_t)entry;

    task_count++;

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


task_t* task_get_head()
{
    return task_list;
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

    print("PID   STATE   EIP\n");

    while (t)
    {
        print_dec(t->pid);
        print("    ");
        print(task_state_string(t->state));
        print("    ");
        print_hex(t->eip);
        print("\n");

        t = t->next;
    }
}

#include "scheduler.h"

void task_yield()
{
    task_t* prev = task_get_current();
    task_t* next = scheduler_next();

    if (!next || next == prev)
        return;

    prev->state = TASK_READY;
    next->state = TASK_RUNNING;

    current_task = next;

    switch_task(&prev->esp, next->esp);
}


#include "pit.h"

void demo_task()
{
    while (1)
    {
        print("Bee task running\n");
        pit_sleep(500);
    }
}



void task_kill(uint32_t pid)
{
    task_t* prev = 0;
    task_t* t = task_get_head();

    while (t)
    {
        if (t->pid == pid)
        {
            if (prev)
                prev->next = t->next;

            print("Task killed: ");
            print_dec(pid);
            print("\n");

            t->state = TASK_BLOCKED;

            return;
        }

        prev = t;
        t = t->next;
    }

    print("PID not found\n");
}
