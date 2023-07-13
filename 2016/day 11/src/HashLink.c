#include "../inc/HashLink.h"

#include "stdio.h"
#include "string.h"

// init
void HashLink_Init(HashLink_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->list_length = 0;
}

// push
void HashLink_Push(HashLink_t *me, Building_t *building)
{
    // create hash node
    HashLinkNode_t *new_node = (HashLinkNode_t*)malloc(sizeof(HashLinkNode_t));
    new_node->building = building;
    new_node->next = NULL;

    // push node
    if(me->list_length)
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
    me->list_length++;
}

// building exists
uint8_t HashLink_Exists(HashLink_t *me, Building_t *building)
{
    for(HashLinkNode_t *node=me->head; node!=NULL; node=node->next)
        if(memcmp(building, node->building, sizeof(Building_t)))
            return 1;
    return 0;
}

// constructors
void HashLink_Destroy(HashLink_t *me)
{
    // destroy nodes
    for(HashLinkNode_t *node=me->head; node!=NULL;)
    {
        HashLinkNode_t *next = node->next;
        Building_Destroy(node->building);
        free(node);
        node = next;
    }
}