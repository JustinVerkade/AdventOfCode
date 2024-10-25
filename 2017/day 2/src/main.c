#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

void day1()
{
    // fetch data
    uint32_t buffer[16][16];
    for (size_t y = 0; y<16; y++)
    {
        for (size_t x = 0; x<16; x++)
        {
            char number[16];
            scanf("%s", number);
            uint32_t value = atoi(number);
            buffer[y][x] = value;
        }
    }

    // find highest
    uint32_t total_counter = 0;
    for(size_t y=0; y<16; y++)
    {
        uint32_t max_row = 0;
        uint32_t min_row = 99999999;

        for (size_t x = 0; x<16; x++)
        {
            if(buffer[y][x] > max_row) max_row = buffer[y][x];
            if(buffer[y][x] < min_row) min_row = buffer[y][x];
        }
        
        uint32_t delta = max_row - min_row;
        total_counter += delta;
    }

    printf("Awnser: %ld\n", total_counter);
}

void day2()
{
    // fetch data
    uint32_t buffer[16][16];
    for(size_t y = 0; y<16; y++)
    {
        for(size_t x = 0; x<16; x++)
        {
            char number[16];
            scanf("%s", number);
            uint32_t value = atoi(number);
            buffer[y][x] = value;
        }
    }

    // find highest
    uint32_t total_counter = 0;
    for(size_t y=0; y<16; y++)
    {
        // loop first value
        for(size_t x1 = 0; x1<16; x1++)
        {
            // loop second value
            for(size_t x2 = 0; x2<16; x2++)
            {
                if(x1 == x2) continue;
                
                // devisable
                if(buffer[y][x1] % buffer[y][x2] == 0)
                {
                    printf("%ld, %ld \n", buffer[y][x1], buffer[y][x2]);
                    uint32_t awnser = buffer[y][x1] / buffer[y][x2];
                    total_counter += awnser;
                    x1 = 16;
                    x2 = 16;
                }
            }
        }
    }

    printf("Awnser: %ld\n", total_counter);
}


int main()
{
    //day1();
    day2();

    return 0;
}
