#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int localPos(int x)
{
    x += 31 * 22;
    return x % 31;
}

void part1()
{
    const int lines = 323;
    // data aquisition
    int i;
    char data[lines][32];
    for(i=0; i<lines; i++)
    {
        printf("%i: ", i);
        scanf("%s", data[i]);
        data[i][31] = '\0';
    }

    // calculations
    int trees = 0;
    int pos_x;
    int pos_y;

    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=3, pos_y+=1)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
        {
            data[pos_y][localPos(pos_x)] = 'X';
            trees++;
        }
        else
        {
            data[pos_y][localPos(pos_x)] = 'O';
        }
    }

    for(i=0; i<lines; i++)
    {
        printf("%i: %s\n", i, data[i]);
    }
    printf("Trees: %i\n", trees);
}

void part2()
{
    const int lines = 323;
    // data aquisition
    int i;
    char data[lines][32];
    for(i=0; i<lines; i++)
    {
        printf("%i: ", i);
        scanf("%s", data[i]);
        data[i][31] = '\0';
    }

    // calculations 1, 1
    int trees0 = 0;
    int pos_x;
    int pos_y;

    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=1, pos_y+=1)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
            trees0++;
    }

    // calculations 3, 1
    int trees1 = 0;
    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=3, pos_y+=1)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
            trees1++;
    }

    // calculations 5, 1
    int trees2 = 0;
    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=5, pos_y+=1)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
            trees2++;
    }

    // calculations 7, 1
    int trees3 = 0;
    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=7, pos_y+=1)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
            trees3++;
    }

    // calculations 1, 2
    int trees4 = 0;
    for(pos_x=0, pos_y=0; pos_y<lines; pos_x+=1, pos_y+=2)
    {
        if(data[pos_y][localPos(pos_x)] == '#')
            trees4++;
    }

    printf("Trees: %lu\n", (long unsigned)trees0 * (long unsigned)trees1 * (long unsigned)trees2 * (long unsigned)trees3 * (long unsigned)trees4);
}

int main()
{
    part1();
    part2();

    return 0;
}
