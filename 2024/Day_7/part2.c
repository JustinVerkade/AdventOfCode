//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

typedef struct Input Input;
struct Input
{
    uint64_t result;
    uint64_t values[256];
    uint64_t value_count;
};

//######################################################################################################################

static uint64_t get_formula_result(char* buffer)
{   
    char value_buffer[64];
    memset(value_buffer, 0x00, 64);
    char* p_index = buffer;
    char* p_buffer = value_buffer;
    while ((*p_index >= '0') && (*p_index <= '9')) {
        *p_buffer = *p_index;
        p_buffer++;
        p_index++;
    }
    return atoll(value_buffer);
}

static void get_formula_values(char* buffer, Input* data)
{
    strtok(buffer, " ");
    char* p_buffer = strtok(NULL, " ");
    while (p_buffer) {
        data->values[data->value_count] = atoi(p_buffer);
        p_buffer = strtok(NULL, " ");
        data->value_count++;
    }
}

typedef enum Operator Operator;
enum Operator
{
    PLUS=0,
    MUL,
    CON,
    COUNT,
};

static uint64_t recursive(uint64_t target, uint64_t total, uint64_t* values, uint64_t len)
{
    if ((target == total) && (len == 0)) {
        return 1;
    } else if ((target != total) && (len == 0)) {
        return 0;
    } else if (total > target) {
        return 0;
    }
    
    for (uint64_t i=0; i<(uint64_t)COUNT; i++) {
        if (i == PLUS) {
            uint64_t new_total = total + values[0];
            uint64_t* new_values = &values[1];
            uint64_t new_len = len - 1;
            int8_t ret = recursive(target, new_total, new_values, new_len);
            if (ret == 1)
                return 1;
        } else if (i == MUL) {
            uint64_t new_total = total * values[0];
            uint64_t* new_values = &values[1];
            uint64_t new_len = len - 1;
            int8_t ret = recursive(target, new_total, new_values, new_len);
            if (ret == 1)
                return 1;
        } else if (i == CON) {
            char buffer[32];
            sprintf(buffer, "%ld%ld", total, values[0]);
            uint64_t new_total = atoll(buffer);
            uint64_t* new_values = &values[1];
            uint64_t new_len = len - 1;
            int8_t ret = recursive(target, new_total, new_values, new_len);
            if (ret == 1)
                return 1;
        } else {
            // do nothing
        }
    }
    return 0;
}

static uint64_t find_operators(Input data)
{
    int8_t result = recursive(data.result, 0, data.values, data.value_count);
    if (result == 1)
        return data.result;
    return 0;
}

static uint64_t process_data(char* file_name)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return 0;
    }

    uint64_t answer = 0;
    char buffer[256];
    while (fgets(buffer, 256, p_file)) {
        Input data;
        data.value_count = 0;
        data.result = get_formula_result(buffer);
        get_formula_values(buffer, &data);
        answer += find_operators(data);
    }

    return answer;
}

int solve_puzzle(char* file_name)
{
    uint64_t answer = process_data(file_name);
    printf("Answers: %ld\n", answer);
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