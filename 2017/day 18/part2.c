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

typedef struct Program_t Program_t;
struct Program_t {
    int8_t id;
    Register_t registers[26];
    Instruction_t instructions[2000];
    int64_t instruction_size;
    int64_t instruction_count;
    int64_t instruction_index;
    int64_t current_sound;

    struct Program_t* thread;

    int64_t buffer[20000];
    int64_t buffer_write_ptr;
    int64_t buffer_read_ptr;
    int64_t buffer_size;

    int64_t send_counter;
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

Program_t* program_create(char* file_name)
{
    Program_t* program = (Program_t*)malloc(sizeof(Program_t));
    if (program == NULL) {
        printf("Failed to allocate memory for program!\n");
        return NULL;
    }

    program->instruction_size = 2000;
    program->instruction_count = 0;

    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL ){
        printf("Failed to open file!\n");
        free(program);
        return NULL;
    }

    char instruction_string[32];
    while (fgets(instruction_string, 32, p_file)) {
        Instruction_t* instruction = &program->instructions[program->instruction_count];
        parse_instruction_string(instruction_string, instruction, program->registers);
        program->instruction_count++;
        if (program->instruction_count == program->instruction_size) {
            printf("Buffer overflowed!\n");
            free(program);
            fclose(p_file);
            return NULL;
        }
    }
    fclose(p_file);

    memset(program->registers, 0x00, 26);
    program->instruction_index = 0;
    program->current_sound = 0;
    
    memset(program->buffer, 0x00, sizeof(int64_t) * 2000);
    program->buffer_write_ptr = 0;
    program->buffer_read_ptr = 0;
    program->buffer_size = 2000;
    
    program->send_counter = 0;

    return program;
}

int8_t program_push(Program_t* program, int64_t value)
{
    program->buffer[program->buffer_write_ptr] = value;
    program->buffer_write_ptr = (program->buffer_write_ptr + 1) % program->buffer_size;
    if (program->buffer_write_ptr == program->buffer_read_ptr) {
        printf("Buffer overflow!\n");
        return 1;
    }
    program->send_counter++;
    return 0;
}

int8_t program_pop(Program_t* program, int64_t* value)
{
    if (program->buffer_write_ptr == program->buffer_read_ptr) {
        return 0;
    }
    *value = program->buffer[program->buffer_read_ptr];
    program->buffer_read_ptr = (program->buffer_read_ptr + 1) % program->buffer_size;
    return 1;
}

int8_t program_step(Program_t* program)
{
    Instruction_t* instruction = &program->instructions[program->instruction_index];
    switch (instruction->type) {
    case INST_SND:
        int64_t send_value = 0;
        if (instruction->register_a == NULL) {
            send_value = instruction->value_a;
        } else {
            send_value = instruction->register_a->value;
        }
        program_push(program->thread, send_value);
        program->instruction_index++;
        break;
    case INST_SET:
        if (instruction->register_b == NULL) {
            instruction->register_a->value = instruction->value_b;
        } else {
            instruction->register_a->value = instruction->register_b->value;
        }
        program->instruction_index++;
        break;
    case INST_ADD:
        if (instruction->register_b == NULL) {
            instruction->register_a->value += instruction->value_b;
        } else {
            instruction->register_a->value += instruction->register_b->value;
        }
        program->instruction_index++;
        break;
    case INST_MUL:
        if (instruction->register_b == NULL) {
            instruction->register_a->value *= instruction->value_b;
        } else {
            instruction->register_a->value *= instruction->register_b->value;
        }
        program->instruction_index++;
        break;
    case INST_MOD:
        if (instruction->register_b == NULL) {
            instruction->register_a->value %= instruction->value_b;
        } else {
            instruction->register_a->value %= instruction->register_b->value;
        }
        program->instruction_index++;
        break;
    case INST_RCV:
        int64_t value = 0;
        int8_t received = program_pop(program, &value);
        if (received == 0) {
            return 1;
        } 
        instruction->register_a->value = value;
        program->instruction_index++;
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
            program->instruction_index += value_b;
        } else {
            program->instruction_index++;
        }
        break;
    }
    return 0;
}

int solve_puzzle(char* file_name)
{
    Program_t* program_0 = program_create(file_name);
    Program_t* program_1 = program_create(file_name);

    program_0->id = 0;
    program_1->id = 1;
    program_0->registers['p' - 'a'].value = 0;
    program_1->registers['p' - 'a'].value = 1;
    program_0->thread = program_1;
    program_1->thread = program_0;

    for (;;) {
        int8_t waiting = 0;
        waiting += program_step(program_0);
        waiting += program_step(program_1);
        if (waiting == 2) {
            break;
        }
    }

    printf("Answer: %ld\n", program_0->send_counter);
    free(program_0);
    free(program_1);
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