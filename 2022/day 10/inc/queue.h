#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct command_t command_t;
struct command_t
{
    int32_t V;
    uint32_t tick;
    struct command_t *next;
};

typedef struct queue_t queue_t;
struct queue_t
{
    struct command_t *head;
    struct command_t *tail;
    uint32_t length;
};

// init
void Queue_Init(queue_t *me);

// methods
void Queue_Push(queue_t *me, int32_t v, int32_t tick);
void Queue_Execute(queue_t *me, uint32_t tick, int32_t *reg);

// constructors
queue_t* Queue_Create();
void Queue_Destroy();

#endif // _QUEUE_H_