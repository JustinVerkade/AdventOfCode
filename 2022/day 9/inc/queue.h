#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct position_t position_t;
struct position_t
{
    int32_t x;
    int32_t y;
    struct position_t *next;
};

typedef struct queue_t queue_t;
struct queue_t
{
    struct position_t *head;
    struct position_t *tail;
    uint16_t length;
};

// init
void Queue_Init(queue_t *me);

// methods
void Queue_Push(queue_t *me, int32_t x, int32_t y);
position_t Queue_Pop(queue_t *me);

// constructors
queue_t* Queue_Create();
void Queue_Destroy(queue_t *me);

#endif // _QUEUE_H_