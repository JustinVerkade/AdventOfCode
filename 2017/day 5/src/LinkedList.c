#include "../include/LinkedList.h"

#include <stdlib.h>
#include <stdio.h>

// init
void Memory_Init(memory_t *me)
{
    me->length = 0;
    me->head = NULL;
    me->tail = NULL;
}

// push
void Memory_Push(memory_t *me, int16_t instruction)
{
    // create instruction
    instruction_t *new_instruction = (instruction_t*)malloc(sizeof(instruction_t));
    new_instruction->instruction = instruction;
    new_instruction->next = NULL;
    new_instruction->prev = NULL;

    // install instruction
    if(!me->length)
    {
        me->head = new_instruction;
        me->tail = new_instruction;
    }
    else
    {
        me->tail->next = new_instruction;
        new_instruction->prev = me->tail;
        me->tail = new_instruction;
    }
    me->length++;
}

// constructors
memory_t* Memory_Create()
{
    memory_t *new_memory = (memory_t*)malloc(sizeof(memory_t));
    Memory_Init(new_memory);
    return new_memory;
}

void Memory_Destroy(memory_t *me)
{
    uint32_t destructor_counter = 0;
    for(instruction_t *current=me->head; current!=NULL;)
    {
        destructor_counter++;
        instruction_t *temp = current->next;
        free(current);
        current = temp;
    }
    printf("Destroyed %d instructions\n", destructor_counter);
    free(me);
}