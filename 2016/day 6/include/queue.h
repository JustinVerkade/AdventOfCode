#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "node.h"

typedef struct queue_t queue_t;
struct queue_t
{
    struct node_t *head;
    struct node_t *tail;
    short length;
};

// general methods
void Queue_Init(queue_t *me);
void Queue_Push(queue_t *me, char *msg);
short Queue_GetLength(queue_t *me);
void Queue_Print(queue_t *me);

// constructors
queue_t* Queue_Create();
void Queue_Destroy(queue_t *me);

#endif // _QUEUE_H_