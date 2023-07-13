#include "../include/node.h"

#include <stdlib.h>

// general methods
void Node_Init(node_t *me, char *msg)
{
    memmove(me->msg, msg, MSG_LENGTH+1);
    me->next = NULL;
}

// constructors
node_t* Node_Create(char *msg)
{
    node_t *node = (node_t*)malloc(sizeof(node_t));

    if(node == NULL)
        return NULL;

    Node_Init(node, msg);
    return node;
}

void Node_Destroy(node_t *me)
{
    free(me);
}