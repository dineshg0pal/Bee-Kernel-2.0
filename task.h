#ifndef TASK_H
#define TASK_H

#include <stdint.h>

/* ---------------- Task States ---------------- */

#define TASK_RUNNING 0
#define TASK_READY   1
#define TASK_BLOCKED 2

/* ---------------- Task Structure ---------------- */

typedef struct task
{
    uint32_t pid;

    uint32_t esp;
    uint32_t ebp;
    uint32_t eip;

    uint32_t state;

    struct task* next;

} task_t;

/* ---------------- API ---------------- */

/* task system */
void task_init();

/* task creation */
task_t* task_create();

/* task list */
void task_add(task_t* task);

/* current task */
task_t* task_get_current();

task_t* task_get_head();

void task_list_print();

void task_yield();

#endif
