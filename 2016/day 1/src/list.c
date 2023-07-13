#include "../include/list.h"

#include <stdio.h>
#include <stdlib.h>

list_t* List_Create()
{
    list_t *new = (list_t*)malloc(sizeof(list_t));
    new->head = NULL;
    new->last = NULL;
    new->length = 0;
    return new;
}

void List_Push(list_t *me, int pos_x, int pos_y)
{
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    new_node->position_x = pos_x;
    new_node->position_y = pos_y;
    new_node->next = NULL;

    if(!me->length)
    {
        me->head = new_node;
        me->last = new_node;
    }
    else
    {
        me->last->next = new_node;
        me->last = new_node;
    }

    me->length++;
}

int List_InList(list_t *me, int pos_x, int pos_y)
{
    int counter = 0;
    for(node_t *node=me->head; node!=NULL; node=node->next)
    {
        int node_pos_x = node->position_x;
        int node_pos_y = node->position_y;

        if(node_pos_x == pos_x && node_pos_y == pos_y)
            return 1;
    }
    return 0;
}

unsigned int List_GetLength(list_t *me)
{
    return me->length;
}

void List_Destroy(list_t *me)
{
    int destroyed = 0;
    node_t *curr = me->head;
    do
    {
        destroyed++;
        node_t *next = curr->next;
        free(curr);
        curr = next;
    } while (curr != NULL);
    printf("%d\n", destroyed);

    free(me);
}
