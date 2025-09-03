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
    printf("ip=%2d [%12d, %12d, %12d, %12d, %12d, %12d]\n",
        registers[ip_index], registers[0], registers[1], registers[2],
        registers[3], registers[4], registers[5]);
}

int solve_puzzle(char* file_name)
{

    int32_t a = 1;
    int32_t c = 0;
    int32_t d = 0;
    int32_t e = 0;
    int32_t f = 0;

    e += 2;
    e = e * e;
    e *= 19;
    e *= 11;
    c += 5;
    c *= 22;
    c += 12;
    e += c;
    if (a) {
        c = 27;
        c *= 28;
        c += 29;
        c *= 30;
        c *= 14;
        c *= 32;
        e += c;
        a = 0;
    }

    // reverse engineered program
    // f = 1;
    // for (;;) {
    //     d = 1;
    //     for (;;) {
    //         if (f * d == e) {
    //             a += f;
    //             printf("[%d %d, %d, %d, %d]\n", a, c, d, e, f);
    //         }
    //         d += 1;
    //         if (d > e) {
    //             f += 1;
    //             if (f > e) {
    //                 goto GOTO_16;
    //             }
    //             break;
    //         }
    //     }
    // }
    // GOTO_16:
    // int32_t answer = a;

    // optimized
    int32_t answer = 0;
    for (int32_t i=1; i<=e; i++) {
        if (e % i == 0) {
            answer += i;
        }
    }

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