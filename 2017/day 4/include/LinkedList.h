#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

typedef struct node_t node_t;
struct node_t
{
    char word[32];
    struct node_t *next;
};

typedef struct list_t list_t;
struct list_t
{
    struct node_t *head;
    struct node_t *tail;
    int list_length;
};

// init
void List_Init(list_t *me);

// methods
void List_PushNode(list_t *me, char word[16]);

// constructors
list_t* List_Create();
void List_Destroy(list_t *me);

#endif // _LINKED_LIST_H_