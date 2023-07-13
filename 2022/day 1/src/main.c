#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void part1()
{
    // fetch data
    uint32_t best_value = 0;
    uint32_t current_value = 0;
    char buffer[16];
    while(1)
    {
        // fetch value
        fgets(buffer, 16, stdin);
        current_value += atoi(buffer);

        // check double newline
        uint8_t len = strlen(buffer);
        if(len == 1 && buffer[0] == '\n')
        {
            printf("Current val = %ld\n", current_value);
            if(current_value > best_value)
                best_value = current_value;
            current_value = 0;
        }

        if(buffer[0] == 's')
            break;
    }
    printf("Best awnser: %ld\n", best_value);
}

void part2()
{
    // fetch data
    uint32_t current_value = 0;
    uint32_t best_elfs[3] = {0, 0, 0};
    char buffer[16];
    while(1)
    {
        // fetch value
        fgets(buffer, 16, stdin);
        current_value += atoi(buffer);

        // check double newline
        uint8_t len = strlen(buffer);
        if(len == 1 && buffer[0] == '\n')
        {
            printf("current val = %ld\n", current_value);

            // find weakest elf
            uint8_t worst_index = 0;
            uint32_t worst_score = 999999999;
            for(uint8_t i=0; i<3; i++)
            {
                if(best_elfs[i] < worst_score)
                {
                    worst_score = best_elfs[i];
                    worst_index = i;
                }
            }

            // update score
            if(best_elfs[worst_index] < current_value)
                best_elfs[worst_index] = current_value;

            // reset current
            current_value = 0;
        }

        if(buffer[0] == 's')
            break;
    }

    // sum best 3
    uint32_t best_value = 0;
    for(uint8_t i=0; i<3; i++)
        best_value += best_elfs[i];


    printf("Best awnser: %ld\n", best_value);
}

int main()
{
    //part1();
    part2();

    return 0;
}