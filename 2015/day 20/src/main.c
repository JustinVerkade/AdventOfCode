#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

int32_t presents(int32_t house_number)
{
    int32_t presents = house_number * 10;
    for(int32_t i=1; i<=(house_number/2); i++)
    {
        if(house_number % i == 0)
        {
            presents += i * 10;
        }
    }
    return presents;
}

void day1()
{
    // calculate
    int32_t house_number = 500000;
    while(presents(++house_number) < 33100000);
    printf("Awnser: %ld\n", house_number);
}

int32_t presents2(int32_t house_number)
{
    int32_t presents = house_number * 11;
    for(int32_t i=1; i<=(house_number/2); i++)
    {
        if(house_number % i == 0 && house_number / i <= 50)
        {
            presents += i * 11;
        }
    }
    // printf("%d %d\n", house_number, presents);
    return presents;
}

void day2()
{
    // calculate
    int32_t house_number = 700000;
    while(presents2(++house_number) < 33100000);
    printf("Awnser: %ld\n", house_number);
}

int main()
{
    //day1();
    day2();

    return 0;
}
