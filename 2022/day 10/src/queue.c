#include "../inc/queue.h"

// init
void Queue_Init(queue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void Queue_Push(queue_t *me, int32_t v, int32_t tick)
{
    // allocate memory
    command_t *new_command = (command_t*)malloc(sizeof(command_t));
    new_command->next= NULL;
    new_command->tick = tick;
    new_command->V = v;

    // insert data
    if(!me->length)
    {
        me->head = new_command;
        me->tail = new_command;
    }
    else
    {
        me->tail->next = new_command;
        me->tail = new_command;
    }
    me->length++;
}

void Queue_Execute(queue_t *me, uint32_t tick, int32_t *reg)
{
    if(!me->length)
        return;

    command_t *prev_current = NULL;
    for(command_t *current=me->head; current!=NULL; current=current->next)
    {
        if(current->tick == tick)
        {
            *reg += current->V;
            if(prev_current)
            {
                if(current->next)
                    prev_current->next = current->next;
                else
                {
                    prev_current->next = NULL;
                    me->tail = prev_current;
                }
                me->length--;
                free(current);
            }
            else
            {
                if(current->next)
                    me->head = current->next;
                me->length--;
                free(current);
            }
        }
        prev_current = current;
    }
}

// constructors
queue_t* Queue_Create()
{
    queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
    Queue_Init(new_queue);
    return new_queue;
}

void Queue_Destroy(queue_t *me)
{
    uint32_t destructor = 0;
    for(command_t *current=me->head; current!=NULL;)
    {
        destructor++;
        command_t *temp = current->next;
        free(current);
        current = temp;
    }
    free(me);
    printf("Destroyed %lu elements\n", destructor);
}
