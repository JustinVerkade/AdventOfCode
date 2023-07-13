#include <stdio.h>
#include <stdlib.h>

typedef struct triangle_t triangle_t;
struct triangle_t
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

void part1()
{
    unsigned int valid_triads = 0;

    short counter = 0;
    while(1)
    {
        char line[64];

        triangle_t triad;
        scanf("%d %d %d", &triad.a, &triad.b, &triad.c);

        // check validity
        short valid = 0;
        if((triad.a + triad.b) > triad.c) valid++;
        if((triad.b + triad.c) > triad.a) valid++;
        if((triad.c + triad.a) > triad.b) valid++;
        if(valid == 3) valid_triads++;

        printf("Valid triads: %d -> %d\n", valid_triads, ++counter);
    }
}

void part2()
{
    unsigned int valid_triads = 0;

    short counter = 0;
    while(1)
    {
        char line[64];

        triangle_t triad1, triad2, triad3;
        scanf("%d %d %d", &triad1.a, &triad2.a, &triad3.a);
        scanf("%d %d %d", &triad1.b, &triad2.b, &triad3.b);
        scanf("%d %d %d", &triad1.c, &triad2.c, &triad3.c);

        // check validity 1
        short valid = 0;
        if((triad1.a + triad1.b) > triad1.c) valid++;
        if((triad1.b + triad1.c) > triad1.a) valid++;
        if((triad1.c + triad1.a) > triad1.b) valid++;
        if(valid == 3) valid_triads++;

        // check validity 2
        valid = 0;
        if((triad2.a + triad2.b) > triad2.c) valid++;
        if((triad2.b + triad2.c) > triad2.a) valid++;
        if((triad2.c + triad2.a) > triad2.b) valid++;
        if(valid == 3) valid_triads++;

        // check validity 3
        valid = 0;
        if((triad3.a + triad3.b) > triad3.c) valid++;
        if((triad3.b + triad3.c) > triad3.a) valid++;
        if((triad3.c + triad3.a) > triad3.b) valid++;
        if(valid == 3) valid_triads++;

        printf("Valid triads: %d -> %d\n", valid_triads, ++counter);
    }
}

int main()
{
    //part1();
    part2();

    return 0;
}