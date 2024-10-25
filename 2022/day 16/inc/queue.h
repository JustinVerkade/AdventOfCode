#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "state.h"

typedef struct node_t node_t;
struct node_t
{
    state_t *state;
    struct node_t *next;
};

typedef struct queue_t queue_t;
struct queue_t
{
    node_t *head;
    node_t *tail;
    uint32_t length;
};

// init
void Queue_Init(queue_t *me);

// methods
void Queue_Push(queue_t *me, state_t *state);
state_t* Queue_Pop(queue_t *me);

// constructor
queue_t* Queue_Create();

// destructor
void Queue_Destroy(queue_t *me);

#endif // _QUEUE_H_