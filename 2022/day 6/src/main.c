#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

#define sequence_length 4

// ##################################################
// data structures



// ##################################################
// processing



// ##################################################
// part 1

void part1(int argc, char *argv[])
{
    clock_t start = clock();

    FILE *file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    uint32_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char*)malloc(length);
    fread(buffer, sizeof(char), length, file);

    uint32_t counter = sequence_length;
    while(1)
    {
        // increment counter
        counter++;

        // create alphabet counter
        uint8_t flag = 0;
        uint32_t bit = 0;
        for(uint8_t i=0; i<sequence_length; i++)
        {
            if(bit & (1<<(buffer[counter - 1 - i] - 'a')))
            {
                flag = 1;
                break;
            }
            bit |= (1<<(buffer[counter - 1 - i] - 'a'));
        }
        if(!flag)
        {
            clock_t end = clock();
            printf("Duration: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
            printf("Awnser: %lu\n", counter);
            return 0;
        }
    }
    return 1;
}

// ##################################################
// part 2

void part2(int argc, char *argv[])
{

}

// ##################################################
// main

int main(int argc, char *argv[])
{
    part1(argc, argv);
    part2(argc, argv);

    return 0;
}