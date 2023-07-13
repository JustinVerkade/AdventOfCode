#ifndef _LIST_H_
#define _LIST_H_

typedef struct node_t node_t;
struct node_t
{
    int position_x;
    int position_y;
    struct node_t *next;
};

typedef struct list_t list_t;
struct list_t
{
    struct node_t *head;
    struct node_t *last;
    unsigned int length;
};

list_t* List_Create();

void List_Push(list_t *me, int pos_x, int pos_y);
int List_InList(list_t *me, int pos_x, int pos_y);
unsigned int List_GetLength(list_t *me);

void List_Destroy(list_t *me);

#endif // _LIST_H_