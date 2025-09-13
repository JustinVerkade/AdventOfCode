//######################################################################################################################

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

//######################################################################################################################
// type defines

#define BUFFER_SIZE 5000

//######################################################################################################################

static int32_t puzzle_get_input(char* file_name, int32_t* numbers_left, int32_t* numbers_right, int32_t size)
{
    FILE* p_file = fopen(file_name, "r");
    if (p_file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    } 

    char buffer[64];
    int32_t numbers_index = 0;
    while(fgets(buffer, 64, p_file))
    {
        int32_t number_left;
        int32_t number_right;
        sscanf(buffer, "%d   %d", &number_left, &number_right);
        numbers_left[numbers_index] = number_left;
        numbers_right[numbers_index] = number_right;
        numbers_index++;
        if (numbers_index == size) {
            printf("Number buffer overflowed!\n");
            return -1;
        }
    }

    fclose(p_file);
    return numbers_index;
}

static void sort_numbers(int32_t* numbers, int32_t len)
{
    int8_t done_sorting = 0;
    int32_t timeout = 1000000;
    while ((done_sorting == 0) && (timeout--)) {
        int8_t swapped = 0;
        for (int32_t i=0; i<len-1; i++) {
            int32_t value_1 = numbers[i];
            int32_t value_2 = numbers[i + 1];
            if (value_1 <= value_2)
                continue;
            int32_t temp = value_1;
            value_1 = value_2;
            value_2 = temp;
            numbers[i] = value_1;
            numbers[i + 1] = value_2;
            swapped = 1;
            break;
        }
        if (swapped == 0)
            done_sorting = 1;
    }
}

static int32_t find_value_occurances(int32_t value, int32_t* numbers, int32_t len)
{
    int32_t occurances = 0;
    for (int32_t i=0; i<len; i++) {
        int32_t number_value = numbers[i];
        occurances += (number_value == value);
    }
    return occurances;
}

int solve_puzzle(char* file_name)
{
    int32_t numbers_left[BUFFER_SIZE];
    int32_t numbers_right[BUFFER_SIZE];
    int32_t number_count = sizeof(numbers_left) / sizeof(int32_t);
    int32_t input_size = puzzle_get_input(file_name, numbers_left, numbers_right, number_count);
    if (input_size == -1) {
        printf("Puzzle failed!\n");
        return 1;
    }

    sort_numbers(numbers_left, input_size);
    sort_numbers(numbers_right, input_size);

    int32_t delta_size = 0;
    for (int32_t i=0; i<input_size; i++) {
        int32_t value = numbers_left[i];
        int32_t occurances = find_value_occurances(value, numbers_right, input_size);
        int32_t multiplication = value * occurances;
        delta_size += multiplication;
    }

    printf("Answer: %d\n", delta_size);
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