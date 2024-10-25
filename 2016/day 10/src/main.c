#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/bot_queue.h"
#include "../include/cmd_queue.h"

void part1()
{
    bot_queue_t *empty_queue = BotQueue_Create();
    bot_queue_t *full_queue = BotQueue_Create();
    cmd_queue_t *cmd_queue = CmdQueue_Create();

    // fetch data
    char cmd[512];
    while(1)
    {
        fgets(cmd, 512, stdin);
        if(cmd[0] == '-') break; // done

        // check type of cmd
        if(!strncmp(cmd, "bot", 3)) // give value
        {
            CmdQueue_Push(cmd_queue, cmd);
        }
        else // bot execute
        {
            // process cmd into a bot
            bot_t *new_bot = Bot_Create(cmd);

            // check if bot ID is already in the empty queue
            // not in queue we add it into the queue
            if(BotQueue_GetById(empty_queue, new_bot->id) == NULL)
            {
                printf("new bot\n");
                BotQueue_PushBot(empty_queue, new_bot);
            }

            // in the queue so we add the value to the existing bot
            // and push the new bot into the full queue
            else
            {
                bot_t *full_bot = BotQueue_PopById(empty_queue, new_bot->id);
                Bot_InsertChip(full_bot, new_bot->chip_hi);
                BotQueue_PushBot(full_queue, full_bot);
                Bot_Destroy(new_bot);
            }
            
        }
    }

    // execute commands
    unsigned int output_data[1000];
    memset(output_data, 0, sizeof(output_data));
    while(cmd_queue->length > 0)
    {
        printf("\rCmds left: %d     ", cmd_queue->length);
        for(cmd_node_t *curr=cmd_queue->head; curr!=NULL; curr=curr->next)
        {
            // check if target is full
            bot_t *transmitting_bot = BotQueue_PopById(full_queue, curr->from);
            if(transmitting_bot == NULL) continue; // not in list

            unsigned int hi = transmitting_bot->chip_hi;
            unsigned int lo = transmitting_bot->chip_lo;

            // handle target 1
            if(curr->target_id_1 == OUTPUT) output_data[curr->target_1] = lo;
            else
            {
                if(BotQueue_GetById(empty_queue, curr->target_1) == NULL) // bot does not exist in empty list
                {
                    bot_t *new_bot = Bot_Create("value 0 goes to bot 0");
                    new_bot->id = curr->target_1;
                    new_bot->chip_count = 0;
                    Bot_InsertChip(new_bot, lo);
                    BotQueue_PushBot(empty_queue, new_bot);
                }
                else // bot in empty queue so is full
                {
                    bot_t *full_bot = BotQueue_PopById(empty_queue, curr->target_1);
                    Bot_InsertChip(full_bot, lo);
                    BotQueue_PushBot(full_queue, full_bot);
                }
            }

            // handle target 2
            if(curr->target_id_2 == OUTPUT) output_data[curr->target_2] = hi;
            else
            {
                if(BotQueue_GetById(empty_queue, curr->target_2) == NULL) // bot does not exist in empty list
                {
                    bot_t *new_bot = Bot_Create("value 0 goes to bot 0");
                    new_bot->id = curr->target_2;
                    new_bot->chip_count = 0;
                    Bot_InsertChip(new_bot, hi);
                    BotQueue_PushBot(empty_queue, new_bot);
                }
                else // bot in empty queue so is full
                {
                    bot_t *full_bot = BotQueue_PopById(empty_queue, curr->target_2);
                    Bot_InsertChip(full_bot, hi);
                    BotQueue_PushBot(full_queue, full_bot);
                }
            }

            // free the bot
            Bot_Destroy(transmitting_bot);

            // command has been executed
            CmdQueue_Delete(cmd_queue, curr->id);
            break;
        }
    }

    printf("\nDone\n");

    printf("Result: %d\n", output_data[0] * output_data[1] * output_data[2]);

    BotQueue_Destroy(empty_queue);
    BotQueue_Destroy(full_queue);
    CmdQueue_Destroy(cmd_queue);
}

void part2()
{

}

int main()
{
    part1();
    part2();

    return 0;
}