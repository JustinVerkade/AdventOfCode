#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

#define MEMORY_SIZE 16

typedef struct adres_t adres_t;
struct adres_t
{
    uint8_t data[MEMORY_SIZE];
    struct adres_t *next;
};

typedef struct memory_t memory_t;
struct memory_t
{
    adres_t *head;
    adres_t *tail;
    uint32_t length;
};

// init
void Memory_Init(memory_t *me);

// push
void Memory_Push(memory_t *me, uint8_t data[MEMORY_SIZE]);

// constructors
memory_t* Memory_Create();
void Memory_Destroy(memory_t *me);

#endif // _MEMORY_H_