//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum InstructionType_e InstructionType_e;
enum InstructionType_e {
    TYPE_INC=0,
    TYPE_DEC,
    TYPE_TGL,
    TYPE_CPY,
    TYPE_JNZ,
    TYPE_OUT,
    TYPE_ERR,
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t {
    InstructionType_e type;
    int64_t* input_reg;
    int64_t input_val;
    int64_t* output_reg;
    int64_t output_val;
};

//######################################################################################################################
// functions

void instruction_from_string(Instruction_t* instruction, char* string, int64_t* registers)
{
    char* instruction_tag = strtok(string, " ");
    if (!memcmp("inc", instruction_tag, 3)) {
        instruction->type = TYPE_INC;
    } else if (!memcmp("dec", instruction_tag, 3)) {
        instruction->type = TYPE_DEC;
    } else if (!memcmp("tgl", instruction_tag, 3)) {
        instruction->type = TYPE_TGL;
    } else if (!memcmp("cpy", instruction_tag, 3)) {
        instruction->type = TYPE_CPY;
    } else if (!memcmp("jnz", instruction_tag, 3)) {
        instruction->type = TYPE_JNZ;
    } else if (!memcmp("out", instruction_tag, 3)) {
        instruction->type = TYPE_OUT;
    } else {
        instruction->type = TYPE_ERR;
    }

    char* register1_tag = strtok(NULL, " ");
    if (register1_tag[0] >= 'a' && register1_tag[0] <= 'z') {
        int8_t register_index = register1_tag[0] - 'a';
        instruction->input_reg = &registers[register_index];
        instruction->input_val = 0;
    } else {
        int64_t register_value = atoi(register1_tag);
        instruction->input_reg = NULL;
        instruction->input_val = register_value;
    }

    char* register2_tag = strtok(NULL, " ");
    if (register2_tag == NULL) {
        instruction->output_reg = NULL;
        instruction->output_val = 0;
    } else if (register2_tag[0] >= 'a' && register2_tag[0] <= 'z') {
        int8_t register_index = register2_tag[0] - 'a';
        instruction->output_reg = &registers[register_index];
        instruction->output_val = 0;
    } else {
        int64_t register_value = atoi(register2_tag);
        instruction->output_reg = NULL;
        instruction->output_val = register_value;
    }
}

void instruction_toggle(Instruction_t* instruction)
{
    switch (instruction->type) {
    case TYPE_INC:
        instruction->type = TYPE_DEC;
        break;
    case TYPE_DEC:
        instruction->type = TYPE_INC;
        break;
    case TYPE_TGL:
        instruction->type = TYPE_INC;
        break;
    case TYPE_CPY:
        instruction->type = TYPE_JNZ;
        break;
    case TYPE_JNZ:
        instruction->type = TYPE_CPY;
        break;
    case TYPE_OUT:
        instruction->type = TYPE_INC;
        break;
    case TYPE_ERR:
        printf("Ftw this should not happen!\n");
        return;
    }
}

int8_t test_register_value(int64_t start_value, int64_t* registers, Instruction_t* base_instructions, uint64_t base_instructions_count)
{
    printf("\rTesting [%8ld]: Pattern counter: %12ld ", start_value, 0lu);

    // set instructions
    Instruction_t instructions[200];
    uint64_t instruction_size = sizeof(instructions) / sizeof(Instruction_t);
    uint64_t instruction_count = base_instructions_count;
    memcpy(instructions, base_instructions, sizeof(Instruction_t) * base_instructions_count);

    // reset registers
    memset(registers, 0x00, sizeof(int64_t) * 26);
    registers[0] = start_value;

    int8_t check_value = 0;
    int64_t counter = 0;

    int64_t value0 = 0;
    int64_t value1 = 0;
    int64_t program_counter = 0;
    for (;;) {
        Instruction_t* instruction = &instructions[program_counter];
        switch (instruction->type) {
        case TYPE_INC:
            (*instruction->input_reg)++;
            program_counter++;
            break;
        case TYPE_DEC:
            (*instruction->input_reg)--;
            program_counter++;
            break;
        case TYPE_TGL:
            if (instruction->input_reg == NULL) {
                value0 = instruction->input_val;
            } else {
                value0 = *instruction->input_reg;
            }
            int64_t toggle_index = program_counter + value0;
            Instruction_t* toggle_instruction = &instructions[toggle_index];
            instruction_toggle(toggle_instruction);
            program_counter++;
            break;
        case TYPE_CPY:
            if (instruction->input_reg == NULL) {
                value0 = instruction->input_val;
            } else {
                value0 = *instruction->input_reg;
            }
            *instruction->output_reg = value0;
            program_counter++;
            break;
        case TYPE_JNZ:
            if (instruction->input_reg == NULL) {
                value0 = instruction->input_val;
            } else {
                value0 = *instruction->input_reg;
            }
            if (instruction->output_reg == NULL) {
                value1 = instruction->output_val;
            } else {
                value1 = *instruction->output_reg;
            }

            if (value0 != 0) {
                program_counter += value1;
            } else {
                program_counter++;
            }
            break;
        case TYPE_OUT:
            if (instruction->input_reg == NULL) {
                value0 = instruction->input_val;
            } else {
                value0 = *instruction->input_reg;
            }

            if (check_value != value0) {
                printf("Pattern is not repeating!\n");
                program_counter = 0xFFFF;
            }
            counter++;
            printf("\rTesting [%8ld]: Pattern counter: %12ld ", start_value, counter);
            check_value = 1 - check_value;

            program_counter++;
            break;
        case TYPE_ERR:
            printf("Ftw this should not happen!\n");
            return 1;
        }
        if (program_counter >= instruction_count) {
            printf("Program ended!\n");
            break;
        }
    }
}

//######################################################################################################################

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Could not open file!\n");
        return 1;
    }

    Instruction_t instructions[200];
    uint64_t instruction_size = sizeof(instructions) / sizeof(Instruction_t);
    uint64_t instruction_count = 0;

    int64_t registers[26] = {0};

    char string[32];
    while (fgets(string, 32, p_file)) {
        Instruction_t* instruction = &instructions[instruction_count];
        instruction_from_string(instruction, string, registers);
        instruction_count++;
        if (instruction_count == instruction_size) {
            printf("Instruction array is to small!\n");
            return 1;
        }
    }
    
    for (int64_t i=1; i<0xFFFFFFFF; i++) {
        test_register_value(i, registers, instructions, instruction_count);
    }

    printf("Register A: %ld\n", registers[0]);
    return 0;
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