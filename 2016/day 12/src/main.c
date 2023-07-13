#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ##################################################
// data structures



// ##################################################
// processing

uint32_t getLines(char *file_name)
{
    FILE *file = fopen(file_name, "r");
    uint32_t counter = 0;
    char buffer[16];
    while(fgets(buffer, 16, file))
        counter++;
    fclose(file);
    return counter;
}

char** getInstructions(char *file_name, uint32_t lines)
{
    char **instructions = (char**)malloc(lines * sizeof(char*));
    FILE *file = fopen(file_name, "r");
    for(uint32_t l=0; l<lines; l++)
    {
        char buffer[1024];
        fgets(buffer, 1024, file);
        char *instruction = (char*)calloc(strlen(buffer) + 2, sizeof(char));
        memcpy(instruction, buffer, strlen(buffer));
        instructions[l] = instruction;
    }
    fclose(file);
    return instructions;
}

void destroyInstructions(char **instructions, uint32_t lines)
{
    for(uint32_t l=0; l<lines; l++)
        free(instructions[l]);
    free(instructions);
}

uint8_t processInstruction(char **instructions, uint32_t lines, int32_t *registers)
{
    static int32_t instruction_counter = 0;
    
    // process instructions
    char *instruction = instructions[instruction_counter];
    if(!strncmp(instruction, "cpy", 3))
    {
        char temp[8];
        char reg1[8];
        char reg2[8];
        sscanf(instruction, "%s %s %s", temp, reg1, reg2);

        // register 1
        int val = 0;
        if((reg1[0] >= '0' && reg1[0] <= '9') || reg1[0] == '-')
        {
            val = atoi(reg1);
        }
        else
        {
            val = registers[reg1[0] - 'a'];
        }

        // register 2
        registers[reg2[0] - 'a'] = val;

        // increment counter
        instruction_counter++;
    }
    else if(!strncmp(instruction, "inc", 3))
    {
        char temp[8];
        char reg1[8];
        sscanf(instruction, "%s %s", temp, reg1);

        // execute instruction
        registers[reg1[0] - 'a']++;

        // increment counter
        instruction_counter++;
    }
    else if(!strncmp(instruction, "dec", 3))
    {
        char temp[8];
        char reg1[8];
        sscanf(instruction, "%s %s", temp, reg1);

        // execute instruction
        registers[reg1[0] - 'a']--;

        // increment counter
        instruction_counter++;
    }
    else if(!strncmp(instruction, "jnz", 3))
    {
        char temp[8];
        char reg1[8];
        char reg2[8];
        sscanf(instruction, "%s %s %s", temp, reg1, reg2);

        // register 1
        int val1 = 0;
        if((reg1[0] >= '0' && reg1[0] <= '9') || reg1[0] == '-')
        {
            val1 = atoi(reg1);
        }
        else
        {
            val1 = registers[reg1[0] - 'a'];
        }

        // register 2
        int val2 = 0;
        if((reg2[0] >= '0' && reg2[0] <= '9') || reg2[0] == '-')
        {
            val2 = atoi(reg2);
        }
        else
        {
            val2 = registers[reg2[0] - 'a'];
        }

        // check if jump
        if(val1)
        {
            instruction_counter += val2;
        }
        else
        {
            instruction_counter++;
        }
    }
    return instruction_counter < 0 || instruction_counter >= lines; 
}

// ##################################################
// part 1

void part1(int argc, char *argv[])
{
    // fetch instruction count
    uint32_t lines = getLines(argv[1]);

    // allocate memory
    char **instructions = getInstructions(argv[1], lines);

    int32_t registers[12];
    for(uint8_t i=0; i<12; i++)
        registers[i] = 0;

    for(;;) if(processInstruction(instructions, lines, registers))
        break;

    // awnser
    printf("Awnser: %ld\n", registers[0]);

    // deallocate memory
    destroyInstructions(instructions, lines);
}

// ##################################################
// part 2

void part2(int argc, char *argv[])
{
    // fetch instruction count
    uint32_t lines = getLines(argv[1]);

    // allocate memory
    char **instructions = getInstructions(argv[1], lines);

    int32_t registers[12];
    for(uint8_t i=0; i<12; i++)
        registers[i] = 0;
    registers[2] = 1;

    for(;;) if(processInstruction(instructions, lines, registers))
        break;

    // awnser
    printf("Awnser: %ld\n", registers[0]);

    // deallocate memory
    destroyInstructions(instructions, lines);
}

// ##################################################
// main

int main(int argc, char *argv[])
{
    //part1(argc, argv);
    part2(argc, argv);

    return 0;
}