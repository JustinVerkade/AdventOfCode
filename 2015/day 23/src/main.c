#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

// ###################################################################
// state machine registers

typedef struct registers registers;
struct registers
{
    int32_t addr;
    int32_t a;
    int32_t b;
};

// ###################################################################
// parse line

void execute(char *cmd, registers *reg)
{
    // halve
    if(!strncmp(cmd, "hlf", 3))
    {
        int32_t *tar = cmd[4] == 'a' ? &reg->a : &reg->b;
        *tar /= 2;
        reg->addr++;
    }

    // triple
    if(!strncmp(cmd, "tpl", 3))
    {
        int32_t *tar = cmd[4] == 'a' ? &reg->a : &reg->b;
        *tar *= 3;
        reg->addr++;
    }

    // increment
    if(!strncmp(cmd, "inc", 3))
    {
        int32_t *tar = cmd[4] == 'a' ? &reg->a : &reg->b;
        *tar += 1;
        reg->addr++;
    }

    // jump
    if(!strncmp(cmd, "jmp", 3))
    {
        reg->addr += atoi(cmd + 4);
    }

    // jump if even
    if(!strncmp(cmd, "jie", 3))
    {
        int32_t *tar = cmd[4] == 'a' ? &reg->a : &reg->b;
        uint8_t even = (*tar % 2) == 0;
        reg->addr += even ? atoi(cmd + 7) : 1;
    }

    // jump if one
    if(!strncmp(cmd, "jio", 3))
    {
        int32_t *tar = cmd[4] == 'a' ? &reg->a : &reg->b;
        reg->addr += *tar == 1 ? atoi(cmd + 7) : 1;
    }
}

// ###################################################################
// day 1

void day1()
{
    // fetch data
    uint32_t instruction_max = 0;
    char instructions[2000][16];
    char instruction_buffer[16];
    while(fgets(instruction_buffer, 16, stdin))
    {
        if(instruction_buffer[0] == 's') break;
        memcpy(instructions[instruction_max], instruction_buffer, 16);
        instruction_max++;
    }

    // create register
    registers reg;
    reg.addr = 0;
    reg.b = 0;
    reg.a = 0;

    // execution program
    while(reg.addr >= 0 && reg.addr < instruction_max)
        execute(instructions[reg.addr], &reg);

    printf("Awnser part 1: %d\n", reg.b);
}

// ###################################################################
// day 2

void day2()
{
    // fetch data
    uint32_t instruction_max = 0;
    char instructions[2000][16];
    char instruction_buffer[16];
    while(fgets(instruction_buffer, 16, stdin))
    {
        if(instruction_buffer[0] == 's') break;
        memcpy(instructions[instruction_max], instruction_buffer, 16);
        instruction_max++;
    }

    // create register
    registers reg;
    reg.addr = 0;
    reg.b = 0;
    reg.a = 1;

    // execution program
    while(reg.addr >= 0 && reg.addr < instruction_max)
        execute(instructions[reg.addr], &reg);

    printf("Awnser part 2: %d\n", reg.b);
}

// ###################################################################
// main

int main()
{
    day1();
    day2();

    return 0;
}
