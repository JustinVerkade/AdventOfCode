#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/LinkedList.h"

instruction_t* Memory_jump(instruction_t *current)
{
    int16_t jump = current->instruction++;
    if(jump >= 0)
    {
        for(uint16_t i=0; i<jump; i++)
        {
            if(current->next == NULL)
                return NULL;
            current = current->next;
        }
    }
    else
    {
        for(uint16_t i=0; i<-jump; i++)
        {
            if(current->prev == NULL)
                return NULL;
            current = current->prev;
        }
    }
    return current;
}

void day1()
{
    // create memory
    memory_t *memory = Memory_Create();

    // fetch data
    while(1)
    {
        char rx_buffer[5];
        memset(rx_buffer, 0x00, 5);
        scanf("%s", rx_buffer);
        if(*rx_buffer == 's')
            break;

        int16_t value = atoi(rx_buffer);
        Memory_Push(memory, value);
    };
    printf("Loaded %d instructions\n", memory->length);

    int32_t stack_ptr = 0;
    instruction_t *current = memory->head;
    while(1)
    {
        stack_ptr++;
        current = Memory_jump(current);
        if(current == NULL)
            break;
    }

    printf("Awmser: %ld jumps\n", stack_ptr);

    Memory_Destroy(memory);
    printf("Exit!");
}

instruction_t* Memory_jump2(instruction_t *current)
{
    int16_t jump = current->instruction;
    if(current->instruction >= 3)
        current->instruction--;
    else
        current->instruction++;
    if(jump >= 0)
    {
        for(uint16_t i=0; i<jump; i++)
        {
            if(current->next == NULL)
                return NULL;
            current = current->next;
        }
    }
    else
    {
        for(uint16_t i=0; i<-jump; i++)
        {
            if(current->prev == NULL)
                return NULL;
            current = current->prev;
        }
    }
    return current;
}

void day2()
{
    // create memory
    memory_t *memory = Memory_Create();

    // fetch data
    while(1)
    {
        char rx_buffer[5];
        memset(rx_buffer, 0x00, 5);
        scanf("%s", rx_buffer);
        if(*rx_buffer == 's')
            break;

        int16_t value = atoi(rx_buffer);
        Memory_Push(memory, value);
    };
    printf("Loaded %d instructions\n", memory->length);

    int32_t stack_ptr = 0;
    instruction_t *current = memory->head;
    while(1)
    {
        stack_ptr++;
        current = Memory_jump2(current);
        if(current == NULL)
            break;
    }

    printf("Awmser: %ld jumps\n", stack_ptr);

    Memory_Destroy(memory);
    printf("Exit!");
}

int main()
{
    //day1();
    day2();

    return 0;
}