#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures



// ##################################################
// processing

void getValues(char *str, int64_t *v1, int64_t *v2)
{
    // fetch halfway
    char *c = str;
    for(; *c!='-'; c++);
    *c = 0x00;
    *v1 = atoi(str);
    *v2 = atoi(c+1);
}

// ##################################################
// part 1

void part1()
{
    // process
    uint64_t counter = 0;
    while(1)
    {
        // fetch data
        char buffer[64];
        memset(buffer, 0x00, 32);
        fgets(buffer, 32, stdin);

        // break loop
        if(buffer[0] == 's') break;

        // declare buffers
        char elf_1[16];
        char elf_2[16];
        memset(elf_1, 0x00, 16);
        memset(elf_2, 0x00, 16);

        // process halves
        char *c = buffer;
        for(; *c!=','; c++);
        memcpy(elf_1, buffer, c - buffer);
        memcpy(elf_2, c+1, strlen(buffer) - (c - buffer) - 2);

        // process values
        int64_t elf_1_value_1;
        int64_t elf_1_value_2;
        getValues(elf_1, &elf_1_value_1, &elf_1_value_2);

        int64_t elf_2_value_1;
        int64_t elf_2_value_2;
        getValues(elf_2, &elf_2_value_1, &elf_2_value_2);

        // test overlap
        if((elf_1_value_1 == elf_2_value_1 || elf_1_value_2 == elf_2_value_2) || 
                (elf_1_value_1 > elf_2_value_1 && elf_1_value_2 < elf_2_value_2) || 
                (elf_1_value_1 < elf_2_value_1 && elf_1_value_2 > elf_2_value_2))
        {
            counter += 1;
        }
    }
    printf("Awnser: %llu\n", counter);
}

// ##################################################
// part 2

void part2()
{
    // process
    uint64_t counter = 0;
    while(1)
    {
        // fetch data
        char buffer[64];
        memset(buffer, 0x00, 32);
        fgets(buffer, 32, stdin);

        // break loop
        if(buffer[0] == 's') break;

        // declare buffers
        char elf_1[16];
        char elf_2[16];
        memset(elf_1, 0x00, 16);
        memset(elf_2, 0x00, 16);

        // process halves
        char *c = buffer;
        for(; *c!=','; c++);
        memcpy(elf_1, buffer, c - buffer);
        memcpy(elf_2, c+1, strlen(buffer) - (c - buffer) - 2);

        // process values
        int64_t elf_1_value_1;
        int64_t elf_1_value_2;
        getValues(elf_1, &elf_1_value_1, &elf_1_value_2);

        int64_t elf_2_value_1;
        int64_t elf_2_value_2;
        getValues(elf_2, &elf_2_value_1, &elf_2_value_2);

        // test overlap
        if((elf_1_value_1 >= elf_2_value_1 && elf_1_value_1 <= elf_2_value_2) || 
                (elf_1_value_2 >= elf_2_value_1 && elf_1_value_2 <= elf_2_value_2) || 
                (elf_2_value_1 >= elf_1_value_1 && elf_2_value_1 <= elf_1_value_2) || 
                (elf_2_value_2 >= elf_1_value_1 && elf_2_value_2 <= elf_1_value_2))
        {
            counter += 1;
        }
    }
    printf("Awnser: %llu\n", counter);
}

// ##################################################
// main

int main()
{
    //part1();
    part2();

    return 0;
}