//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

#define REGISTER_A 0
#define REGISTER_B 1
#define REGISTER_C 2

#define INST_ADV 0
#define INST_BXL 1
#define INST_BST 2
#define INST_JNZ 3
#define INST_BXC 4
#define INST_OUT 5
#define INST_BDV 6
#define INST_CDV 7

typedef struct Cpu Cpu;
struct Cpu
{
    int32_t registers[3];
    int32_t pc;
};

//######################################################################################################################

static void print_cpu(Cpu* cpu)
{
    printf("CPU:\n");
    printf("    Register A:      %d\n", cpu->registers[REGISTER_A]);
    printf("    Register B:      %d\n", cpu->registers[REGISTER_B]);
    printf("    Register C:      %d\n", cpu->registers[REGISTER_C]);
    printf("    Process counter: %d\n\n", cpu->pc);
}

static void print_program(int8_t* program, int32_t program_count)
{
    printf("Program:    %d", program[0]);
    for (int32_t i=1; i<program_count; i++) {
        printf(",%d", program[i]);
    }
    printf("\n");
}

static Cpu get_registers(char* file_name)
{
    FILE* p_file = fopen(file_name, "r ");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return (Cpu){0, 0, 0, -1};
    }
    Cpu cpu;
    fscanf(p_file, "Register A: %d\n", &cpu.registers[REGISTER_A]);
    fscanf(p_file, "Register B: %d\n", &cpu.registers[REGISTER_B]);
    fscanf(p_file, "Register C: %d\n", &cpu.registers[REGISTER_C]);
    fclose(p_file);
    return cpu;
}

static int32_t get_program(char* file_name, int8_t* program, int32_t program_size)
{
    FILE* p_file = fopen(file_name, "r ");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    // move over the register information
    char buffer[256];
    for (int32_t i=0; i<4; i++)
        fgets(buffer, 256, p_file);

    int32_t program_count = 0;
    fgets(buffer, 256, p_file);
    for (int32_t i=9; i<strlen(buffer); i+=2) {
        char character = buffer[i];
        program[program_count] = character - '0';
        program_count++;
        if (program_count == program_size) {
            printf("Program size overflow!\n");
            fclose(p_file);
            return -1;
        }
    }

    fclose(p_file);
    return program_count;
}

static int32_t combo_operand(Cpu* cpu, int8_t value)
{
    if (value >= 0 && value <= 3) {
        return value;
    } else if (value == 4) {
        return cpu->registers[REGISTER_A];
    } else if (value == 5) {
        return cpu->registers[REGISTER_B];
    } else if (value == 6) {
        return cpu->registers[REGISTER_C];
    }
    return 0;
}

static void instruction_adv(Cpu* cpu, int8_t value)
{
    int32_t combo = combo_operand(cpu, value);
    int32_t denominator = 1;
    for (int32_t i=0; i<combo; i++)
        denominator *= 2;
    cpu->registers[REGISTER_A] /= denominator;
    cpu->pc += 2;
}

static void instruction_bxl(Cpu* cpu, int8_t value)
{
    int32_t result = cpu->registers[REGISTER_B] ^ value;
    cpu->registers[REGISTER_B] = result;
    cpu->pc += 2;
}

static void instruction_bst(Cpu* cpu, int8_t value)
{
    int32_t combo = combo_operand(cpu, value);
    int32_t result = combo % 8;
    cpu->registers[REGISTER_B] = result;
    cpu->pc += 2;
}

static void instruction_jnz(Cpu* cpu, int8_t value)
{
    if (cpu->registers[REGISTER_A] == 0) {
        cpu->pc += 2;
        return;
    }
    cpu->pc = value;
}

static void instruction_bxc(Cpu* cpu, int8_t value)
{
    int32_t result = cpu->registers[REGISTER_B] ^ cpu->registers[REGISTER_C];
    cpu->registers[REGISTER_B] = result;
    cpu->pc += 2;
}

static int32_t instruction_out(Cpu* cpu, int8_t value, int8_t* output, int32_t output_size, int32_t output_count)
{
    int32_t combo = combo_operand(cpu, value);
    int32_t result = combo % 8;
    output[output_count] = result;
    cpu->pc += 2;
    return output_count + 1;
}

static void instruction_bdv(Cpu* cpu, int8_t value)
{
    int32_t combo = combo_operand(cpu, value);
    int32_t denominator = 1;
    for (int32_t i=0; i<combo; i++)
        denominator *= 2;
    cpu->registers[REGISTER_B] = cpu->registers[REGISTER_A] / denominator;
    cpu->pc += 2;
}

static void instruction_cdv(Cpu* cpu, int8_t value)
{
    int32_t combo = combo_operand(cpu, value);
    int32_t denominator = 1;
    for (int32_t i=0; i<combo; i++)
        denominator *= 2;
    cpu->registers[REGISTER_C] = cpu->registers[REGISTER_A] / denominator;
    cpu->pc += 2;
}

static int32_t execute_program(Cpu* cpu, int8_t* program, int32_t program_count, int8_t* output, int32_t output_size)
{   
    int32_t output_count = 0;
    memset(output, 0x00, output_size);
    while (cpu->pc < program_count) { 
        int8_t instruction = program[cpu->pc];
        int8_t value = program[cpu->pc + 1];
        switch (instruction) {
        case INST_ADV:
            instruction_adv(cpu, value);
            break;
        case INST_BXL:
            instruction_bxl(cpu, value);
            break;
        case INST_BST:
            instruction_bst(cpu, value);
            break;
        case INST_JNZ:
            instruction_jnz(cpu, value);
            break;
        case INST_BXC:
            instruction_bxc(cpu, value);
            break;
        case INST_OUT:
            output_count = instruction_out(cpu, value, output, output_size, output_count);
            break;
        case INST_BDV:
            instruction_bdv(cpu, value);
            break;
        case INST_CDV:
            instruction_cdv(cpu, value);
            break;
        default:
            printf("Ftw program glitch!\n");
            break;
        }
    }
    return output_count;
}

int solve_puzzle(char* file_name)
{
    Cpu cpu = get_registers(file_name);
    if (cpu.pc == -1) {
        printf("Failed to fetch cpu registers!\n");
        return 1;
    }

    int8_t program[32];
    int32_t program_size = sizeof(program) / sizeof(int8_t);
    int32_t program_count = get_program(file_name, program, program_size);
    if (program_count == -1) {
        printf("Failed to fetch program!\n");
        return 1;
    }

    int8_t output[64];
    int32_t output_size = sizeof(output) / sizeof(char);
    int32_t output_count = execute_program(&cpu, program, program_count, output, output_size);
    print_program(output, output_count);
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