#include "../inc/queue.h"

// init queue
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
int32_t Queue_Pop(queue_t *me)
{
    // check if empty
    if(!me->length)
        return -1;

    // fetch coordinate
    coordinate_t *coordinate = me->head;
    me->head = me->head->next;

    // get hash
    int32_t hash = coordinate->hash;

    // free memory
    free(coordinate);
    me->length--;

    return hash;
}

void Queue_Push(queue_t *me, int32_t hash)
{
    // allocate memory
    coordinate_t *new_coordinate = (coordinate_t*)malloc(sizeof(coordinate_t));
    new_coordinate->hash = hash;
    new_coordinate->next = NULL;

    // insert memory
    if(!me->length)
    {
        me->head = new_coordinate;
        me->tail = new_coordinate;
    }
    else
    {
        me->tail->next = new_coordinate;
        me->tail = new_coordinate;
    }
    me->length++;
}

// constructors
queue_t* Queue_Create()
{
    queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
    Queue_Init(new_queue);
    return new_queue;
}

void Queue_Destroy(queue_t *me)
{
    for(coordinate_t *current=me->head; current!=NULL;)
    {
        coordinate_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(me);
}
