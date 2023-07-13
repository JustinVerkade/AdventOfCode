#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../inc/queue.h"

// ##################################################
// data structures



// ##################################################
// processing

void CRT_Display(char *data)
{
    printf("CRT Display\n");
    for(uint16_t ptr=1; ptr<=240; ptr++)
    {
        printf("%c%c", data[ptr-1], data[ptr-1]);

        if(ptr % 40 == 0 && ptr)
            printf("\n");
    }
}

// ##################################################
// part 1

void part1(char *file_name)
{
    // queue
    queue_t *queue = Queue_Create();

    // fetch data
    FILE *file = fopen(file_name, "r");
    char buffer[16];
    uint32_t current_tick = 0;
    while(fgets(buffer, 16, file))
    {
        if(buffer[0] == 'a')
        {
            int32_t value = atoi(buffer + 5);
            Queue_Push(queue, value, current_tick+2);
            current_tick++;
        }
        current_tick++;
    }
    printf("Test\n");

    // execute
    uint32_t reg_tick_counter = 0;
    int32_t reg = 1;
    uint32_t tick = 1;
    uint16_t tick_checks[] = {20, 60, 100, 140, 180, 220};
    uint16_t tick_check_count = 6;
    while(queue->length)
    {
        for(uint8_t index=0; index<tick_check_count; index++)
            if(tick_checks[index] == tick)
            {
                printf("Tick: %lu, Register: %lu\n", tick , reg);
                reg_tick_counter += tick * reg;
            }

        Queue_Execute(queue, tick, &reg);
        tick++;
    }
    printf("Awnser: %ld\n", reg_tick_counter);
}

// ##################################################
// part 2

void part2(char *file_name)
{
    // queue
    queue_t *queue = Queue_Create();

    // fetch data
    FILE *file = fopen(file_name, "r");
    char buffer[16];
    uint32_t current_tick = 0;
    while(fgets(buffer, 16, file))
    {
        if(buffer[0] == 'a')
        {
            int32_t value = atoi(buffer + 5);
            Queue_Push(queue, value, current_tick+2);
            current_tick++;
        }
        current_tick++;
    }
    printf("Test\n");

    char data[240];
    memset(data, ' ', 240);//219
    CRT_Display(data);

    // execute
    int32_t reg = 1;
    uint32_t tick = 1;
    for(uint8_t tick=1; tick<=240; tick++)
    {
        // calculate sprite position
        uint8_t sprite_x = reg;
        uint8_t crt_x = (tick-1) % 40;

        if(sprite_x-1 <= crt_x && sprite_x+1 >= crt_x)
            data[tick-1] = 219;

        Queue_Execute(queue, tick, &reg);
    }
    CRT_Display(data);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argv[1]);
    part2(argv[1]);

    return 0;
}