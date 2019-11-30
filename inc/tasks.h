#ifndef _TASKS_H
#define _TASKS_H

#define MAX_TASKS 2

typedef struct 
{
    void(*fun)(void); // pointer to the task function
    unsigned int trigger_time; // period [in ms] when task is executed
    unsigned int current_time; // internal counter, set to 0 on init
} TASK;

extern TASK tasks[MAX_TASKS];

void tasks_init();
void tasks_set(unsigned int taskId, void(*fun)(void), unsigned int trigger_time);

#endif