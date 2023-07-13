#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Building.h"

typedef struct Node_t Node_t;
struct Node_t
{
    Building_t *building;
    struct Node_t *next;
};

typedef struct Queue_t Queue_t;
struct Queue_t
{
    struct Node_t *head;
    struct Node_t *tail;
    uint32_t size;
};

// queue init
void Queue_Init(Queue_t *me);

// push
void Queue_Push(Queue_t *me, Building_t *building);

// pop
Building_t *Queue_Pop(Queue_t *me);

// constructors
Queue_t *Queue_Create();
void Queue_Destroy(Queue_t *me);

#endif // _QUEUE_H_
