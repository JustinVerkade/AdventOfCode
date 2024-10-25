#include "../inc/list.h"

void List_Init(list_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

void List_PushValue(list_t *me, int32_t value)
{
    // create item
    item_t *new_item = (item_t*)malloc(sizeof(item_t));
    new_item->index = me->length;
    new_item->value = value;
    new_item->next = NULL;
    new_item->prev = NULL;

    // insert item
    if(!me->length)
    {
        me->head = new_item;
        me->tail = new_item;
    }
    else
    {
        new_item->prev = me->tail;
        me->tail->next = new_item;
        me->tail = new_item;
    }
    me->length++;
}

static item_t* List_Get(list_t *me, int32_t index)
{
    // find xth item
    item_t *current = me->head;
    for(uint32_t ptr=0; ptr<index; ptr++)
        current = current->next;
    return current;
}

void List_Move(list_t *me, int32_t index, int32_t move)
{
    int32_t new_index = index + move;
    new_index = new_index < 0 ? 20 * me->length - 1 + new_index : new_index;
    new_index = new_index % me->length;
    if(move < 0 && new_index == 0)
        new_index = me->length;
    if(index + move > me->length)
        new_index++;
    printf("Index: %ld\n", index);
    printf("Index: %ld\n", new_index);

    // get current item ptr
    item_t *current = List_Get(me, index);

    // copy
    item_t *item = (item_t*)malloc(sizeof(item_t));
    item->index = current->index;
    item->value = current->value;
    item->next = current->next;
    item->prev = current->prev;

    // insert new item by index
    item_t *insert = me->head;
    for(uint32_t ptr=0; ptr<new_index; ptr++)
    {
        insert = insert->next;

        // insert item as tail
        if(insert == NULL)
        {
            me->tail->next = current;
            current->prev = me->tail;
            me->tail = current;
            return;
        }
    }

    // insert item at head
    if(current == me->head) // first item
    {
        me->head->prev = item; // insert item behind current
        item->next = me->head; // insert current after item
        me->head = item;       // update head
    }

    // insert item at tail
    else if(current == me->tail)
    {
        item->prev = me->tail->prev;
        me->tail->prev->next = item;
        me->tail->prev = item; // insert item behind current
        item->next = me->tail; // insert current after item
    }

    // insert item somewhere else
    else
    {
        item->prev = current->prev; // insert current prev as item prev
        current->prev->next = item; // insert item as current prev next
        current->prev = item; // insert item behind current
        item->next = current; // insert current after item
    }

    // remove previous
    if(current->prev)
    {
        current->prev->next = current->next;
    }
    if(current->next)
    {
        current->next->prev = current->prev;
    }

    free(current);
}

void List_Print(list_t *me)
{
    for(item_t *current=me->head; current!=NULL; current=current->next)
    {
        printf("%ld, ", current->value);
    }
    printf("\n");
}

list_t* List_Create()
{
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
    List_Init(new_list);
    return new_list;
}

void List_Destroy(list_t *me)
{
    for(item_t *current=me->head; current!=NULL;)
    {
        item_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(me);
}