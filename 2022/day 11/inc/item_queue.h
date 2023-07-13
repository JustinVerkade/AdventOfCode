#ifndef _ITEM_QUEUE_H_
#define _ITEM_QUEUE_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VALUE_STRING_SIZE 1024

typedef struct item_t item_t;
struct item_t
{
    //char value[VALUE_STRING_SIZE];
    uint64_t value;
    struct item_t *next;
};

typedef struct item_queue_t item_queue_t;
struct item_queue_t
{
    struct item_t *head;
    struct item_t *tail;
    uint32_t length;
};

// init
void ItemQueue_Init(item_queue_t *me);

// methods
void ItemQueue_PushValue(item_queue_t *me, uint64_t value);
void ItemQueue_Push(item_queue_t *me, item_t *item);
item_t* ItemQueue_Pop(item_queue_t *me);

// constructors
item_queue_t* ItemQueue_Create();
void ItemQueue_Destroy(item_queue_t *me);

#endif // _ITEM_QUEUE_H_