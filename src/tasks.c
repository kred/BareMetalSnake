#include "tasks.h"

TASK tasks[MAX_TASKS];

void tasks_init()
{
    for(unsigned int i = 0; i < MAX_TASKS; ++i)
    {
        tasks[i].fun = 0;
        tasks[i].current_time = 0;
        tasks[i].trigger_time = 0;
    }
}

void tasks_set(unsigned int taskId, void(*fun)(void), unsigned int trigger_time)
{
    tasks[taskId].trigger_time = trigger_time;
    tasks[taskId].current_time = 0;
    tasks[taskId].fun = fun;
}