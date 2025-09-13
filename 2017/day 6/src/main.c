#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/memory.h"

/*
b1 = 0 + 2 = 2 add
b2 = 2 + 2 = 4 add
b3 = 7 / b-1 = 1 spare
b4 = 0 + 1 = 2 add
*/

uint32_t checkMemory(memory_t *me, uint8_t data[MEMORY_SIZE])
{
    uint32_t counter = 0;
    for(adres_t *current=me->head; current!=NULL; current=current->next)
    {
        uint8_t flag = 0;
        for(uint8_t i=0; i<MEMORY_SIZE; i++)
        {
            if(current->data[i] != data[i])
            {
                flag = 1;
                break;
            }
        }
        if(flag == 0)
        {
            return me->length - counter;
        }
        counter++;
    }
    return 0;
}

void printData(uint8_t data[MEMORY_SIZE])
{
    printf("Data = {");
    for(uint8_t i=0; i<MEMORY_SIZE-1; i++)
        printf("%d, ", data[i]);
    printf("%d};\n", data[MEMORY_SIZE-1]);
}

void day1()
{
    // create memory
    memory_t *memory = Memory_Create();

    uint8_t data[MEMORY_SIZE] = {4, 10, 4, 1, 8, 4, 9, 14, 5, 1, 14, 15, 0, 15, 3, 5};
    //uint8_t data[MEMORY_SIZE] = {0, 2, 7, 0};

    printData(data);

    while(1)
    {
        // find largest value
        uint8_t largest_value = 0;
        uint8_t largest_index = 0;
        for(uint8_t i=0; i<MEMORY_SIZE; i++)
        {
            if(data[i] > largest_value)
            {
                largest_value = data[i];
                largest_index = i;
            }
        }

        // apply addition
        data[largest_index] = 0;
        for(uint8_t i=0; i<largest_value; i++)
        {
            uint8_t index = (largest_index + 1 + i) % MEMORY_SIZE;
            data[index] += 1;
        }

        if(checkMemory(memory, data))
            break;
        Memory_Push(memory, data);
    } 

    printf("Awnser: %d\n", memory->length + 1);
    
    Memory_Destroy(memory);
    printf("Exit\n");
}

void day2()
{
    // create memory
    memory_t *memory = Memory_Create();

    uint8_t data[MEMORY_SIZE] = {4, 10, 4, 1, 8, 4, 9, 14, 5, 1, 14, 15, 0, 15, 3, 5};
    //uint8_t data[MEMORY_SIZE] = {0, 2, 7, 0};

    printData(data);

    uint32_t counter = 0;
    while(1)
    {
        // find largest value
        uint8_t largest_value = 0;
        uint8_t largest_index = 0;
        for(uint8_t i=0; i<MEMORY_SIZE; i++)
        {
            if(data[i] > largest_value)
            {
                largest_value = data[i];
                largest_index = i;
            }
        }
        
        // apply addition
        data[largest_index] = 0;
        for(uint8_t i=0; i<largest_value; i++)
        {
            uint8_t index = (largest_index + 1 + i) % MEMORY_SIZE;
            data[index] += 1;
        }

        if(counter = checkMemory(memory, data))
            break;
        Memory_Push(memory, data);
    } 

    printf("Awnser: %d\n", counter);
    
    Memory_Destroy(memory);
    printf("Exit\n");
}

int main()
{
    //day1();
    day2();

    return 0;
}