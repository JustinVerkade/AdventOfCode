#include "../inc/main.h"
#include "stdint.h"

//####################### DAY 1 #######################//

// data structures

// functions

// execution call
void executeDayOne(char* argv)
{
    FILE *file = fopen(argv, "r");
    char number[32];
    long total_sum = 0;
    while(fgets(number, 32, file))
    {
        int value = atoi(number);
        total_sum += value;
    }

    // answer
    printf("Answer: %d\n", total_sum);

    fclose(file);
}

//####################### DAY 2 #######################//

// data structures

// functions

// execution call
void executeDayTwo(char* argv)
{
    FILE *file = fopen(argv, "r");
    char number[32];
    long total_sum = 0;
    int64_t buffer_index = 1;
    int64_t buffer[200000] = {0};

    while(1)
    {
        fseek(file, 0, SEEK_SET);
        while(fgets(number, 32, file))
        {
            int value = atoi(number);
            total_sum += value;

            // check if sum already exists
            for(int32_t index=0; index<buffer_index; index++)
            {
                if(buffer[index] == total_sum)
                {
                    printf("Discovered!: %d\n", total_sum);
                    return;
                }
            }
            buffer[buffer_index++] = total_sum;
        }
    }

    // answer
    printf("Answer: %d\n", total_sum);

    fclose(file);
}

//################# PROGRAM EXECUTION #################//

int main(int argc, char **argv)
{
    executeDayOne(argv[1]);
    executeDayTwo(argv[1]);
}