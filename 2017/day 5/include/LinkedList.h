#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <stdint.h>

typedef struct instruction_t instruction_t;
struct instruction_t
{
    int16_t instruction;
    struct instruction_t *next;
    struct instruction_t *prev;
};

typedef struct memory_t memory_t;
struct memory_t
{
    instruction_t *head;
    instruction_t *tail;
    uint32_t length;
};

// init
void Memory_Init(memory_t *me);

// push
void Memory_Push(memory_t *me, int16_t instruction);

// constructors
memory_t* Memory_Create();
void Memory_Destroy(memory_t *me);

#endif // _LINKED_LIST_H_