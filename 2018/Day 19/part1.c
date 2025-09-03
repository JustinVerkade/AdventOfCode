//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Instructions_e Instructions_e;
enum Instructions_e {
    INST_ADDR=0,
    INST_ADDI,
    INST_MULR,
    INST_MULI,
    INST_BANR,
    INST_BANI,
    INST_BORR,
    INST_BORI,
    INST_SETR,
    INST_SETI,
    INST_GTIR,
    INST_GTRI,
    INST_GTRR,
    INST_EQIR,
    INST_EQRI,
    INST_EQRR,
};

typedef struct Instruction_t Instruction_t;
struct Instruction_t {
    Instructions_e type;
    int8_t a;
    int8_t b;
    int8_t c;
};

//######################################################################################################################

void inst_addr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] + registers[b];
}

void inst_addi(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] + b;
}

void inst_mulr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] * registers[b];
}

void inst_muli(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] * b;
}

void inst_banr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] & registers[b];
}

void inst_bani(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] & b;
}

void inst_borr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] | registers[b];
}

void inst_bori(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] | b;
}

void inst_setr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a];
}

void inst_seti(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a;
}

void inst_gtir(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a > registers[b] ? 1 : 0;
}

void inst_gtri(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] > b ? 1 : 0;
}

void inst_gtrr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] > registers[b] ? 1 : 0;
}

void inst_eqir(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a == registers[b] ? 1 : 0;
}

void inst_eqri(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] == b ? 1 : 0;
}

void inst_eqrr(int32_t registers[6], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] == registers[b] ? 1 : 0;
}

void(*instructions[])(int32_t registers[6], int32_t a, int32_t b, int32_t c) = {
    inst_addr, inst_addi, inst_mulr, inst_muli, inst_banr, inst_bani, inst_borr, inst_bori,
    inst_setr, inst_seti, inst_gtir, inst_gtri, inst_gtrr, inst_eqir, inst_eqri, inst_eqrr};
const int32_t instruction_count = 16;

void print_state(int32_t ip_index, int32_t registers[6])
{
    printf("ip=%d [%d, %d, %d, %d, %d, %d]\n",
        registers[ip_index], registers[0], registers[1], registers[2],
        registers[3], registers[4], registers[5]);
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    int32_t pointer_index = 0;
    int32_t registers[6] = {0};
    char instruction[8] = {0};
    int32_t a = 0;
    int32_t b = 0;
    int32_t c = 0;
    fscanf(p_file, "#ip %d\n", &pointer_index);

    Instruction_t buffer[2000];
    int32_t buffer_size = sizeof(buffer) / sizeof(Instruction_t);
    int32_t buffer_count = 0;

    while (fscanf(p_file, "%s %d %d %d\n", instruction, &a, &b, &c) > 0) {
        Instruction_t* inst = &buffer[buffer_count];
        inst->a = a;
        inst->b = b;
        inst->c = c;
        if (!memcmp(instruction, "addr", 4)) {
            inst->type = INST_ADDR;
        } else if (!memcmp(instruction, "addi", 4)) {
            inst->type = INST_ADDI;
        } else if (!memcmp(instruction, "mulr", 4)) {
            inst->type = INST_MULR;
        } else if (!memcmp(instruction, "muli", 4)) {
            inst->type = INST_MULI;
        } else if (!memcmp(instruction, "banr", 4)) {
            inst->type = INST_BANR;
        } else if (!memcmp(instruction, "bani", 4)) {
            inst->type = INST_BANI;
        } else if (!memcmp(instruction, "borr", 4)) {
            inst->type = INST_BORR;
        } else if (!memcmp(instruction, "bori", 4)) {
            inst->type = INST_BORI;
        } else if (!memcmp(instruction, "setr", 4)) {
            inst->type = INST_SETR;
        } else if (!memcmp(instruction, "seti", 4)) {
            inst->type = INST_SETI;
        } else if (!memcmp(instruction, "gtir", 4)) {
            inst->type = INST_GTIR;
        } else if (!memcmp(instruction, "gtri", 4)) {
            inst->type = INST_GTRI;
        } else if (!memcmp(instruction, "gtrr", 4)) {
            inst->type = INST_GTRR;
        } else if (!memcmp(instruction, "eqir", 4)) {
            inst->type = INST_EQIR;
        } else if (!memcmp(instruction, "eqri", 4)) {
            inst->type = INST_EQRI;
        } else if (!memcmp(instruction, "eqrr", 4)) {
            inst->type = INST_EQRR;
        } else {
            printf("Unknown register type!\n");
            fclose(p_file);
            return 1;
        }
        buffer_count++;
        if (buffer_count == buffer_size) {
            printf("Buffer size to small!\n");
            fclose(p_file);
            return 1;
        }
    }
    fclose(p_file);

    while (registers[pointer_index] >= 0 && registers[pointer_index] < buffer_count) {
        Instruction_t* inst = &buffer[registers[pointer_index]];
        instructions[inst->type](registers, inst->a, inst->b, inst->c);
        registers[pointer_index]++;
    }

    int32_t answer = registers[0];
    printf("Answer: %d\n", answer);

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