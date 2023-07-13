#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

// ##################################################
// data structures



// ##################################################
// processing



// ##################################################
// part 1

void part1()
{
    // process
    uint32_t score = 0;
    FILE *file = fopen("data.txt", "r");
    while(1)
    {
        // get data
        char buffer[64];
        fgets(buffer, 64, file);
        if(buffer[0] == '0') break;
        uint8_t len = strlen(buffer) - 1;
        char part1[32];
        char part2[32];
        memcpy(part1, buffer, len/2);
        memcpy(part2, buffer+len/2, len/2);

        // get match
        char match;
        for(uint8_t i=0; i<(len/2); i++)
            for(uint8_t j=0; j<(len/2); j++)
                if(part1[i] == part2[j])
                {
                    match = part1[i];
                    break;
                }
        
        // get score
        if(match <= 'Z')
            score += match - 64 + 26;
        else
            score += match - 96;
    }

    fclose(file);
    printf("Answer: %lu\n", score);
}

// ##################################################
// part 2

void part2()
{
    // process
    uint32_t score = 0;
    FILE *file = fopen("data.txt", "r");
    while(1)
    {
        // get data
        char buffer1[64];
        fgets(buffer1, 64, file);
        uint8_t len1 = strlen(buffer1) - 1;
        if(buffer1[0] == '0') break;

        char buffer2[64];
        fgets(buffer2, 64, file);
        uint8_t len2 = strlen(buffer2) - 1;

        char buffer3[64];
        fgets(buffer3, 64, file);
        uint8_t len3 = strlen(buffer3) - 1;

        // get match
        char match = '2';
        for(uint8_t i=0; i<(len1); i++)
            for(uint8_t j=0; j<(len2); j++)
                if(buffer1[i] == buffer2[j])
                    for(uint8_t k=0; k<(len3); k++)
                        if(buffer1[i] == buffer3[k])
                            match = buffer1[i];
        
        // get score
        if(match <= 'Z')
            score += match - 64 + 26;
        else
            score += match - 96;
    }

    fclose(file);
    printf("Answer: %lu\n", score);
}

// ##################################################
// main

int main()
{
    part1();
    part2();

    return 0;
}