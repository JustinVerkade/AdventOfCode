//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

//######################################################################################################################
// type defines

#define TRHEAD_COUNT        (28u)
#define SEARCH_SPACE        (16777215u + 200u)

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
    uint32_t a;
    uint32_t b;
    uint32_t c;
};

typedef struct ThreadArgument_t ThreadArgument_t;
struct ThreadArgument_t {
    int8_t thread_id;
    pthread_mutex_t *mutex;
    uint32_t *value;
    uint32_t pointer;
    Instruction_t buffer[2000];
    uint32_t buffer_count;
    uint32_t start_value;
    uint32_t end_value;
};

//######################################################################################################################

void inst_addr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] + registers[b];
}

void inst_addi(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] + b;
}

void inst_mulr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] * registers[b];
}

void inst_muli(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] * b;
}

void inst_banr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] & registers[b];
}

void inst_bani(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] & b;
}

void inst_borr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] | registers[b];
}

void inst_bori(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] | b;
}

void inst_setr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a];
}

void inst_seti(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = a;
}

void inst_gtir(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = a > registers[b] ? 1llu : 0llu;
}

void inst_gtri(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] > b ? 1llu : 0llu;
}

void inst_gtrr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] > registers[b] ? 1llu : 0llu;
}

void inst_eqir(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = a == registers[b] ? 1llu : 0llu;
}

void inst_eqri(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] == b ? 1llu : 0llu;
}

void inst_eqrr(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c)
{
    registers[c] = registers[a] == registers[b] ? 1llu : 0llu;
}

void(*instructions[])(uint32_t registers[6], uint32_t a, uint32_t b, uint32_t c) = {
    inst_addr, inst_addi, inst_mulr, inst_muli, inst_banr, inst_bani, inst_borr, inst_bori,
    inst_setr, inst_seti, inst_gtir, inst_gtri, inst_gtrr, inst_eqir, inst_eqri, inst_eqrr};
const uint32_t instruction_count = 16;

void print_state(uint32_t ip_index, uint32_t registers[6])
{
    printf("ip=%u [%u, %u, %u, %u, %u, %u]\n",
        registers[ip_index], registers[0], registers[1], registers[2],
        registers[3], registers[4], registers[5]);
}

void print_inst(int32_t pointer, Instruction_t* inst, uint32_t* registers)
{
    printf("%2d ", pointer);
    switch (inst->type) {
    case INST_ADDR:
        printf("ADDR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_ADDI:
        printf("ADDI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_MULR:
        printf("MULR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_MULI:
        printf("MULI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_BANR:
        printf("BANR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_BANI:
        printf("BANI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_BORR:
        printf("BORR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_BORI:
        printf("BORI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_SETR:
        printf("SETR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_SETI:
        printf("SETI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_GTIR:
        printf("GTIR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_GTRI:
        printf("GTRI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_GTRR:
        printf("GTRR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_EQIR:
        printf("EQIR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_EQRI:
        printf("EQRI: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    case INST_EQRR:
        printf("EQRR: [%u, %u, %u, %u, %u, %u]\n", registers[0], registers[1], registers[2], registers[3], registers[4], registers[5]);
        break;
    }
}

uint32_t test_program(uint32_t start_value, Instruction_t* buffer, uint32_t buffer_count, uint32_t pointer_index)
{
    uint32_t registers[6] = {start_value, 0, 0, 0, 0, 0};
    uint32_t counter = 0;
    uint32_t counter_timeout = 6400;
    while (registers[pointer_index] >= 0 && registers[pointer_index] < buffer_count && ++counter < counter_timeout) {
        int32_t pointer = registers[pointer_index];
        Instruction_t* inst = &buffer[pointer];
        instructions[inst->type](registers, inst->a, inst->b, inst->c);
        registers[pointer_index]++;
    }
    if (counter == counter_timeout) {
        return 0;
    }
    return counter;
}

void* thread_func(void* arg)
{
    ThreadArgument_t* arguments = (ThreadArgument_t*)arg;
    pthread_mutex_lock(arguments->mutex);
    printf("Thread %2ld started <> [%u -> %u]\n", arguments->thread_id, arguments->start_value, arguments->end_value);
    pthread_mutex_unlock(arguments->mutex);
    for (uint32_t test=arguments->start_value; test<=arguments->end_value; test++) {
        uint32_t result = test_program(test, arguments->buffer, arguments->buffer_count, arguments->pointer);
        if (result == 0) {
            continue;
        }
        pthread_mutex_lock(arguments->mutex);
        if (*arguments->value > test) {
            *arguments->value = test;
            printf("Thread %2ld found lowest <> %u\n", arguments->thread_id, test);
        }
        pthread_mutex_unlock(arguments->mutex);
    }

    pthread_mutex_lock(arguments->mutex);
    printf("Thread %2ld finished.\n", arguments->thread_id);
    pthread_mutex_unlock(arguments->mutex);

    return NULL;
}

int solve_puzzle(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    uint32_t pointer_index = 0;
    char instruction[8] = {0};
    uint32_t a = 0;
    uint32_t b = 0;
    uint32_t c = 0;
    fscanf(p_file, "#ip %d\n", &pointer_index);

    Instruction_t buffer[2000];
    uint32_t buffer_size = sizeof(buffer) / sizeof(Instruction_t);
    uint32_t buffer_count = 0;

    while (fscanf(p_file, "%s %u %u %u\n", instruction, &a, &b, &c) > 0) {
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

    uint32_t best_value = 0x7FFFFFFF;
    pthread_t threads[TRHEAD_COUNT];
    pthread_mutex_t mutex = {0};
    pthread_mutex_init(&mutex, NULL);
    ThreadArgument_t arguments[TRHEAD_COUNT];
    for (uint32_t i=0; i<TRHEAD_COUNT; i++) {
        arguments[i].thread_id = i + 1;
        arguments[i].mutex = &mutex;
        arguments[i].value = &best_value;
        arguments[i].pointer = pointer_index;
        memcpy(arguments[i].buffer, buffer, sizeof(Instruction_t) * 2000);
        arguments[i].buffer_count = buffer_count;
        arguments[i].start_value = SEARCH_SPACE / TRHEAD_COUNT * i;
        arguments[i].end_value = SEARCH_SPACE / TRHEAD_COUNT * (i + 1) - 1;
        pthread_create(&threads[i], NULL, thread_func, &arguments[i]);
    }

    for (uint32_t i=0; i<TRHEAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    uint32_t answer = best_value;
    printf("Answer: %u\n", answer);

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