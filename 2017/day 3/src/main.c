#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

uint32_t getRingCount(uint32_t value, uint32_t ring)
{
    uint16_t width = ring * 2 - 1;
    uint16_t height = width - 2;
    uint32_t numbers_in_ring = width * 2 + height * 2;
    return numbers_in_ring;
}

void day1()
{
    // get data
    uint32_t input;
    scanf("%ld", &input);

    // get ring width
    uint32_t value_counter = 1;
    uint32_t ring = 2;

    // one line iterator
    while((value_counter += getRingCount(0, ring++)) < input);

    printf("Ring Value: %ld, Ring: %d\n", value_counter, ring);
    uint32_t ring_width = ring - 1;

    // get ring height
    uint16_t width = ring * 2 - 2;
    int32_t offset = (value_counter - input) % width - width / 2;
    printf("Awnser: %ld\n", abs(offset) + ring_width);
}

void day2()
{
    // get data
    uint32_t input;
    scanf("%ld", &input);

    // buffer
    uint32_t buffer[101][101];
    buffer[50][50] = 1;
    uint8_t position_x = 50;
    uint8_t position_y = 50;

    uint32_t steps_moved = 0;
    while(1)
    {
        // east
        steps_moved++;
        for(uint16_t i=0; i<steps_moved; i++)
        {
            position_x += 1;
            uint32_t value1 = buffer[position_x+1][position_y];
            uint32_t value2 = buffer[position_x-1][position_y];
            uint32_t value3 = buffer[position_x][position_y+1];
            uint32_t value4 = buffer[position_x][position_y-1];
            uint32_t value5 = buffer[position_x+1][position_y+1];
            uint32_t value6 = buffer[position_x+1][position_y-1];
            uint32_t value7 = buffer[position_x-1][position_y+1];
            uint32_t value8 = buffer[position_x-1][position_y-1];
            uint32_t value = value1 + value2 + value3 + value4 + value5 + value6 + value7 + value8;
            buffer[position_x][position_y] = value;
            if(value > input)
            {
                printf("Awnser: %ld\n", value);
                return;
            }
        }

        // east
        for(uint16_t i=0; i<steps_moved; i++)
        {
            position_y += 1;
            uint32_t value1 = buffer[position_x+1][position_y];
            uint32_t value2 = buffer[position_x-1][position_y];
            uint32_t value3 = buffer[position_x][position_y+1];
            uint32_t value4 = buffer[position_x][position_y-1];
            uint32_t value5 = buffer[position_x+1][position_y+1];
            uint32_t value6 = buffer[position_x+1][position_y-1];
            uint32_t value7 = buffer[position_x-1][position_y+1];
            uint32_t value8 = buffer[position_x-1][position_y-1];
            uint32_t value = value1 + value2 + value3 + value4 + value5 + value6 + value7 + value8;
            buffer[position_x][position_y] = value;
            if(value > input)
            {
                printf("Awnser: %ld\n", value);
                return;
            }
        }

        // west
        steps_moved++;
        for(uint16_t i=0; i<steps_moved; i++)
        {
            position_x -= 1;
            uint32_t value1 = buffer[position_x+1][position_y];
            uint32_t value2 = buffer[position_x-1][position_y];
            uint32_t value3 = buffer[position_x][position_y+1];
            uint32_t value4 = buffer[position_x][position_y-1];
            uint32_t value5 = buffer[position_x+1][position_y+1];
            uint32_t value6 = buffer[position_x+1][position_y-1];
            uint32_t value7 = buffer[position_x-1][position_y+1];
            uint32_t value8 = buffer[position_x-1][position_y-1];
            uint32_t value = value1 + value2 + value3 + value4 + value5 + value6 + value7 + value8;
            buffer[position_x][position_y] = value;
            if(value > input)
            {
                printf("Awnser: %ld\n", value);
                return;
            }
        }

        // south
        for(uint16_t i=0; i<steps_moved; i++)
        {
            position_y -= 1;
            uint32_t value1 = buffer[position_x+1][position_y];
            uint32_t value2 = buffer[position_x-1][position_y];
            uint32_t value3 = buffer[position_x][position_y+1];
            uint32_t value4 = buffer[position_x][position_y-1];
            uint32_t value5 = buffer[position_x+1][position_y+1];
            uint32_t value6 = buffer[position_x+1][position_y-1];
            uint32_t value7 = buffer[position_x-1][position_y+1];
            uint32_t value8 = buffer[position_x-1][position_y-1];
            uint32_t value = value1 + value2 + value3 + value4 + value5 + value6 + value7 + value8;
            buffer[position_x][position_y] = value;
            if(value > input)
            {
                printf("Awnser: %ld\n", value);
                return;
            }
        }
    }
}

int main()
{
    //day1();
    day2();

    return 0;
}