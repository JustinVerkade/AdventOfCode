#include "../include/memory.h"

#include <stdlib.h>
#include <stdio.h>

// init
void Memory_Init(memory_t *me)
{
    me->head = NULL;
    me->tail = NULL;
    me->length = 0;
}

// push
void Memory_Push(memory_t *me, uint8_t data[MEMORY_SIZE])
{
    // allocate memory
    adres_t *new_adres = (adres_t*)malloc(sizeof(adres_t));
    memcpy(new_adres, data, MEMORY_SIZE);
    new_adres->next = NULL;

    if(!me->length)
    {
        me->head = new_adres;
        me->tail = new_adres;
    }
    else
    {
        me->tail->next = new_adres;
        me->tail = new_adres;
    }
    me->length++;
}

// constructors
memory_t* Memory_Create()
{
    // create
    memory_t *new_memory = (memory_t*)malloc(sizeof(memory_t));
    Memory_Init(new_memory);
    return new_memory;
}

void Memory_Destroy(memory_t *me)
{
    uint32_t destructor_counter = 0;
    for(adres_t *current=me->head; current!=NULL;)
    {
        destructor_counter++;
        adres_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %d addresses\n", destructor_counter);
    free(me);
}
