#ifndef _ELEMENTS_H_
#define _ELEMENTS_H_

#include <stdint.h>

// node
typedef struct element_t element_t;
struct element_t
{
    char compare[6];
    char replace[16];
    struct element_t *next;
};

// list
typedef struct replacement_t replacement_t;
struct replacement_t
{
    element_t *head;
    element_t *tail;
    uint16_t length;
};

// init
void Replacement_Init(replacement_t *me);

// push
void Replacement_Push(replacement_t *me, char *compare, char *replace);

// constructors
replacement_t* Replacement_Create();
void Replacement_Destroy(replacement_t *me);

#endif // _ELEMENTS_H_