#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void part1()
{
    // receive data
    int list[3000];
    int a;
    memset(list, 0, sizeof(int) * 3000);
    for(a=0; a<3000; a++)
    {
        int val = -1;
        scanf("%i", &val);
        if(val == -1) break;
        list[a] = val;
    }
    
    // parse data

    // filter variables that are to large for a
    int b;
    for(b=0; b<=a; b++)
    {
        int c;
        for(c=0; c<=a; c++)
            if(list[b] + list[c] == 2020)
                printf("Awnser: %i, %i -> %i\n", list[b], list[c], list[b] * list[c]);
    }
}

void part2()
{
    // receive data
    int list[3000];
    int a;
    memset(list, 0, sizeof(int) * 3000);
    for(a=0; a<3000; a++)
    {
        int val = -1;
        scanf("%i", &val);
        if(val == -1) break;
        list[a] = val;
    }
    
    // parse data

    // filter variables that are to large for a
    int b;
    for(b=0; b<=a; b++)
    {
        int c;
        for(c=0; c<=a; c++)
        {
            int d;
            for(d=0; d<=a; d++)
                if(list[b] + list[c] + list[d] == 2020)
                    printf("Awnser: %i, %i, %i -> %i\n", list[b], list[c], list[d], list[b] * list[c] * list[d]);
        }
    }
}

int main()
{
    //part1();
    //part2();
    return 0;
}
