#include "../include/bot_queue.h"

#include <stdlib.h>
#include <stdio.h>

// init
void BotQueue_Init(bot_queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void BotQueue_Push(bot_queue_t *me, char *cmd)
{
    if(!me->length)
    {
        bot_node_t *node = BotNode_Create(cmd);
        me->head = node;
        me->tail = node;
        me->length++;
    }
    else
    {
        bot_node_t *node = BotNode_Create(cmd);
        bot_node_t *prev_tail = me->tail;
        prev_tail->next = node;
        me->tail = node;
        me->length++;
    }
}

void BotQueue_PushBot(bot_queue_t *me, bot_t *bot)
{
    if(!me->length)
    {
        bot_node_t *node = (bot_node_t*)malloc(sizeof(bot_node_t));
        node->bot = bot;
        node->next = NULL;

        me->head = node;
        me->tail = node;
        me->length++;
    }
    else
    {
        bot_node_t *node = (bot_node_t*)malloc(sizeof(bot_node_t));
        node->bot = bot;
        node->next = NULL;

        bot_node_t *prev_tail = me->tail;
        prev_tail->next = node;
        me->tail = node;
        me->length++;
    }
}

bot_t* BotQueue_GetById(bot_queue_t const *me, unsigned int id)
{
    if(me->length == 0) return NULL;
    bot_node_t *curr = me->head;
    do
    {
        if(curr->bot->id == id)
            return curr->bot;
        curr = curr->next;
    } while (curr != NULL);
    return NULL;
}

bot_t* BotQueue_PopById(bot_queue_t *me, unsigned int id)
{
    if(me->length == 0) return NULL;
    bot_node_t *prev = NULL;
    bot_node_t *curr = me->head;
    do
    {
        if(curr->bot->id == id)
        {
            bot_t *bot = curr->bot;

            // stich links
            if(prev != NULL) prev->next = curr->next;
            if(curr == me->head) me->head = curr->next;
            if(curr == me->tail)
            {
                me->tail = prev;
                if(prev != NULL)
                    prev->next = NULL;
            }

            // free node without bot;
            free(curr);
            
            me->length--;

            return bot;
        }

        prev = curr;
        curr = curr->next;
    } while (curr != NULL);
    return NULL;
}

void BotQueue_Print(bot_queue_t const *me)
{
    bot_node_t *curr = me->head;
    do
    {
        Bot_Print(curr->bot);
        curr = curr->next;
    } while (curr != NULL);
    
}

// constructors
bot_queue_t* BotQueue_Create()
{
    bot_queue_t *queue = (bot_queue_t*)malloc(sizeof(bot_queue_t));
    if(queue == NULL) return NULL;
    BotQueue_Init(queue);
    return queue;
}

void BotQueue_Destroy(bot_queue_t *me)
{
    int counter = 0;
    if(me->length)
    {
        bot_node_t *curr = me->head;
        do
        {
            bot_node_t *next = curr->next;
            BotNode_Destroy(curr);
            counter++;
            curr = next;
        } while (curr != NULL);
    }
    printf("BotQueue deleted %d items!\n", counter);
    free(me);
}