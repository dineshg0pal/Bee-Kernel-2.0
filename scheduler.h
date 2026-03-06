#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

void scheduler_init();
task_t* scheduler_next();

#endif
