#include "../inc/Queue.h"

#include "stdlib.h"

// queue init
void Queue_Init(Queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->size = 0;
}

// push
void Queue_Push(Queue_t *me, Building_t *building)
{
    // create hash node
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    new_node->building = building;
    new_node->next = NULL;

    // push node
    if(me->size)
    {
        me->tail->next = new_node;
        me->tail = new_node;
    }
    else
    {
        me->head = new_node;
        me->tail = new_node;
    }

    // increment counter
    me->size++;
}

// pop
Building_t *Queue_Pop(Queue_t *me)
{
    // get pop item
    Building_t *building = me->head->building;

    // swap indices
    Node_t *temp = me->head;
    me->head = me->head->next;

    // free memory
    free(temp);

    // decrement size
    me->size--;

    return building;
}

// constructors
Queue_t *Queue_Create()
{
    Queue_t *new_queue = (Queue_t*)malloc(sizeof(Queue_t));
    if(new_queue) Queue_Init(new_queue);
    return new_queue;
}

void Queue_Destroy(Queue_t *me)
{
    // destroy nodes
    for(Node_t *node=me->head; node!=NULL;)
    {
        Node_t *next = node->next;
        Building_Destroy(node->building);
        free(node);
        node = next;
    }
    free(me);
}