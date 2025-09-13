#ifndef _CMD_QUEUE_H_
#define _CMD_QUEUE_H_

#include "cmd_node.h"

typedef struct cmd_queue_t cmd_queue_t;
struct cmd_queue_t
{
    struct cmd_node_t *head;
    struct cmd_node_t *tail;
    unsigned int length;
};

// init
void CmdQueue_Init(cmd_queue_t *me);

// methods
void CmdQueue_Push(cmd_queue_t *me, char *cmd);
void CmdQueue_Delete(cmd_queue_t *me, unsigned int id);

// constructors
cmd_queue_t* CmdQueue_Create();
void CmdQueue_Destroy(cmd_queue_t *me);

#endif // _CMD_QUEUE_H_