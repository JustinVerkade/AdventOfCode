#ifndef _CMD_NODE_H_
#define _CMD_NODE_H_

typedef enum output_type_e output_type_e;
enum output_type_e
{
    OUTPUT,
    BOT,
};

typedef struct cmd_node_t cmd_node_t;
struct cmd_node_t
{
    // id
    unsigned int id;

    // execute
    unsigned int from;

    // first call
    unsigned int target_id_1;
    output_type_e target_1;

    // second call
    unsigned int target_id_2;
    output_type_e target_2;

    // link
    struct cmd_node_t *next;
};

// init
void CmdNode_Init(cmd_node_t *me, unsigned int id, char *cmd);

// constructors
cmd_node_t* CmdNode_Create(unsigned int id, char *cmd);
void CmdNode_Destroy(cmd_node_t *me);

#endif // _CMD_NODE_H_