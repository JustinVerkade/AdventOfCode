#include "../include/cmd_queue.h"

#include <stdlib.h>
#include <stdio.h>

// init
void CmdQueue_Init(cmd_queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void CmdQueue_Push(cmd_queue_t *me, char *cmd)
{
    if(!me->length)
    {
        cmd_node_t *node = CmdNode_Create(me->length, cmd);
        me->head = node;
        me->tail = node;
        me->length++;
    }
    else
    {
        cmd_node_t *node = CmdNode_Create(me->length, cmd);
        cmd_node_t *prev_tail = me->tail;
        prev_tail->next = node;
        me->tail = node;
        me->length++;
    }
}

void CmdQueue_Delete(cmd_queue_t *me, unsigned int id)
{
    cmd_node_t *prev = NULL;
    cmd_node_t *curr = me->head;
    do
    {
        if(curr->id == id)
        {

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
            CmdNode_Destroy(curr);
            me->length--;
            break;
        }

        prev = curr;
        curr = curr->next;
    } while (curr != NULL);
}

// constructors
cmd_queue_t* CmdQueue_Create()
{
    cmd_queue_t *queue = (cmd_queue_t*)malloc(sizeof(cmd_queue_t));
    if(queue == NULL) return NULL;
    CmdQueue_Init(queue);
    return queue;
}

void CmdQueue_Destroy(cmd_queue_t *me)
{
    int counter = 0;
    if(me->length)
    {
        cmd_node_t *curr = me->head;
        do
        {
            cmd_node_t *next = curr->next;
            CmdNode_Destroy(curr);
            counter++;
            curr = next;
        } while (curr != NULL);
    }
    printf("CmdQueue deleted %d items!\n", counter);
    free(me);
}