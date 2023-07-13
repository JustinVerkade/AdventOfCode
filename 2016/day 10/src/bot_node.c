#include "../include/bot_queue.h"

#include <stdlib.h>
#include <stdio.h>

// init
void BotNode_Init(bot_node_t *me, char *cmd)
{
    me->bot = Bot_Create(cmd);
    me->next = NULL;
}

// constructors
bot_node_t* BotNode_Create(char *cmd)
{
    bot_node_t *node = (bot_node_t*)malloc(sizeof(bot_node_t));
    if(node == NULL) return NULL;
    BotNode_Init(node, cmd);
    return node;
}

void BotNode_Destroy(bot_node_t *me)
{
    Bot_Destroy(me->bot);
    free(me);
}