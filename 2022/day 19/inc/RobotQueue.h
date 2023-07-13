#ifndef _ROBOT_QUEUE_H_
#define _ROBOT_QUEUE_H_

#include "robot.h"

typedef struct RobotQueueNode_t RobotQueueNode_t;
struct RobotQueueNode_t
{
    robots_t *robots;
    struct RobotQueueNode_t *next;
};

typedef struct RobotQueue_t RobotQueue_t;
struct RobotQueue_t
{
    struct RobotQueueNode_t *head;
    struct RobotQueueNode_t *tail;
    uint32_t length;
};

// init
void RobotQueue_Init(RobotQueue_t *me);

// methods
void RobotQueue_Push(RobotQueue_t *me, robots_t *robots);
robots_t* RobotQueue_Pop(RobotQueue_t *me);

// constructors
RobotQueue_t* RobotQueue_Create();
void RobotQueue_Destroy(RobotQueue_t *me);

#endif // _ROBOT_QUEUE_H_