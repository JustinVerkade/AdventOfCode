#include "../inc/RobotQueue.h"

// init
void RobotQueue_Init(RobotQueue_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// methods
void RobotQueue_Push(RobotQueue_t *me, robots_t *robots)
{
    // // check if already exists
    // for(RobotQueueNode_t *node=me->head; node!=NULL; node=node->next)
    //     if(!memcmp(robots, node->robots, sizeof(robots_t)))
    //         return;

    // allocate memory
    RobotQueueNode_t *new_node = (RobotQueueNode_t*)malloc(sizeof(RobotQueueNode_t));
    new_node->robots = robots;
    new_node->next = NULL;

    // insert data
    if(!me->length)
    {
        me->head = new_node;
        me->tail = new_node;
    }
    else
    {
        me->tail->next = new_node;
        me->tail = new_node;
    }
    me->length++;
}

robots_t* RobotQueue_Pop(RobotQueue_t *me)
{
    // check length
    if(!me->length)
        return NULL;

    // pop node
    RobotQueueNode_t *node = me->head;
    robots_t *robots = node->robots;
    me->head = me->head->next;
    free(node);

    // return robots
    me->length--;
    return robots;
}

// constructors
RobotQueue_t* RobotQueue_Create()
{
    RobotQueue_t *new_queue = (RobotQueue_t*)malloc(sizeof(RobotQueue_t));
    RobotQueue_Init(new_queue);
    return new_queue;
}

void RobotQueue_Destroy(RobotQueue_t *me)
{
    for(RobotQueueNode_t *current=me->head; current!=NULL;)
    {
        RobotQueueNode_t *temp = current->next;
        Robots_Destroy(current->robots);
        free(current);
        current = temp;
    }
    free(me);
}