#include "string.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

//####################### DAY 1 #######################//

// data structures

// functions

// execution call
void executeDayOne(char* argv)
{
    // fetch lowest value
    uint8_t *addresses = (uint8_t*)calloc(0xFFFFFFFF, 1);

    FILE *file = fopen(argv, "r");
    char line_buffer[64];
    while(fgets(line_buffer, 64, file))
    {
        // tokenize output
        char *tokens[2];
        tokens[0] = strtok(line_buffer, "-");
        tokens[1] = strtok(NULL, "-");
        
        // fetch values
        uint32_t first_value = atoi(tokens[0]);
        uint32_t second_value = atoi(tokens[1]);

        // fill array
        for(uint32_t i=first_value; i<=second_value; i++)
            addresses[i] = 1;
    }

    // search for first instance
    for(uint32_t i=0; i<0xFFFFFFFF; i++)
    {
        if(!addresses[i])
        {
            printf("Answer: %ld\n", i);
            break;
        }
    }

    free(addresses);
    fclose(file);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    // fetch lowest value
    uint64_t size = 0xFFFFFFFF;
    uint8_t *addresses = (uint8_t*)calloc(size + 1, 1);

    FILE *file = fopen(argv, "r");
    char line_buffer[64];
    while(fgets(line_buffer, 64, file))
    {
        // tokenize output
        char *tokens[2];
        tokens[0] = strtok(line_buffer, "-");
        tokens[1] = strtok(NULL, "-");
        
        // fetch values
        uint64_t first_value = atoll(tokens[0]);
        uint64_t second_value = atoll(tokens[1]);

        // fill array
        for(uint64_t i=first_value; i<=second_value; i++)
            addresses[i] = 1;
    }

    // count filled spaces
    uint32_t counter = 0;
    for(uint64_t i=0; i<size + 1; i++)
        if(!addresses[i])
            counter++;

    printf("Answer: %lu\n", counter);

    free(addresses);
    fclose(file);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}