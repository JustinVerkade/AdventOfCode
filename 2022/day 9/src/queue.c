#include "../inc/queue.h"

// init
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void Queue_Push(queue_t *me, int32_t x, int32_t y)
{
    // check if exists
    if(me->length > 0)
        for(position_t *current=me->head; current!=NULL; current=current->next)
            if(current->x == x && current->y == y)
                return; // already exists

    // create position
    position_t *new_position = (position_t*)malloc(sizeof(position_t));
    new_position->x = x;
    new_position->y = y;
    new_position->next = NULL;

    // push to list
    if(!me->length)
    {
        me->head = new_position;
        me->tail = new_position;
    }
    else
    {
        me->tail->next = new_position;
        me->tail = new_position;
    }
    me->length++;
}

position_t Queue_Pop(queue_t *me)
{
    // fetch and update head
    position_t *position = me->head;
    me->head = me->head->next;

    // fetch data
    int32_t x = position->x;
    int32_t y = position->y;

    // free memory
    free(position);

    // return position
    return (position_t){x, y, NULL};
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
    uint32_t destroyed = 0;
    for(position_t *current=me->head; current!=NULL;)
    {
        destroyed++;
        position_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(me);
    printf("Destroyed %llu elements\n", destroyed);
}