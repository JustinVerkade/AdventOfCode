#ifndef _RELATION_H_
#define _RELATION_H_

#include <stdint.h>

// node
typedef struct relation_t relation_t;
struct relation_t
{
    char name[12];
    int16_t happiness;
    struct relation_t *next;
};

// linked list
typedef struct relations_t relations_t;
struct relations_t
{
    struct relation_t *head;
    struct relation_t *tail;
    uint8_t length;
};

// init
void Relations_Init(relations_t *me);

// methods
void Relations_Push(relations_t *me, char *name, int16_t happiness);
int16_t Relations_GetHappiness(relations_t *me, char *name);

// constructors
relations_t* Relations_Create();
void Relations_Destroy(relations_t *me);

#endif // _RELATION_H_