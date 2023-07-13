#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "md5.h"

void day1()
{
    printf("Start\n");
    char *key = "iwrupvqb%ld";

    uint32_t value=0;
    while(1)
    {
        char buffer[16];
        sprintf(buffer, key, value);
        char *output = GetMD5String(buffer, 20);
        if(!strncmp(output, "00000", 5))
            break;
        value++;
    }

    printf("Awnser: %ld\n", value);
}

void day2()
{
    printf("Start\n");
    char *key = "iwrupvqb%lld";

    uint64_t value=0;
    while(1)
    {
        char buffer[32];
        sprintf(buffer, key, value);
        char *output = GetMD5String(buffer, 32);
        if(!strncmp(output, "000000", 6))
            break;
        value++;
    }

    printf("Awnser: %ld\n", value);
}

int main()
{
    day1();
    day2();

    return 0;
}
