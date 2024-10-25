#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct coordinate_t coordinate_t;
struct coordinate_t
{
    uint32_t hash;
    struct coordinate_t *next;
};

typedef struct queue_t queue_t;
struct queue_t
{
    struct coordinate_t *head;
    struct coordinate_t *tail;
    int32_t length;
};

// init queue
void Queue_Init(queue_t *me);

// methods
int32_t Queue_Pop(queue_t *me);
void Queue_Push(queue_t *me, int32_t hash);

// constructors
queue_t* Queue_Create();
void Queue_Destroy(queue_t *me);

#endif // _QUEUE_H_