#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define GRID_SIZE 10000

void getPositionDiagonal(uint64_t value, uint64_t *x, uint64_t *y)
{
    uint64_t counter = 0;
    uint64_t diagonal_len = 1;
    *x = 0;
    *y = 0;

    for(uint64_t i=0; i<(value-1); i++)
    {
        // update position
        if(*y == 0)
        {
            *x = 0;
            *y = diagonal_len;
            diagonal_len++;
            printf("%llu\n", diagonal_len);
        }
        else
        {
            (*y)--;
            (*x)++;
        }
    }
}

uint64_t getPosition(uint64_t x, uint64_t y)
{
    return x * GRID_SIZE + y;
}

void printGrid(uint64_t *grid)
{
    for(uint32_t y=0; y<GRID_SIZE; y++)
    {
        for(uint32_t x=0; x<GRID_SIZE; x++)
        {
            printf("%10llu, ", grid[getPosition(x, y)]);
        }
        printf("\n");
    }
}

void day1()
{
    // calculate position index
    uint64_t value = 20151125;
    uint64_t counter = 1;
    uint64_t diagonal_len = 1;
    uint64_t x = 0;
    uint64_t y = 0;

    while(1)
    {
        // increment counter
        counter++;
        value = (value * 252533) % 33554393;

        // update position
        if(y == 0)
        {
            x = 0;
            y = diagonal_len;
            diagonal_len++;
        }
        else
        {
            y--;
            x++;
        }
        
        //printf("%llu, %llu\n", x, y);
        if(x == 3019-1 && y == 3010-1)
        {
            printf("Awnser %llu\n", value);
            break;
        }
        
    }
}

void day2()
{
    
}

int main()
{
    day1();
    day2();

    return 0;
}
