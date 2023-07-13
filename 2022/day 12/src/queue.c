#include "../inc/queue.h"

// init
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void Queue_Push(queue_t *me, uint32_t x_pos, uint32_t y_pos, char character, uint32_t value)
{
    // check if exists
    for(node_t *current=me->head; current!=NULL; current=current->next)
        if(current->x == x_pos && current->y == y_pos)
            return; // already exists

    // allocate memory
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->next = NULL;
    new_node->x = x_pos;
    new_node->y = y_pos;
    new_node->character = character;
    new_node->value = value;

    // insert node
    if(!me->length)
    {
        me->head = new_node;
        me->tail = new_node;
    }
    else
    {
        me->tail->next = new_node;
        me->tail = new_node;
    }
    me->length++;
}

uint32_t Queue_Pop(queue_t *me, uint32_t *x_pos, uint32_t *y_pos, char *character)
{
    // check if filled
    if(!me->length)
        return; // empty

    // fetch node and replace
    node_t *node = me->head;
    me->head = me->head->next;
    *x_pos = node->x;
    *y_pos = node->y;
    *character = node->character;
    uint32_t value = node->value;

    // free memory
    me->length--;
    free(node);

    return value;
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
    for(node_t *current=me->head; current!=NULL;)
    {
        destroyed++;
        node_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(me);
    printf("Destroyed %lu elements\n", destroyed);
}