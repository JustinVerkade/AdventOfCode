#include "../inc/item_queue.h"

// init
void ItemQueue_Init(item_queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void ItemQueue_PushValue(item_queue_t *me, uint32_t value)
{
    // allocate memory
    item_t *new_item = (item_t*)malloc(sizeof(item_t));
    new_item->value = value;
    new_item->next = NULL;

    // insert
    if(!me->length)
    {
        me->head = new_item;
        me->tail = new_item;
    }
    else
    {
        me->tail->next = new_item;
        me->tail = new_item;
    }
    me->length++;
}

void ItemQueue_Push(item_queue_t *me, item_t *item)
{
    // insert
    if(!me->length)
    {
        me->head = item;
        me->tail = item;
    }
    else
    {
        me->tail->next = item;
        me->tail = item;
    }
    me->length++;
}

item_t* ItemQueue_Pop(item_queue_t *me)
{
    // check length
    if(!me->length)
        return NULL;

    // pop item
    me->length--;
    item_t *item = me->head;
    me->head = me->head->next;
    item->next = NULL;
    return item;
}

// constructors
item_queue_t* ItemQueue_Create()
{
    item_queue_t *new_item_queue = (item_queue_t*)malloc(sizeof(item_queue_t));
    ItemQueue_Init(new_item_queue);
    return new_item_queue;
}

void ItemQueue_Destroy(item_queue_t *me)
{
    uint32_t destroyed = 0;
    for(item_t *current=me->head; current!=NULL;)
    {
        destroyed++;
        item_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %lu items\n", destroyed);
    free(me);
}