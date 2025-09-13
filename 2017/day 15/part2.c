//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines



//######################################################################################################################

uint32_t generator_a_next(uint32_t value)
{
    uint64_t multiplied = (uint64_t)value * 16807llu;
    uint32_t remainder = multiplied % 2147483647;
}

uint32_t generator_b_next(uint32_t value)
{
    uint64_t multiplied = (uint64_t)value * 48271llu;
    uint32_t remainder = multiplied % 2147483647;
}

int solve_puzzle(char* file_name)
{
    uint32_t generator_a = 783;
    uint32_t generator_b = 325;

    uint32_t counter = 0;
    for (int32_t i=0; i<5000000; i++) {
        do {
            generator_a = generator_a_next(generator_a);
        } while (generator_a % 4 != 0);
        do {
            generator_b = generator_b_next(generator_b);
        } while (generator_b % 8 != 0);
        if ((generator_a & 0x0000FFFF) == ((generator_b & 0x0000FFFF))) {
            counter++;
        }
    }
    printf("Answer: %d\n", counter);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}