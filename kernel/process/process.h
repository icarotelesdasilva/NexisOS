#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>


/*
* NOTE: This is a multitask start file
* The kernel may not have full multitasking, but that's an assumption
* (This does not affect the behavior of the kernel and anything yet).
*/

typedef struct {
    uint32_t pid;
    uintptr_t esp; 
    uint32_t state;

}task_t;
extern task_t *current;
extern task_t *next;

#endif
