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
    int32_t* reference;
    int32_t value;
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t
{
    InstructionType_e type;
    Register_t register_a;
    Register_t register_b;
};

//######################################################################################################################

void register_set(Register_t* reg, int32_t value)
{
    *reg->reference = value;
}

int32_t register_get(Register_t* reg)
{
    if (reg->reference == NULL) {
        return reg->value;
    }
    return *reg->reference;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t registers[26] = {0};

    Instruction_t instructions[200] = {0};
    int32_t instruction_size = sizeof(instructions) / sizeof(Instruction_t);
    int32_t instruction_count = 0;

    char inst_string[16];
    char vala_string[8];
    char valb_string[8];
    while (fscanf(p_file, "%s %s %s\n", inst_string, vala_string, valb_string) == 3) {
        Instruction_t* instruction = &instructions[instruction_count];
        if (!memcmp(inst_string, "set", 3)) {
            instruction->type = INST_SET;
        } else if (!memcmp(inst_string, "sub", 3)) {
            instruction->type = INST_SUB;
        } else if (!memcmp(inst_string, "mul", 3)) {
            instruction->type = INST_MUL;
        } else if (!memcmp(inst_string, "jnz", 3)) {
            instruction->type = INST_JNZ;
        }

        if (vala_string[0] >= 'a' && vala_string[0] <= 'z') {
            int32_t index = vala_string[0] - 'a';
            instruction->register_a.reference = &registers[index];
            instruction->register_a.value = 0;
        } else {
            instruction->register_a.reference = NULL;
            instruction->register_a.value = atoi(vala_string);
        }

        if (valb_string[0] >= 'a' && valb_string[0] <= 'z') {
            int32_t index = valb_string[0] - 'a';
            instruction->register_b.reference = &registers[index];
            instruction->register_b.value = 0;
        } else {
            instruction->register_b.reference = NULL;
            instruction->register_b.value = atoi(valb_string);
        }

        instruction_count++;
        if (instruction_count == instruction_size) {
            printf("Buffer overflowed!\n");
            fclose(p_file);
            return 1;
        }
    }

    int32_t mul_counter = 0;
    int32_t val_a = 0;
    int32_t val_b = 0;
    int32_t instruction_ptr = 0;
    while (instruction_ptr >= 0 && instruction_ptr < instruction_count) {
        Instruction_t* instruction = &instructions[instruction_ptr];
            val_a = register_get(&instruction->register_a);
            val_b = register_get(&instruction->register_b);
        switch(instruction->type) {
        case INST_SET:
            register_set(&instruction->register_a, val_b);
            instruction_ptr++;
            break;
        case INST_SUB:
            register_set(&instruction->register_a, val_a - val_b);
            instruction_ptr++;
            break;
        case INST_MUL:
            mul_counter++;
            register_set(&instruction->register_a, val_a * val_b);
            instruction_ptr++;
            break;
        case INST_JNZ:
            if (val_a != 0) {
                instruction_ptr += val_b;
            } else {
                instruction_ptr++;
            }
            break;
        }
    }

    for (int32_t i=0; i<26; i++) {
        printf("%c: %d\n", i + 'A', registers[i]);
    }


    fclose(p_file);
    printf("Answer: %d\n", mul_counter);
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