#ifndef _BOT_NODE_H_
#define _BOT_NODE_H_

#include "bot.h"

typedef struct bot_node_t bot_node_t;
struct bot_node_t
{
    bot_t *bot;
    struct bot_node_t *next;
};

// init
void BotNode_Init(bot_node_t *me, char *cmd);

// constructors
bot_node_t* BotNode_Create(char *cmd);
void BotNode_Destroy(bot_node_t *me);

#endif // _BOT_NODE_H_