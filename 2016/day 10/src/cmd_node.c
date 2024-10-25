#include "../include/cmd_node.h"

#include <stdlib.h>
#include <stdio.h>

// init
void CmdNode_Init(cmd_node_t *me, unsigned int id, char *cmd)
{
    me->id = id;
    me->next = NULL;

    char *c1, *c2, *ptr;
    char number[6];

    // fetch executing target
    c1 = cmd + 4;
    c2 = c1;
    for(; *c2!=' '; c2++)
    memset(number, 0, 6);
    memcpy(number, c1, c2 - c1);
    me->from = atoi(number);

    // fetch target 1 type
    ptr = c2 + 14;
    if(!strncmp(ptr, "bot", 3))
    {
        me->target_id_1 = BOT;
        ptr += 4;
    }
    else
    {
        me->target_id_1 = OUTPUT;
        ptr += 7;
    }

    // fetch target 1 id
    c1 = ptr;
    c2 = c1;
    for(; *c2!=' '; c2++)
    memset(number, 0, 6);
    memcpy(number, c1, c2 - c1);
    me->target_1 = atoi(number);

    // fetch target 2 type
    ptr = c2 + 13;
    if(!strncmp(ptr, "bot", 3))
    {
        me->target_id_2 = BOT;
        ptr += 4;
    }
    else
    {
        me->target_id_2 = OUTPUT;
        ptr += 7;
    }

    // fetch target 2 id
    c1 = ptr;
    c2 = c1;
    for(; *c2!='\0'; c2++)
    memset(number, 0, 6);
    memcpy(number, c1, c2 - c1);
    me->target_2 = atoi(number);
}

// constructors
cmd_node_t* CmdNode_Create(unsigned int id, char *cmd)
{
    cmd_node_t *node = (cmd_node_t*)malloc(sizeof(cmd_node_t));
    if(node == NULL) return NULL;
    CmdNode_Init(node, id, cmd);
    return node;
}

void CmdNode_Destroy(cmd_node_t *me)
{
    free(me);
}