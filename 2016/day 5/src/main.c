#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "md5.h"

void day1()
{
    printf("Start\n");
    char *key = "uqwqemis%ld";

    uint32_t value=0;
    uint8_t counter = 0;
    while(1)
    {
        char buffer[16];
        sprintf(buffer, key, value);
        char *output = GetMD5String(buffer, 20);
        if(!strncmp(output, "00000", 5))
        {
            printf("%c", output[5]);
            counter++;
            if(counter == 8)
                break;
        }
        value++;
    }
    printf("\n");
}

void day2()
{
    printf("Start\n");
    char *key = "uqwqemis%ld";

    uint32_t value=0;
    uint8_t counter = 0x00;
    char awnser[9] = "--------\0";
    while(1)
    {
        char buffer[32];
        sprintf(buffer, key, value);
        char *output = GetMD5String(buffer, 32);
        value++;
        if(!strncmp(output, "00000", 5))
        {
            // cancel if invalid
            if(output[5] < '0' || output[5] > '7')
                continue;

            // check if already filled
            if(awnser[output[5] - '0'] != '-')
                continue;

            awnser[output[5] - '0'] = output[6];
            printf("%s\n", awnser);

            // check if full
            uint8_t flag = 0;
            for(uint8_t i=0; i<8; i++)
                if(awnser[i] == '-')
                    flag = 1;
            if(!flag)
                break;
        }
        
    }
    printf("Awnser: %s\n", awnser);
}

int main()
{
    //day1();
    day2();

    return 0;
}
