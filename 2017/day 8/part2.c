//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef struct Register_t Register_t;
struct Register_t {
    char id[4];
    int32_t value;
};

typedef enum InstructionType_e InstructionType_e;
enum InstructionType_e {
    TYPE_INC=0,
    TYPE_DEC,
};

typedef enum Check_e Check_e;
enum Check_e {
    CHECK_GREATER_THEN=0,
    CHECK_SMALLER_THEN,
    CHECK_GREATER_OR_EQUAL_THEN,
    CHECK_SMALLER_OR_EQUAL_THEN,
    CHECK_EQUAL_TO,
    CHECK_NOT_EQUAL_TO,
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t {
    Register_t* operation_register;
    InstructionType_e operation;
    int32_t operation_value;
    Register_t* check_register;
    Check_e check;
    int32_t check_value;
};

//######################################################################################################################

Register_t* register_add(char id[4], Register_t* registers, int32_t* register_count, int32_t register_size)
{
    // if register already exists return the register
    for (int32_t i=0; i<*register_count; i++) {
        Register_t* reg = &registers[i];
        if (!memcmp(id, reg->id, strlen(reg->id)) && strlen(id) == strlen(reg->id)) {
            return reg;
        }
    }

    // add the register to the register array
    Register_t* reg = &registers[*register_count];
    memcpy(registers[*register_count].id, id, strlen(id));
    registers[*register_count].value = 0;
    (*register_count)++;
    if (*register_count == register_size) {
        printf("Register buffer overflowed!\n");
        return NULL;
    }
    return reg;
}

int8_t instruction_check(Instruction_t* instruction)
{
    int32_t register_value = instruction->check_register->value;
    int32_t check_value = instruction->check_value;
    switch (instruction->check) {
    case CHECK_GREATER_THEN:
        return register_value > check_value;
    case CHECK_SMALLER_THEN:
        return register_value < check_value;
    case CHECK_GREATER_OR_EQUAL_THEN:
        return register_value >= check_value;
    case CHECK_SMALLER_OR_EQUAL_THEN:
        return register_value <= check_value;
    case CHECK_EQUAL_TO:
        return register_value == check_value;
    case CHECK_NOT_EQUAL_TO:
        return register_value != check_value;
    }
    return 0;
}

void instruction_execute(Instruction_t* instruction)
{
    int32_t register_value = instruction->operation_value;
    switch (instruction->operation) {
    case TYPE_INC:
        instruction->operation_register->value += register_value;
        break;
    case TYPE_DEC:
        instruction->operation_register->value -= register_value;
        break;
    }
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Instruction_t instructions[1600] = {0};
    int32_t instructions_size = sizeof(instructions) / sizeof(Instruction_t);
    int32_t instructions_count = 0;

    Register_t registers[1600] = {0};
    int32_t registers_size = sizeof(registers) / sizeof(Register_t);
    int32_t registers_count = 0;

    char id[4] = {0};
    char inst[4] = {0};
    int32_t operand = 0;
    char check_id[4] = {0};
    char check[3] = {0};
    int32_t check_value = 0;

    while (fscanf(p_file, "%s %s %d if %s %s %d\n", id, inst, &operand, check_id, check, &check_value) == 6) {
        Instruction_t* instruction = &instructions[instructions_count];
        Register_t* operation_register = register_add(id, registers, &registers_count, registers_size);
        if (operation_register == NULL) {
            printf("Failed to add register to array!\n");
        }
        instruction->operation_register = operation_register;
        if (!memcmp(inst, "inc", 3)) {
            instruction->operation = TYPE_INC;
        } else {
            instruction->operation = TYPE_DEC;
        }
        instruction->operation_value = operand;

        Register_t* check_register = register_add(check_id, registers, &registers_count, registers_size);
        if (check_register == NULL) {
            printf("Failed to add register to array!\n");
        }
        instruction->check_register = check_register;
        if (!memcmp(check, "<=", 2)) {
            instruction->check = CHECK_SMALLER_OR_EQUAL_THEN;
        } else if (!memcmp(check, ">=", 2)) {
            instruction->check = CHECK_GREATER_OR_EQUAL_THEN;
        } else if (!memcmp(check, "!=", 2)) {
            instruction->check = CHECK_NOT_EQUAL_TO;
        } else if (!memcmp(check, "==", 2)) {
            instruction->check = CHECK_EQUAL_TO;
        } else if (!memcmp(check, ">", 1)) {
            instruction->check = CHECK_GREATER_THEN;
        } else if (!memcmp(check, "<", 1)) {
            instruction->check = CHECK_SMALLER_THEN;
        } else {
            printf("Faulty: %s\n", check);
            for (;;);
        }
        instruction->check_value = check_value;

        instructions_count++;
        if (instructions_count == instructions_size) {
            printf("Instructions array overflowed!\n");
            return 1;
        }

        printf("Test: %s %s %d if %s %s %d\n", id, inst, operand, check_id, check, check_value);
    }

    // print stats
    printf("Registers: %d\n", registers_count);
    printf("Instructions: %d\n", instructions_count);

    printf("Running instructions!\n");
    int32_t answer = 0;
    for (int32_t index=0; index<instructions_count; index++) {
        Instruction_t* instruction = &instructions[index];
        int8_t valid = instruction_check(instruction);
        if (valid == 1) {
            instruction_execute(instruction);
        }

        for (int32_t index=0; index<registers_count; index++) {
            Register_t* reg = &registers[index];
            int32_t value = reg->value;
            if (value > answer) {
                answer = value;
            }
        }

    }

    


    printf("Answer: %d\n", answer);
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