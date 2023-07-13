#include "../include/LinkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// init
void List_Init(list_t *me)
{
    me->list_length = 0;
    me->head = NULL;
    me->tail = NULL;
}

// methods
void List_PushNode(list_t *me, char word[16])
{
    // create node
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    memcpy(new_node->word, word, 16);
    new_node->next = NULL;

    // insert node
    if(!me->list_length)
    {
        me->head = new_node;
        me->tail = new_node;
    }
    else
    {
        me->tail->next = new_node;
        me->tail = new_node;
    }
    me->list_length++;
}

// constructors
list_t* List_Create()
{
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
    List_Init(new_list);
    return new_list;
}

void List_Destroy(list_t *me)
{
    int destroy_counter = 0;
    node_t *current = me->head;
    for(; current!=NULL;)
    {
        destroy_counter++;
        node_t *temp = current->next;
        free(current);
        current=temp;
    }
    printf("Destroyed %d nodes\n", destroy_counter);
    free(me);
}