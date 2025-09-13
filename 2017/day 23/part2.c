//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum InstructionType_e InstructionType_e;
enum InstructionType_e {
    INST_SET=0,
    INST_SUB,
    INST_MUL,
    INST_JNZ,
};

typedef struct Register_t Register_t;
struct Register_t {
    int64_t* reference;
    int64_t value;
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t
{
    InstructionType_e type;
    Register_t register_a;
    Register_t register_b;
};

//######################################################################################################################

void register_set(Register_t* reg, int64_t value)
{
    *reg->reference = value;
}

int64_t register_get(Register_t* reg)
{
    if (reg->reference == NULL) {
        return reg->value;
    }
    return *reg->reference;
}

int solve_puzzle(char* file_name)
{
    int64_t a = 1;
    int64_t b = 0;
    int64_t c = 0;
    int64_t d = 0;
    int64_t e = 0;
    int64_t f = 0;
    int64_t g = 0;
    int64_t h = 0;

    b = 65;
    c = b;
    if (a != 0) {
        goto jump_3;
    }
    goto jump_4;
    jump_3:
    b = b * 100;
    b = b + 100000;
    c = b;
    c = c + 17000;
    jump_4:
    jump_32:
    f = 1;
    d = 2;
    jump_24:
    e = 2;
    jump_20:
    g = d;
    g = g * e;
    g = g - b;
    if (g != 0) {
        goto jump_15;
    }
    f = 0;
    jump_15:
    e = e + 1;
    g = e;
    g = g - b;
    if (g != 0) {
        goto jump_20;
    }
    d = d + 1;
    g = d;
    g = g - b;
    if (g != 0) {
        goto jump_24;
    }
    if (f != 0) {
        goto jump_25;
    }
    h = h + 1;
    jump_25:
    g = b;
    g = g - c;
    printf("H: %\d (G: %ld)\n", h, g);
    if (g != 0) {
        goto jump_29;
    }
    goto jump_30;
    jump_29:
    b = b + 17;
    goto jump_32;

    jump_30:

    printf("Answer: %ld\n", h);
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