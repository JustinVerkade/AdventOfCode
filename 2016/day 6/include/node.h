#ifndef _NODE_H_
#define _NODE_H_

#define MSG_LENGTH 8

typedef struct node_t node_t;
struct node_t
{
    char msg[7];
    struct node_t *next;
};

// general methods
void Node_Init(node_t *me, char *msg);

// constructors
node_t* Node_Create(char *msg);
void Node_Destroy(node_t *me);


#endif // _NODE_H_