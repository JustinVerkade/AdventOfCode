//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Register_t Register_t;
struct Register_t {
    char id;
    int64_t value;
};

typedef enum InstructionType_e InstructionType_e;
enum InstructionType_e {
    INST_SND=0,
    INST_SET,
    INST_ADD,
    INST_MUL,
    INST_MOD,
    INST_RCV,
    INST_JGZ,
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t{
    InstructionType_e type;
    Register_t* register_a;
    Register_t* register_b;
    int64_t value_a;
    int64_t value_b;
};

//######################################################################################################################

void parse_instruction_string(char* string, Instruction_t* instruction, Register_t* registers)
{
    if (!memcmp(string, "snd", 3)) {
        instruction->type = INST_SND;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
    } else if (!memcmp(string, "set", 3)) {
        instruction->type = INST_SET;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_b = &registers[index];
        } else {
            instruction->register_b = NULL;
            instruction->value_b = atoi(token);
        }
    } else if (!memcmp(string, "add", 3)) {
        instruction->type = INST_ADD;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_b = &registers[index];
        } else {
            instruction->register_b = NULL;
            instruction->value_b = atol(token);
        }
    } else if (!memcmp(string, "mul", 3)) {
        instruction->type = INST_MUL;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_b = &registers[index];
        } else {
            instruction->register_b = NULL;
            instruction->value_b = atoi(token);
        }
    } else if (!memcmp(string, "mod", 3)) {
        instruction->type = INST_MOD;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_b = &registers[index];
        } else {
            instruction->register_b = NULL;
            instruction->value_b = atoi(token);
        }
    } else if (!memcmp(string, "rcv", 3)) {
        instruction->type = INST_RCV;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
    } else if (!memcmp(string, "jgz", 3)) {
        instruction->type = INST_JGZ;
        char* token = strtok(string, " ");
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_a = &registers[index];
        } else {
            instruction->register_a = NULL;
            instruction->value_a = atoi(token);
        }
        token = strtok(NULL, " ");
        if (token[0] >= 'a' && token[0] <= 'z') {
            int64_t index = token[0] - 'a';
            instruction->register_b = &registers[index];
        } else {
            instruction->register_b = NULL;
            instruction->value_b = atoi(token);
        }
    } else {
        printf("FTW is this for instruction!\n");
    }
}

int solve_puzzle(char* file_name)
{
    Register_t registers[26] = {0};
    Instruction_t instructions[100] = {0};
    int64_t instruction_size = sizeof(instructions) / sizeof(Instruction_t);
    int64_t instruction_count = 0;

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL ){
        printf("Failed to open file!\n");
        return 1;
    }

    char instruction_string[32];
    while (fgets(instruction_string, 32, p_file)) {
        Instruction_t* instruction = &instructions[instruction_count];
        parse_instruction_string(instruction_string, instruction, registers);
        instruction_count++;
        if (instruction_count == instruction_size) {
            printf("Buffer overflowed!\n");
            return 1;
        }
    }

    int64_t current_sound = 0;
    int64_t last_sound = 0;
    int64_t instruction_index = 0;
    int64_t counter = 1000000;
    while (instruction_index >= 0 && instruction_index < instruction_count && counter--) {
        Instruction_t* instruction = &instructions[instruction_index];
        switch (instruction->type) {
        case INST_SND:
            if (instruction->register_a == NULL) {
                current_sound = instruction->value_a;
            } else {
                current_sound = instruction->register_a->value;
            }
            instruction_index++;
            break;
        case INST_SET:
            if (instruction->register_b == NULL) {
                instruction->register_a->value = instruction->value_b;
            } else {
                instruction->register_a->value = instruction->register_b->value;
            }
            instruction_index++;
            break;
        case INST_ADD:
            if (instruction->register_b == NULL) {
                instruction->register_a->value += instruction->value_b;
            } else {
                instruction->register_a->value += instruction->register_b->value;
            }
            instruction_index++;
            break;
        case INST_MUL:
            if (instruction->register_b == NULL) {
                instruction->register_a->value *= instruction->value_b;
            } else {
                instruction->register_a->value *= instruction->register_b->value;
            }
            instruction_index++;
            break;
        case INST_MOD:
            if (instruction->register_b == NULL) {
                instruction->register_a->value %= instruction->value_b;
            } else {
                instruction->register_a->value %= instruction->register_b->value;
            }
            instruction_index++;
            break;
        case INST_RCV:
            int64_t value = 0;
            if (instruction->register_a == NULL) {
                value = instruction->value_a;
            } else {
                value = instruction->register_a->value;
            }
            if (value != 0) {
                printf("Answer: %ld\n", current_sound);
                return 1;
                last_sound = current_sound;
            }
            instruction_index++;
            break;
        case INST_JGZ:
            int64_t value_a = 0;
            int64_t value_b = 0;
            if (instruction->register_a == NULL) {
                value_a = instruction->value_a;
            } else {
                value_a = instruction->register_a->value;
            }
            if (instruction->register_b == NULL) {
                value_b = instruction->value_b;
            } else {
                value_b = instruction->register_b->value;
            }

            if (value_a > 0) {
                instruction_index += value_b;
            } else {
                instruction_index++;
            }
            break;
        }
    }

    fclose(p_file);
    printf("File: %s\n", file_name);
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