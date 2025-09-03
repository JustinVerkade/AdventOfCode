//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines



//######################################################################################################################

static char* puzzle_get_input(char* file_name, int32_t* len)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return NULL;
    }

    fseek(p_file, 0, SEEK_END);
    int32_t input_length = ftell(p_file);
    fseek(p_file, 0, SEEK_SET);
    *len = input_length;

    char* input_data = (char*)calloc(sizeof(char), input_length + 1);
    if (input_data == NULL) {
        printf("Failed to allocate input data memory!\n");
        fclose(p_file);
        return NULL;
    }

    fread(input_data, input_length, input_length, p_file);

    fclose(p_file);

    return input_data;
}

static int32_t get_value(char* p_value, int8_t is_first)
{
    // copy characters until character is not a digit
    char buffer[64];
    memset(buffer, 0x00, 64);
    int32_t index = 0;
    while ((p_value[index] >= '0') && (p_value[index] <= '9')) {
        buffer[index] = p_value[index];
        index++;
    }

    // reject mul if end is not a comma or bracket
    if ((p_value[index] != ',') && is_first == 1)
        return 0;
    if ((p_value[index] != ')') && is_first == 0)
        return 0;

    int32_t value = atoi(buffer);
    return value;
}

static int32_t check_mul(char* mul_start)
{
    if (memcmp(mul_start, "mul(", 4) != 0)
        return 0;

    char* p_first_value = mul_start + 4;
    int32_t first_value = get_value(p_first_value, 1);

    char* p_index = p_first_value;
    for (; *p_index!=','; p_index++);
    p_index++;

    char* p_second_value = p_index;
    int32_t second_value = get_value(p_second_value, 0);
    int32_t multiplication = first_value * second_value;

    return first_value * second_value;
}

static int32_t solve_puzzle_input(char* input_data, int32_t input_length)
{
    int32_t sum_of_mul = 0;
    for (int32_t i=0; i<input_length; i++) {
        char* start_pointer = &input_data[i];
        char start_character = *start_pointer;
        if (start_character != 'm')
            continue;
        sum_of_mul += check_mul(start_pointer);
    }
    return sum_of_mul;
}

int solve_puzzle(char* file_name)
{
    int32_t input_length = 0;
    char* input_data = puzzle_get_input(file_name, &input_length);
    if (input_data == NULL) {
        printf("Fetching input failed!\n");
        return 1;
    }

    int32_t answer = solve_puzzle_input(input_data, input_length);
    free(input_data);

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