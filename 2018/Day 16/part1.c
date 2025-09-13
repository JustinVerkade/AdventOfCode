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

typedef struct Test_t Test_t;
struct Test_t {
    int32_t before[4];
    int32_t assembly[4];
    int32_t after[4];
};

//######################################################################################################################

void inst_addr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] + registers[b];
}

void inst_addi(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] + b;
}

void inst_mulr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] * registers[b];
}

void inst_muli(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] * b;
}

void inst_banr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] & registers[b];
}

void inst_bani(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] & b;
}

void inst_borr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] | registers[b];
}

void inst_bori(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] | b;
}

void inst_setr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a];
}

void inst_seti(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a;
}

void inst_gtir(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a > registers[b] ? 1 : 0;
}

void inst_gtri(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] > b ? 1 : 0;
}

void inst_gtrr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] > registers[b] ? 1 : 0;
}

void inst_eqir(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = a == registers[b] ? 1 : 0;
}

void inst_eqri(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] == b ? 1 : 0;
}

void inst_eqrr(int32_t registers[4], int32_t a, int32_t b, int32_t c)
{
    registers[c] = registers[a] == registers[b] ? 1 : 0;
}

void(*instructions[])(int32_t registers[4], int32_t a, int32_t b, int32_t c) = {
    inst_addr, inst_addi, inst_mulr, inst_muli, inst_banr, inst_bani, inst_borr, inst_bori,
    inst_setr, inst_seti, inst_gtir, inst_gtri, inst_gtrr, inst_eqir, inst_eqri, inst_eqrr};
const int32_t instruction_count = 16;

void mask_print(int32_t value)
{
    printf("Mask: ");
    for (int32_t i=15; i>=0; i--) {
        if (value & (1<<i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    Test_t tests[2000];
    int32_t tests_size = sizeof(tests) / sizeof(Test_t);
    int32_t tests_count = 0;

    for (; tests_count<tests_size; tests_count++) {
        int8_t ret = 0;
        ret = fscanf(p_file, "Before: [%d,%d,%d,%d]\n",
            &tests[tests_count].before[0],
            &tests[tests_count].before[1],
            &tests[tests_count].before[2],
            &tests[tests_count].before[3]);
        if (ret != 4) {
            break;
        }
        ret = fscanf(p_file, "%d %d %d %d\n", 
            &tests[tests_count].assembly[0], 
            &tests[tests_count].assembly[1],
            &tests[tests_count].assembly[2],
            &tests[tests_count].assembly[3]);
        if (ret != 4) {
            break;
        }
        ret = fscanf(p_file, "After:  [%d,%d,%d,%d]\n",
            &tests[tests_count].after[0],
            &tests[tests_count].after[1],
            &tests[tests_count].after[2],
            &tests[tests_count].after[3]);
        if (ret != 4) {
            break;
        }
    }

    int32_t masks[16] = {0};
    int32_t masks_size = sizeof(masks) / sizeof(int32_t);
    memset(masks, 0xFF, sizeof(int32_t) * masks_size);

    int32_t counter = 0;
    for (int32_t test_id=0; test_id<tests_count; test_id++) {
        Test_t* test = &tests[test_id];
        int32_t sub_counter = 0;
        for (int32_t inst_id=0; inst_id<instruction_count; inst_id++) {
            int32_t registers[4] = {test->before[0], test->before[1], test->before[2], test->before[3]};
            int32_t instruction = test->assembly[0];
            int32_t a = test->assembly[1];
            int32_t b = test->assembly[2];
            int32_t c = test->assembly[3];
            instructions[inst_id](registers, a, b, c);
            if(memcmp(registers, test->after, sizeof(int32_t) * 4) != 0){
                masks[instruction] &= ~(1<<inst_id);
            } else {
                sub_counter++;
            }
        }
        if (sub_counter >= 3) {
            counter++;
        }
    }

    printf("Answer: %d\n", counter);
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