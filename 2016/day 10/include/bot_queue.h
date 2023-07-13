#ifndef _BOT_QUEUE_H_
#define _BOT_QUEUE_H_

#include "bot_node.h"

typedef struct bot_queue_t bot_queue_t;
struct bot_queue_t
{
    struct bot_node_t *head;
    struct bot_node_t *tail;
    unsigned int length;
};

// init
void BotQueue_Init(bot_queue_t *me);

// methods
void BotQueue_Push(bot_queue_t *me, char *cmd);
void BotQueue_PushBot(bot_queue_t *me, bot_t *bot);

bot_t* BotQueue_GetById(bot_queue_t const *me, unsigned int id);
bot_t* BotQueue_PopById(bot_queue_t *me, unsigned int id);
void BotQueue_Print(bot_queue_t const *me);

// constructors
bot_queue_t* BotQueue_Create();
void BotQueue_Destroy(bot_queue_t *me);

#endif // _BOT_QUEUE_H_