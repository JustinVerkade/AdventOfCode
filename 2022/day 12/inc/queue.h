#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct node_t node_t;
struct node_t
{
    uint32_t x;
    uint32_t y;
    char character;
    uint32_t value;
    struct node_t *next;
};

typedef struct queue_t queue_t;
struct queue_t
{
    struct node_t *head;
    struct node_t *tail;
    uint32_t length;
};

// init
void Queue_Init(queue_t *me);

// methods
void Queue_Push(queue_t *me, uint32_t x_pos, uint32_t y_pos, char character, uint32_t value);
uint32_t Queue_Pop(queue_t *me, uint32_t *x_pos, uint32_t *y_pos, char *character);

// constructors
queue_t* Queue_Create();
void Queue_Destroy(queue_t *me);

#endif // _QUEUE_H_