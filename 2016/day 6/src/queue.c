#include "../include/queue.h"
#include <stdlib.h>
#include <stdio.h>

// general methods
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

void Queue_Push(queue_t *me, char *msg)
{
    if(me->length)
    {
        node_t* prev_tail = me->tail;
        node_t* next_tail = Node_Create(msg);

        prev_tail->next = next_tail;
        me->tail = next_tail;        
    }
    else
    {
        node_t* node = Node_Create(msg);
        me->head = node;
        me->tail = node;
    }
    me->length++;
}

short Queue_GetLength(queue_t *me)
{
    return me->length;
}

void Queue_Print(queue_t *me)
{
    for(node_t *curr=me->head; curr!=NULL; curr=curr->next)
    {
        printf("Node: str:%s\n", curr->msg);
    }
}

// constructors
queue_t* Queue_Create()
{
    queue_t *queue = (queue_t*)malloc(sizeof(queue_t));

    if(queue == NULL)
        return NULL;

    Queue_Init(queue);
    return queue;
}

void Queue_Destroy(queue_t *me)
{
    node_t *curr = me->head;
    do
    {
        node_t *next = curr->next;
        free(curr);
        curr = next;
    } while (curr != NULL);
    free(me);
}