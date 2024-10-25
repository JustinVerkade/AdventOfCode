#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct item_t item_t;
struct item_t
{
    int32_t value;
    int32_t index;
    struct item_t *next;
    struct item_t *prev;
};

typedef struct list_t list_t;
struct list_t
{
    struct item_t *head;
    struct item_t *tail;
    int32_t length;
};

void List_Init(list_t *me);
void List_Move(list_t *me, int32_t index, int32_t move);
item_t* List_Remove(list_t *me, int32_t index);

void List_Print(list_t *me);

list_t* List_Create();
void List_Destroy(list_t *me);


#endif // _LIST_H_